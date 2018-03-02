#include "FramesWidget.h"
#include "ui_FramesWidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSignalMapper>

#include <conf/ConfigurationManager.h>
#include <scan/VideoScan.h>
#include <scan/MainScan.h>


FramesWidget::FramesWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , _ui(new Ui::FramesWidget)
    , _currentShow(NULL)
    , _frameSequenceSignalMapper(new QSignalMapper(this))
{
    _ui->setupUi(this);

    QHeaderView *headerView = new QHeaderView(Qt::Horizontal, _ui->frameSequenceListTableWidget);
    _ui->frameSequenceListTableWidget->setHorizontalHeader(headerView);
    headerView->setSectionResizeMode(0, QHeaderView::Stretch);
    headerView->resizeSection(1, 25);
    headerView->setSectionResizeMode(2, QHeaderView::Fixed);
    headerView->resizeSection(2, 45);
    headerView->setSectionResizeMode(3, QHeaderView::Fixed);
    headerView->resizeSection(3, 45);
}

FramesWidget::~FramesWidget()
{
    delete _ui;
    delete _frameSequenceSignalMapper;
}

void FramesWidget::setFrames(const dml::core::FramesList& framesList, const int& currentFramesIndex)
{
    if (_framesList==framesList) return;
    _blockSignals(true);

    dml::core::Frames frames = framesList.at(currentFramesIndex);
    dml::core::FrameSequenceList frameSequenceList = frames.frameSequences();

    bool noFrameSequence = (frameSequenceList.size() == 0);
    _ui->removePushButton->setDisabled(noFrameSequence);

    _ui->typeComboBox->setCurrentIndex(frames.type());
    _frameSequenceSignalMapper->deleteLater();
    _frameSequenceSignalMapper = new QSignalMapper;
    connect(_frameSequenceSignalMapper, SIGNAL(mapped(int)), this, SLOT(onFrameSequencePathBrowseButtonClicked(int)));
    _ui->frameSequenceListTableWidget->setRowCount((int)frameSequenceList.size());
    for (int index=0; index<frameSequenceList.size(); index++)
    {
        dml::core::FrameSequence frameSequence = frameSequenceList.at(index);

        QTableWidgetItem *itemPath = new QTableWidgetItem(QString(frameSequence.framespath().c_str()));
        QPushButton *button = new QPushButton;
        button->setText("...");
        button->setFixedWidth(25);
        connect(button, SIGNAL(clicked()), _frameSequenceSignalMapper, SLOT(map()));
        _frameSequenceSignalMapper->setMapping(button, index);

        QTableWidgetItem *itemFirst = new QTableWidgetItem(QString::number(frameSequence.firstframe()));
        QTableWidgetItem *itemLast = new QTableWidgetItem(QString::number(frameSequence.lastframe()));


        _ui->frameSequenceListTableWidget->setItem(index, 0, itemPath);
        _ui->frameSequenceListTableWidget->setCellWidget(index, 1, button);
        _ui->frameSequenceListTableWidget->setItem(index, 2, itemFirst);
        _ui->frameSequenceListTableWidget->setItem(index, 3, itemLast);
    }
    _framesList = framesList;
    _currentFramesIndex = currentFramesIndex;
    _blockSignals(false);
}

