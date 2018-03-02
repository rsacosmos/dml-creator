#ifndef __DML_CREATOR_VIDEOWIDGET_H__
#define __DML_CREATOR_VIDEOWIDGET_H__

#include <QWidget>
#include <core/Video.h>
#include "scan/ScanShow.h"

namespace Ui { class VideoWidget;
               class FramesWidget; }

class VideoWidget: public QWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~VideoWidget();
    
    void setVideoList(const dml::core::VideoList&);
    void setCurrentShow(dml::scan::ScanShow* currentShow) { _currentShow = currentShow; }

signals:
    void videoListChanged(const dml::core::VideoList&);

private slots:
    void on_previewPushButton_clicked();
    void on_addPushButton_clicked();
    void on_removePushButton_clicked();
    void on_addFramesPushButton_clicked();
    void on_removeFramesPushButton_clicked();

    void setCurrentVideoIndex(int currentVideoIndex);
    void onFramesListChanged(const dml::core::FramesList& framesList);
    void refreshVideo();

private:
    void _blockSignals(bool blocked); // there is an underscore in the name because the official 'QObject::blockSignals()' isn't virtual and should not be inherited.

    Ui::VideoWidget *_ui;
    dml::core::VideoList _videoList;
    dml::scan::ScanShow* _currentShow;
};

#endif // __DML_CREATOR_VIDEOWIDGET_H__
