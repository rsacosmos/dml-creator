#include "MainWindow.h"
#include <QDesktopServices>
#include <QFile>
#include <QCloseEvent>
#include <QFileInfo>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QDomDocument>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QDebug>

#include <xml/Exporter.h>
#include <xml/Importer.h>
#include <conf/ConfigurationManager.h>

static const QString CACHED_LOCATION_KEY("cachedLocation");

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
  , _scan(0)
{
    // Create central widget.
    _ui.setupUi(this);

    _ui.actionNew->setShortcut(QKeySequence::New);
    _ui.actionOpen->setShortcut(QKeySequence::Open);
    _ui.actionSave->setShortcut(QKeySequence::Save);
    _ui.actionSave_as->setShortcut(QKeySequence::SaveAs);
    _ui.actionQuit->setShortcut(QKeySequence::Quit);

    connect(_ui.actionNew, &QAction::triggered, this, &MainWindow::onNewShowRequested);
    connect(_ui.actionOpen, &QAction::triggered, this, &MainWindow::onOpenShowRequested);
    connect(_ui.actionImport, &QAction::triggered, this, &MainWindow::onImportShowRequested);
    connect(_ui.actionSave, &QAction::triggered, this, &MainWindow::onSaveShowRequested);
    connect(_ui.actionSave_as, &QAction::triggered, this, &MainWindow::onSaveAsShowRequested);
    connect(_ui.actionQuit, &QAction::triggered, this, &MainWindow::onQuitRequested);
    
    connect(_ui.actionEdit_configuration_file, &QAction::triggered, this, &MainWindow::onEditConfigFileRequested);
    connect(_ui.actionSelect_configuration_file, &QAction::triggered, this, &MainWindow::onSelectConfigFileRequested);
    connect(_ui.actionSelect_AFDI_schema, &QAction::triggered, this, &MainWindow::onSelectAFDISchemaFileRequested);
    
    connect(_ui.actionHowTo, &QAction::triggered, this, &MainWindow::onHelpContentRequested);
    connect(_ui.actionAbout, &QAction::triggered, this, &MainWindow::onAboutRequested);
    connect(_ui.actionAboutQt, &QAction::triggered, this, &MainWindow::onAboutQtRequested);
    
    connect(_ui.showProperties, &InformationWidget::informationChanged, this, &MainWindow::onInformationChanged);
    connect(_ui.videoWidget, &VideoWidget::videoListChanged, this, &MainWindow::onVideoListChanged);
    connect(_ui.audioWidget, &AudioWidget::audioListChanged, this, &MainWindow::onAudioListChanged);
    connect(_ui.subtitlesWidget, &SubtitlesWidget::subtitlesListChanged, this, &MainWindow::onSubtitlesListChanged);

    connect((QGuiApplication*) QApplication::instance(), &QGuiApplication::focusObjectChanged, this, &MainWindow::focusObjectChanged);

    // Scan dialog.
    connect(&_scanDialog, &ProgressDialog::cancel, this, &MainWindow::onAbortScan);

    // Init texts.
    updateTexts();
    
    loadStoredData();
    _currentShow.clear();
    _ui.videoWidget->setCurrentShow(&_currentShow);
    _ui.subtitlesWidget->setCurrentShow(&_currentShow);
    _ui.audioWidget->setCurrentShow(&_currentShow);
    
    // Ask for user to choose default config filename if not set.
    if (_configFilename.isEmpty() || !QFile::exists(_configFilename))
        onSelectConfigFileRequested();

    // Ask for user to choose default config filename if not set.
    if (_schemaFilename.isEmpty() || !QFile::exists(_schemaFilename))
        onSelectAFDISchemaFileRequested();
}

void MainWindow::updateTexts()
{
    // Retranslate texts if needed
    _ui.retranslateUi(this);

    // Window title.
    setWindowTitle(qApp->applicationName());

    // Show properties layout.
    _ui.showProperties->updateTexts();
}

void MainWindow::reloadShow()
{
    QMutexLocker mutexLocker(_currentShow.locker());

    _ui.subtitlesWidget->setSubtitlesList(_currentShow.subtitlesList());
    _ui.audioWidget->setAudioList(_currentShow.audioList());
    _ui.videoWidget->setVideoList(_currentShow.videoList());
    _ui.rootPathValue->setText(_currentShow.rootPath().c_str());
    _ui.showProperties->reloadInformation(_currentShow.information());
}