void FramesWidget::onFrameSequencePathBrowseButtonClicked(int frameSequenceIndex)
{
    bool noFramesSequence = (frameSequenceIndex < 0);

    _ui->removePushButton->setEnabled(noFramesSequence);

    if (noFramesSequence)
    {
        _blockSignals(true);
        _ui->frameSequenceListTableWidget->setRowCount(0);
        _blockSignals(false);
        return;
    }

    dml::core::Frames frames = _framesList.at(_currentFramesIndex);
    dml::core::FrameSequence frameSequence = frames.frameSequences().at(frameSequenceIndex);

    QString projectPath;
    if(_currentShow != NULL)
        projectPath = QString(_currentShow->rootPath().c_str());
    
    dml::conf::Configuration configuration = dml::conf::ConfigurationManager::instance()->configuration();
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose frames path"), dml::scan::MainScan::toAbsolutePath(projectPath, QString(frameSequence.framespath().c_str())), configuration.videoImageExtensionSupported());
    if (filename.isEmpty())
        return;

    blockSignals(true);
    
    //Guess sequence from/to
    QString path;
    unsigned int firstFrameIndex;
    unsigned int lastFrameIndex;
    dml::scan::VideoScan::guessSequenceFromTo(configuration, projectPath, filename, path, firstFrameIndex, lastFrameIndex);

    QTableWidgetItem *itemPath = _ui->frameSequenceListTableWidget->item(frameSequenceIndex, 0);
    itemPath->setData(Qt::DisplayRole, path);

    QTableWidgetItem *itemFirst = _ui->frameSequenceListTableWidget->item(frameSequenceIndex, 2);
    itemFirst->setData(Qt::DisplayRole, QString::number(firstFrameIndex));

    QTableWidgetItem *itemLast = _ui->frameSequenceListTableWidget->item(frameSequenceIndex, 3);
    itemLast->setData(Qt::DisplayRole, QString::number(lastFrameIndex));    

    blockSignals(false);
    refreshFrames(); // manual call to refreshFrames()
}

void FramesWidget::_blockSignals(bool blocked)
{
    _ui->typeComboBox->blockSignals(blocked);
    _ui->frameSequenceListTableWidget->blockSignals(blocked);
    _ui->addPushButton->blockSignals(blocked);
    _ui->removePushButton->blockSignals(blocked);
}

void FramesWidget::refreshFrames()
{
    dml::core::Frames frames;
    frames.setType((dml::core::Frames::Type)_ui->typeComboBox->currentIndex());

    dml::core::FrameSequenceList frameSequenceList;
    for( int i = 0; i < _ui->frameSequenceListTableWidget->rowCount(); ++i)
    {
        dml::core::FrameSequence frameSequence;

        QTableWidgetItem *itemPath = _ui->frameSequenceListTableWidget->item(i, 0);
        QTableWidgetItem *itemFirst = _ui->frameSequenceListTableWidget->item(i, 2);
        QTableWidgetItem *itemLast = _ui->frameSequenceListTableWidget->item(i, 3);

        frameSequence.setFramespath(itemPath->text().toStdString());
        frameSequence.setFirstframe(itemFirst->text().toInt());
        frameSequence.setLastframe(itemLast->text().toInt());
        frameSequenceList.push_back(frameSequence);
    }
    frames.setFrameSequences(frameSequenceList);

    dml::core::FramesList framesList(_framesList);
    framesList.at(_currentFramesIndex) = frames;
    _framesList = framesList;
    emit framesListChanged(framesList);
}

void FramesWidget::on_addPushButton_clicked()
{
    dml::core::FramesList framesList(_framesList);
    dml::core::Frames frames(framesList.at(_currentFramesIndex));
    dml::core::FrameSequence frameSequence;
    frameSequence.setFramespath(tr("Unnamed path").toStdString());
    frameSequence.setFirstframe(0);
    frameSequence.setLastframe(0);

    dml::core::FrameSequenceList frameSequenceList(frames.frameSequences());
    frameSequenceList.push_back(frameSequence);
    frames.setFrameSequences(frameSequenceList);
    framesList.at(_currentFramesIndex) = frames;
    setFrames(framesList, _currentFramesIndex);
    emit framesListChanged(framesList);
    // TODO manage the index where to insert
}

void FramesWidget::on_removePushButton_clicked()
{
    int currentFrameSequenceIndex = _ui->frameSequenceListTableWidget->currentRow();

    if (currentFrameSequenceIndex < 0)
    {
        return;
    }
    
    if (QMessageBox::No == QMessageBox::question(this, tr("Frame Sequence deletion"), tr("Frame Sequence will be deleted. Are you sure?")))
    {
        return;
    }
    dml::core::FramesList framesList(_framesList);
    dml::core::Frames frames(framesList.at(_currentFramesIndex));
    dml::core::FrameSequenceList frameSequenceList(frames.frameSequences());
    frameSequenceList.erase(frameSequenceList.begin()+currentFrameSequenceIndex);
    frames.setFrameSequences(frameSequenceList);
    framesList.at(_currentFramesIndex) = frames;
    setFrames(framesList, _currentFramesIndex);
    emit framesListChanged(framesList);
}
