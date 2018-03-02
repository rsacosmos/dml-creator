/*
    This file is part of DML Creator SDK.

    DML Creator SDK is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DML Creator SDK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with DML Creator SDK.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __DML_CREATOR_AUDIOWIDGET_H__
#define __DML_CREATOR_AUDIOWIDGET_H__

#include <QWidget>
#include <core/Audio.h>
#include <scan/ScanShow.h>

class QSignalMapper;
namespace Ui { class AudioWidget; }

class AudioWidget : public QWidget
{
    Q_OBJECT

public:
    AudioWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~AudioWidget();
    
    void setAudioList(const dml::core::AudioList&);
    void setCurrentShow(dml::scan::ScanShow* currentShow) { _currentShow = currentShow; }

signals:
    void audioListChanged(const dml::core::AudioList&);

private slots:
    void on_addPushButton_clicked();
    void on_removePushButton_clicked();

    void onSpeakerBrowseButtonClicked(int i);

    void setCurrentAudioIndex(int currentAudioIndex);
    void refreshAudio();

private:
    void _blockSignals(bool blocked); // there is an underscore in the name because the official 'QObject::blockSignals()' isn't virtual and should not be inherited.

    Ui::AudioWidget *_ui;
    dml::core::AudioList _audioList;
    dml::scan::ScanShow* _currentShow;
    QSignalMapper *_speakerSignalMapper;
};

#endif // __DML_CREATOR_AUDIOWIDGET_H__
