#include "VideoWidget.h"
#include "FramesWidget.h"
#include <QFileDialog>
#include <QMessageBox>

#include "ui_VideoWidget.h"
#include "ui_FramesWidget.h"

#include <scan/MainScan.h>
#include <conf/ConfigurationManager.h>


VideoWidget::VideoWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , _ui(new Ui::VideoWidget)
    , _currentShow(NULL)
{
    _ui->setupUi(this);

    connect(_ui->listWidget, &QListWidget::currentRowChanged, this, &VideoWidget::setCurrentVideoIndex);
}

VideoWidget::~VideoWidget()
{
    delete _ui;
}

void VideoWidget::setVideoList(const dml::core::VideoList& videoList)
{
    if (videoList==_videoList) return;

    // Should we rebuild the listWidget?
    bool rebuild = false;
    if (_videoList.size()!=videoList.size())
    {
        rebuild = true;
    }
    else
    {
        for (unsigned int videoIndex=0; videoIndex<videoList.size(); videoIndex++)
        {
            if (videoList.at(videoIndex).videoName()!=_videoList.at(videoIndex).videoName())
            {
                rebuild = true;
                break;
            }
        }
    }

    // Save the current row for later
    int currentRow = _ui->listWidget->currentRow();

    // Rebuild the listWidget if needed
    if (rebuild)
    {
        _blockSignals(true);
        _ui->listWidget->clear();
        for (unsigned int videoIndex=0 ; videoIndex<videoList.size(); videoIndex++)
            _ui->listWidget->addItem(QString::fromStdString(videoList.at(videoIndex).videoName()));
        _blockSignals(false);
    }
    else
    {
        // Check names
        for (unsigned int videoIndex=0 ; videoIndex<videoList.size(); videoIndex++)
        {
            QString videoName = QString::fromStdString(videoList.at(videoIndex).videoName());
            QListWidgetItem *item = _ui->listWidget->item(videoIndex);
            Q_ASSERT(item);
            QString currentItemText = item->text();
            if (currentItemText!=videoName)
            {
                item->setText(videoName);
            }
        }
    }

    // Save the changes
    _videoList = videoList;

    // Update the right part of the widget now
    if (0==videoList.size()) currentRow = -1;
    else if (currentRow>=videoList.size()) currentRow = 0;
    _blockSignals(true);
    _ui->listWidget->setCurrentRow(currentRow);
    _blockSignals(false);
    setCurrentVideoIndex(currentRow); // manual call, because 'setCurrentRow' will not emit 'changed' if the row is the same (in the case of only the currentVideo contents changed)

    emit videoListChanged(_videoList);
}

void VideoWidget::setCurrentVideoIndex(int currentIndex)
{
    bool noVideo = (currentIndex < 0);
    _ui->groupBox->setDisabled(noVideo);
    _ui->removePushButton->setDisabled(noVideo);
    if (noVideo)
    {
        _blockSignals(true);
        _ui->nameLineEdit->clear();
        _ui->framerateDoubleSpinBox->setValue(0);
        _ui->resolutionSpinBox->setValue(0);
        _ui->colorBitsDepthSpinBox->setValue(0);
        _ui->previewLineEdit->clear();
        _ui->framesListlistWidget->clear();
        _blockSignals(false);
        return;
    }
    
    const dml::core::Video& video = _videoList.at(currentIndex);
    
    _blockSignals(true);

    // videoName
    QString videoName = QString::fromStdString(video.videoName());
    if (_ui->nameLineEdit->text() != videoName) _ui->nameLineEdit->setText(videoName);

    // Frame rate
    double framerate = video.framerate();
    if (_ui->framerateDoubleSpinBox->value() != framerate) _ui->framerateDoubleSpinBox->setValue(framerate);

    // Resolution
    int resolution = static_cast<int>(video.resolution());
    if (_ui->resolutionSpinBox->value() != resolution) _ui->resolutionSpinBox->setValue(resolution);

    // Color bits depth
    int colorBitsDepth = static_cast<int>(video.colorBitsDepth());
    if (_ui->colorBitsDepthSpinBox->value() != colorBitsDepth) _ui->colorBitsDepthSpinBox->setValue(colorBitsDepth);

    // Preview
    QString preview = QString::fromStdString(video.preview());
    _ui->previewLineEdit->setText(preview);

    dml::core::FramesList framesList = video.framesList();

    _ui->framesListlistWidget->clear();
    for (int index=0; index<framesList.size(); ++index)
    {
        FramesWidget* frameWidget = new FramesWidget(_ui->framesListlistWidget);
        connect(frameWidget, &FramesWidget::framesListChanged, this, &VideoWidget::onFramesListChanged);
        frameWidget->setFrames(framesList, index);
        frameWidget->setCurrentShow(_currentShow);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(0,200));
        _ui->framesListlistWidget->addItem(item);
        _ui->framesListlistWidget->setItemWidget(item, frameWidget);        
    }
    _blockSignals(false);
}

