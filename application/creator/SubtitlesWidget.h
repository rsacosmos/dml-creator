#ifndef __DML_CREATOR_SUBTITLESWIDGET_H__
#define __DML_CREATOR_SUBTITLESWIDGET_H__

#include <QWidget>
#include <core/Subtitles.h>
#include "scan/ScanShow.h"

namespace Ui { class SubtitlesWidget; }

class SubtitlesWidget: public QWidget
{
    Q_OBJECT
public:
    SubtitlesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~SubtitlesWidget();    

    void setSubtitlesList(const dml::core::SubtitlesList&);
    void setCurrentShow(dml::scan::ScanShow* currentShow) { _currentShow = currentShow; }

signals:
    void subtitlesListChanged(const dml::core::SubtitlesList&);

private slots:
    void on_addPushButton_clicked();
    void on_removePushButton_clicked();

    void setCurrentSubtitlesIndex(int currentIndex);

    void refreshSubtitles();

    void onPathBrowseRequested();

private:
    void connectPropertySignals();
    void disconnectPropertySignals();

private:
    void _blockSignals(bool blocked); // there is an underscore in the name because the official 'QObject::blockSignals()' isn't virtual and should not be inherited.

    // Main window reference.
    Ui::SubtitlesWidget *_ui;

    dml::core::SubtitlesList _subtitlesList;
    dml::scan::ScanShow* _currentShow;
};

#endif