void MainWindow::focusObjectChanged(QObject *focusObject)
{
    QWidget *widget = dynamic_cast<QWidget*>(focusObject);
    if (!widget)
        return;

    QWidget *parent = widget->parentWidget();
    if (!parent)
        return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    onQuitRequested();
    event->ignore();
}

void MainWindow::openShow(const QString &filename)
{
    closeShow();
    
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QString dml = file.readAll();
    file.close();

    _currentShow = dml::xml::Importer::fromDml(dml.toStdString());
    _currentShow.setModified(true);
    _ui.videoWidget->setCurrentShow(&_currentShow);
    _currentShowFilename = filename;

    QFileInfo fileinfo(_currentShowFilename);
    _currentShow.setRootPath(fileinfo.absolutePath().toStdString());
    
    reloadShow();
}

void MainWindow::closeShow()
{
    loadStoredData();
    _currentShow.clear();
    _currentShow.setModified(false);
    reloadShow();
}

void MainWindow::onNewShowRequested()
{
    if (!userConfirmClosingCurrentShow())
        return;
    
    closeShow();
}

void MainWindow::onOpenShowRequested()
{
    if (!userConfirmClosingCurrentShow())
        return;
    
    // Getting the cached location of the previous GetOpenFileName
    QSettings settings;
    QString cachedLocation = settings.value(CACHED_LOCATION_KEY).toString();

    // Get the filename
    QString filename = QFileDialog::getOpenFileName(this, tr("Open show"), cachedLocation, tr("Show files (*.dml);;All files (*.*)"));

    // Cancel case
    if (filename.isEmpty())
        return;

    // Cache the location for the next GetOpenFileName.
    settings.setValue(CACHED_LOCATION_KEY, filename);

    openShow(filename);
}

void MainWindow::onSaveShowRequested()
{
    // No filename defined.
    if (_currentShowFilename.isEmpty())
        return onSaveAsShowRequested();
       
    std::string dmlContent = dml::xml::Exporter::toDml(_currentShow).toString(2);
    
    // Check for Dml content.    
    const QByteArray instanceData(dmlContent.c_str());
    QFile standardFile(_schemaFilename);
    if (!standardFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Invalid AFDI standard"), tr("Can not open the standard file"));
        return;
    }
    QXmlSchema xmlSchema; 
    xmlSchema.load(&standardFile, QUrl::fromLocalFile(standardFile.fileName()));
    if(!xmlSchema.isValid()) 
    {
        QMessageBox::critical(this, tr("Invalid AFDI standard"), tr("The schema is invalid"));
        return;
    }
    QXmlSchemaValidator validator(xmlSchema);
    if (!validator.validate(instanceData))
    {
        QMessageBox::critical(this, tr("Invalid Dml content"), tr("Generated Dml file is not valid against Xsd grammar."));
        return;
    }
   
    QFile file(_currentShowFilename);
    file.open(QIODevice::WriteOnly);
    file.write(dmlContent.c_str());
    file.close();
    
    _currentShow.setModified(false);
    storeData();
}

void MainWindow::onSaveAsShowRequested()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save show as"), QString(), tr("Show file (*.dml)"));
    if (filename.isEmpty())
        return;
    
    if (!filename.endsWith(".dml"))
        filename += ".dml";
    
    _currentShowFilename = filename;
    
    onSaveShowRequested();
}

void MainWindow::import(const QString& path)
{
    if (!userConfirmClosingCurrentShow())
        return;
    
    // Getting the cached location of the previous GetOpenFileName
    QSettings settings;
    // Cache the location for the next GetOpenFileName.
    settings.setValue(CACHED_LOCATION_KEY, path);

    if (_scan)
    {
        _scan->deleteLater();
        _scan = 0;
    }

    closeShow();

    QFile file (_configFilename);
    file.open(QIODevice::ReadOnly);
    QByteArray xml = file.readAll();
    file.close();

    _currentShow.setRootPath(path.toStdString());
    _ui.rootPathValue->setText(path);

    QDomDocument doc;
    doc.setContent(xml);

    QDomElement root = doc.firstChildElement("dmlCreator");

    dml::conf::ConfigurationManager::instance()->loadConfigurationData(root);
    _scan = new dml::scan::MainScan(_currentShow, dml::conf::ConfigurationManager::instance()->configuration());

    connect(&_currentShow, &dml::scan::ScanShow::notifyReload, this, &MainWindow::onNotifyReloadShow);
    connect(_scan, &dml::scan::MainScan::notifyProgress, this, &MainWindow::onNotifyProgress);
    connect(_scan, &dml::scan::MainScan::notifyFinished, this, &MainWindow::onNotifyFinished);

    _scanDialog.show();
    _scan->start(path);
}