void VideoWidget::_blockSignals(bool blocked)
{
    _ui->listWidget->blockSignals(blocked);
    _ui->nameLineEdit->blockSignals(blocked);
    _ui->previewLineEdit->blockSignals(blocked);
    _ui->framerateDoubleSpinBox->blockSignals(blocked);
    _ui->resolutionSpinBox->blockSignals(blocked);
    _ui->colorBitsDepthSpinBox->blockSignals(blocked);
    _ui->previewPushButton->blockSignals(blocked);
}

void VideoWidget::refreshVideo()
{
    int currentVideoIndex = _ui->listWidget->currentRow();
    if (-1==currentVideoIndex) return;

    dml::core::Video video;
    video.setVideoName(_ui->nameLineEdit->text().toStdString());
    video.setFramerate(_ui->framerateDoubleSpinBox->value());
    video.setResolution(_ui->resolutionSpinBox->value());
    video.setColorBitsDepth(_ui->colorBitsDepthSpinBox->value());
    video.setPreview(_ui->previewLineEdit->text().toStdString());
    
    dml::core::FramesList framesList;
    int framecount = 0;
    for(int i = 0; i < _ui->framesListlistWidget->count(); ++i)
    {
        QListWidgetItem* item = _ui->framesListlistWidget->item(i);
        FramesWidget* framesWidget = (FramesWidget*)_ui->framesListlistWidget->itemWidget(item);
        dml::core::Frames frames = framesWidget->frames();
        framesList.push_back(frames);
        foreach(const dml::core::FrameSequence frameSequence, frames.frameSequences())
        {
            framecount += frameSequence.lastframe() - frameSequence.lastframe() + 1;
        }
    }
    video.setFramecount(framecount);
    video.setFramesList(framesList);

    dml::core::VideoList videoList(_videoList);
    if (video!=videoList.at(currentVideoIndex))
    {
        videoList.at(currentVideoIndex) = video;
        setVideoList(videoList);
    }
}

void VideoWidget::on_addPushButton_clicked()
{
    dml::core::Video video;
    video.setVideoName(tr("Unnamed video").toStdString());

    dml::core::VideoList videoList(_videoList);
    videoList.push_back(video);
    setVideoList(videoList);
    // TODO manage the index where to insert
}

void VideoWidget::on_removePushButton_clicked()
{
    QList<QListWidgetItem *> selectedItems = _ui->listWidget->selectedItems();
    
    if (QMessageBox::No == QMessageBox::question(this, 
                                                 tr("Video deletion"),
                                                 tr("Video will be deleted. Are you sure?")))
    {
        return;
    }

    QList<int> indexes;
    dml::core::VideoList videoList(_videoList);
    foreach(QListWidgetItem* item, selectedItems)
    {
        indexes <<_ui->listWidget->row(item);
    }
    qSort(indexes);

    int offset = 0;
    foreach(const int& index, indexes)
    {
        videoList.erase(videoList.begin()+index - offset);
        ++offset;
    }
    setVideoList(videoList);
}

