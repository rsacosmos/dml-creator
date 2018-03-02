#include "SubtitlesWidget.h"
#include "ui_SubtitlesWidget.h"

#include <conf/ConfigurationManager.h>

#include <QMessageBox>
#include <QFileDialog>
#include <scan/MainScan.h>



SubtitlesWidget::SubtitlesWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , _ui(new Ui::SubtitlesWidget)
{
    _ui->setupUi(this);
        

    connect(_ui->subtitlesList, &QListWidget::currentRowChanged, this, &SubtitlesWidget::setCurrentSubtitlesIndex);
    connect(_ui->subtitlesPathButton, &QPushButton::clicked, this, &SubtitlesWidget::onPathBrowseRequested);
}

SubtitlesWidget::~SubtitlesWidget()
{
    delete _ui;
}


void SubtitlesWidget::setSubtitlesList(const dml::core::SubtitlesList& subtitlesList)
{
    if (subtitlesList==_subtitlesList) return;

    // Should we rebuild the listWidget?
    bool rebuild = false;
    if (_subtitlesList.size()!=subtitlesList.size())
    {
        rebuild = true;
    }
    else
    {
        for (unsigned int audioIndex=0; audioIndex<subtitlesList.size(); audioIndex++)
        {
            if (subtitlesList.at(audioIndex).path()!=_subtitlesList.at(audioIndex).path())
            {
                rebuild = true;
                break;
            }
        }
    }

    // Save the current row for later
    int currentRow = _ui->subtitlesList->currentRow();

    // Rebuild the listWidget if needed
    if (rebuild)
    {
        _blockSignals(true);
        _ui->subtitlesList->clear();
        for (unsigned int subtitlesIndex=0 ; subtitlesIndex<subtitlesList.size(); subtitlesIndex++)
            _ui->subtitlesList->addItem(QString::fromStdString(subtitlesList.at(subtitlesIndex).path()));
        _blockSignals(false);
    }
    else
    {
        // Check names
        for (unsigned int subtitlesIndex=0 ; subtitlesIndex<subtitlesList.size(); subtitlesIndex++)
        {
            QString path = QString::fromStdString(subtitlesList.at(subtitlesIndex).path());
            QListWidgetItem *item = _ui->subtitlesList->item(subtitlesIndex);
            Q_ASSERT(item);
            QString currentItemText = item->text();
            if (currentItemText!=path)
            {
                item->setText(path);
            }
        }
    }

    // Save the changes
    _subtitlesList = subtitlesList;

    // Update the right part of the widget now
    if (0==subtitlesList.size()) currentRow = -1;
    else if (currentRow>=subtitlesList.size()) currentRow = 0;
    _blockSignals(true);
    _ui->subtitlesList->setCurrentRow(currentRow);
    _blockSignals(false);
    setCurrentSubtitlesIndex(currentRow); // manual call, because 'setCurrentRow' will not emit 'changed' if the row is the same (in the case of only the currentVideo contents changed)

    emit subtitlesListChanged(_subtitlesList);
}

void SubtitlesWidget::setCurrentSubtitlesIndex(int currentIndex)
{
    using namespace dml::core;

    bool noSubtitles = (currentIndex < 0);
    _ui->groupBox->setDisabled(noSubtitles);
    _ui->removePushButton->setDisabled(noSubtitles);

    if (noSubtitles)
    {
        _blockSignals(true);
        _ui->pathLineEdit->clear();
        _ui->languageLineEdit->clear();
        _blockSignals(false);
        return;
    }

    const Subtitles& subtitles = _subtitlesList.at(currentIndex);

    _blockSignals(true);

    // Subtitles language
    QString language = QString::fromStdString(subtitles.language());
    if (_ui->languageLineEdit->text() != language) _ui->languageLineEdit->setText(language);

    // Subtitles language
    QString path = QString::fromStdString(subtitles.path());
    if (_ui->pathLineEdit->text() != language) _ui->pathLineEdit->setText(path);

    _blockSignals(false);    
}


void SubtitlesWidget::_blockSignals(bool blocked)
{
    _ui->pathLineEdit->blockSignals(blocked);
    _ui->languageLineEdit->blockSignals(blocked);
}

void SubtitlesWidget::refreshSubtitles()
{
    using namespace dml::core;

    int currentIndex = _ui->subtitlesList->currentRow();
    if (-1==currentIndex) return;

    Subtitles subtitles;
    subtitles.setPath(_ui->pathLineEdit->text().toStdString());
    subtitles.setLanguage(_ui->languageLineEdit->text().toStdString());

    SubtitlesList subtitlesList(_subtitlesList);
    if (subtitles!=subtitlesList.at(currentIndex))
    {
        subtitlesList.at(currentIndex) = subtitles;
        setSubtitlesList(subtitlesList);
    }
}

void SubtitlesWidget::onPathBrowseRequested()
{
    int currentIndex = _ui->subtitlesList->currentRow();
    if (currentIndex < 0)
    {
        return;
    }
    dml::core::Subtitles subtitles = _subtitlesList.at(currentIndex);

    QString projectPath;
    if(_currentShow != NULL)
        projectPath = QString(_currentShow->rootPath().c_str());

    dml::conf::Configuration configuration = dml::conf::ConfigurationManager::instance()->configuration();
    QString filename = QFileDialog::getOpenFileName(0, tr("Choose subtitles file"), dml::scan::MainScan::toAbsolutePath(projectPath, QString(subtitles.path().c_str())), configuration.subtitlesExtensionSupported());
    if (filename.isEmpty())
        return;

    QString relativePath = dml::scan::MainScan::toLocalPath(projectPath, filename);
    QString language = dml::scan::MainScan::getLanguage(configuration, relativePath);
    if(language.isEmpty())
    {
        language = configuration.defaultSubtitlesLanguage().toLower();
    }
    // Update the QLineEdit and ask for a refresh (let refreshVideo do the job ^^)
    blockSignals(true);
    _ui->pathLineEdit->setText(relativePath);
    _ui->languageLineEdit->setText(language);
    blockSignals(false);
    refreshSubtitles(); // manual call to refreshSubtitles()
}

void SubtitlesWidget::on_addPushButton_clicked()
{
    dml::core::Subtitles subtitles;
    dml::conf::Configuration configuration = dml::conf::ConfigurationManager::instance()->configuration();

    subtitles.setPath(tr("Unknown subtitles").toStdString());
    subtitles.setLanguage(configuration.defaultSubtitlesLanguage().toStdString());

    dml::core::SubtitlesList subtitlesList(_subtitlesList);
    subtitlesList.push_back(subtitles);
    setSubtitlesList(subtitlesList);
}

void SubtitlesWidget::on_removePushButton_clicked()
{
    QList<QListWidgetItem *> selectedItems = _ui->subtitlesList->selectedItems();

    if (QMessageBox::No == QMessageBox::question(this, tr("Subtitles deletion"), tr("Subtitles will be deleted. Are you sure?")))
    {
        return;
    }

    QList<int> indexes;
    dml::core::SubtitlesList subtitlesList(_subtitlesList);
    foreach(QListWidgetItem* item, selectedItems)
    {
        indexes <<_ui->subtitlesList->row(item);
    }
    qSort(indexes);

    int offset = 0;
    foreach(const int& index, indexes)
    {
        subtitlesList.erase(subtitlesList.begin()+index - offset);
        ++offset;
    }
    setSubtitlesList(subtitlesList);
}