void MainWindow::onImportShowRequested()
{
    if (!userConfirmClosingCurrentShow())
        return;

    // Getting the cached location of the previous GetOpenFileName
    QSettings settings;
    QString cachedLocation = settings.value(CACHED_LOCATION_KEY).toString();

    QString path = QFileDialog::getExistingDirectory(this, tr("Import Show from directory"), cachedLocation, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
        return;

    import(path);
}

void MainWindow::onQuitRequested()
{
    if (!userConfirmClosingCurrentShow())
        return;
    
    storeData();
    
    QApplication::quit();
}

void MainWindow::onEditConfigFileRequested()
{
    QString path = "file:///" + _configFilename;
    QDesktopServices::openUrl(QUrl(path));
}

void MainWindow::onSelectConfigFileRequested()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Default config file"), _configFilename, tr("Config file (*.xml)"));
    if (filename.isEmpty())
        return;

    _configFilename = filename;

    storeData();
}

void MainWindow::onSelectAFDISchemaFileRequested()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Default AFDI standard file"), _schemaFilename, tr("AFDI Standard file (*.xsd)"));
    if (filename.isEmpty())
        return;

    _schemaFilename = filename;

    storeData();
}

void MainWindow::onHelpContentRequested()
{
    QString help = "file:///" + QApplication::applicationDirPath() + "/doc/index.html";
    QDesktopServices::openUrl(QUrl(help));
}

void MainWindow::onAboutRequested()
{
    QString about = QString("<b>%1</b><br />").arg(qApp->applicationName());
    about += tr("Version %1").arg(qApp->applicationVersion()) + QString("<br />");
    about += tr("Distributed under LGPL v3.");

    QMessageBox::about(this, tr("About %1").arg(qApp->applicationName()), about);
}

void MainWindow::onAboutQtRequested()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::onInformationChanged(const dml::core::Information& information)
{
    _currentShow.setInformation(information);
}

void MainWindow::onVideoListChanged(const dml::core::VideoList& videoList)
{
    _currentShow.setVideoList(videoList);
}

void MainWindow::onAudioListChanged(const dml::core::AudioList& audioList)
{
    _currentShow.setAudioList(audioList);
}

void MainWindow::onSubtitlesListChanged(const dml::core::SubtitlesList& subtitlesList)
{
    _currentShow.setSubtitlesList(subtitlesList);
}

void MainWindow::onNotifyProgress(dml::scan::MainScan::ScanStep step)
{
    _scanDialog.setStep(step);
}

void MainWindow::onNotifyReloadShow()
{
    reloadShow();
}

void MainWindow::onNotifyFinished()
{
    _scanDialog.hide();
}

void MainWindow::onAbortScan()
{
    if (!_scan)
        return;

    _scanDialog.hide();
    _scan->stop();
}

bool MainWindow::userConfirmClosingCurrentShow()
{
    if (!_currentShow.modified())
        return true;
    
    QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Show modified"), tr("Show has been modified. Are you sure?"));
    if (answer == QMessageBox::No)
        return false;
    
    return true;
}

void MainWindow::loadStoredData()
{
    QSettings settings;
    
    QStringList producerList = settings.value("producer-list").toStringList();
    _ui.showProperties->setProducerList(producerList);
    
    QStringList copyrightList = settings.value("copyright-list").toStringList();
    _ui.showProperties->setCopyrightList(copyrightList);

    _configFilename = settings.value("config-filename").toString();
    _schemaFilename = settings.value("standard-filename").toString();
}

void MainWindow::storeData()
{
    QSettings settings;
    
    settings.setValue("producer-list", _ui.showProperties->producerList());
    settings.setValue("copyright-list", _ui.showProperties->copyrightList());
    settings.setValue("config-filename", _configFilename);
    settings.setValue("standard-filename", _schemaFilename);
}