void VideoWidget::on_previewPushButton_clicked()
{
    int currentVideoIndex = _ui->listWidget->currentRow();
    if (currentVideoIndex < 0)
    {
        return;
    }
    dml::core::Video video = _videoList.at(currentVideoIndex);

    QString projectPath;
    if(_currentShow != NULL)
        projectPath = QString(_currentShow->rootPath().c_str());

    dml::conf::Configuration configuration = dml::conf::ConfigurationManager::instance()->configuration();
    QString filename = QFileDialog::getOpenFileName(0, tr("Choose preview file"), dml::scan::MainScan::toAbsolutePath(projectPath, QString(video.preview().c_str())), configuration.trailerExtensionSupported());
    if (filename.isEmpty())
        return;

    // Update the QLineEdit and ask for a refresh (let refreshVideo do the job ^^)
    blockSignals(true);
    _ui->previewLineEdit->setText(dml::scan::MainScan::toLocalPath(projectPath, filename));
    blockSignals(false);
    refreshVideo(); // manual call to refreshVideo()
}

void VideoWidget::on_addFramesPushButton_clicked()
{
    int currentVideoIndex = _ui->listWidget->currentRow();

    if (currentVideoIndex < 0)
    {
        return;
    }
    dml::core::VideoList videoList(_videoList);
    dml::core::Video video = videoList.at(currentVideoIndex);
    dml::core::FramesList framesList = video.framesList();
    dml::core::Frames frames;
    framesList.push_back(frames);

    FramesWidget* frameWidget = new FramesWidget(_ui->framesListlistWidget);
    connect(frameWidget, &FramesWidget::framesListChanged, this, &VideoWidget::onFramesListChanged);
    frameWidget->setFrames(framesList, _ui->framesListlistWidget->count());
    frameWidget->setCurrentShow(_currentShow);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(0,200));
    _ui->framesListlistWidget->addItem(item);
    _ui->framesListlistWidget->setItemWidget(item, frameWidget);        
    refreshVideo();
}

void VideoWidget::on_removeFramesPushButton_clicked()
{
    int currentVideoIndex = _ui->listWidget->currentRow();

    if (currentVideoIndex < 0)
    {
        return;
    }

    int currentFramesIndex = _ui->framesListlistWidget->currentRow();

    if (currentFramesIndex < 0)
    {
        return;
    }

    if (QMessageBox::No == QMessageBox::question(this, 
        tr("Frames deletion"),
        tr("Frames will be deleted. Are you sure?")))
    {
        return;
    }

    dml::core::VideoList videoList(_videoList);
    dml::core::Video video = videoList.at(currentVideoIndex);
    dml::core::FramesList framesList = video.framesList();

    QListWidgetItem* item = _ui->framesListlistWidget->takeItem(currentFramesIndex);
    _ui->framesListlistWidget->removeItemWidget(item);

    refreshVideo();
}

void VideoWidget::onFramesListChanged(const dml::core::FramesList& framesList)
{
    int currentVideoIndex = _ui->listWidget->currentRow();

    if (currentVideoIndex < 0)
    {
        return;
    }
    dml::core::VideoList videoList(_videoList);
    dml::core::Video video = _videoList.at(currentVideoIndex);
    int framecount = 0;
    foreach(const dml::core::Frames frames, framesList)
    {
        foreach(const dml::core::FrameSequence frameSequence, frames.frameSequences())
        {
            framecount += frameSequence.lastframe() - frameSequence.lastframe() + 1;
        }
    }
    video.setFramecount(framecount);
    video.setFramesList(framesList);
    videoList.at(currentVideoIndex) = video;
    setVideoList(videoList);
}