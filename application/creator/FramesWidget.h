#ifndef __DML_CREATOR_FRAMESWIDGET_H__
#define __DML_CREATOR_FRAMESWIDGET_H__

#include <QWidget>
#include <core/Frames.h>
#include <scan/ScanShow.h>

class QSignalMapper;
namespace Ui { class FramesWidget; }

class FramesWidget: public QWidget
{
    Q_OBJECT

public:
    FramesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~FramesWidget();
    
    void setFrames(const dml::core::FramesList&, const int& currentFramesIndex);
    void setCurrentShow(dml::scan::ScanShow* currentShow) { _currentShow = currentShow; }
    dml::core::Frames frames() const { return _framesList.at(_currentFramesIndex); }

signals:
    void framesListChanged(const dml::core::FramesList&);

private slots:
    void onFrameSequencePathBrowseButtonClicked(int frameSequenceIndex);
    void on_addPushButton_clicked();
    void on_removePushButton_clicked();

    void refreshFrames();

private:
    void _blockSignals(bool blocked); // there is an underscore in the name because the official 'QObject::blockSignals()' isn't virtual and should not be inherited.

    Ui::FramesWidget *_ui;
    dml::core::FramesList _framesList;
    int _currentFramesIndex;
    dml::scan::ScanShow* _currentShow;
    QSignalMapper *_frameSequenceSignalMapper;
};

#endif // __DML_CREATOR_FRAMESWIDGET_H__
