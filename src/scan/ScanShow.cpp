#include "scan/ScanShow.h"

using namespace dml::scan;

ScanShow::ScanShow(QObject* parent)
    : QObject(parent)
    , dml::core::Show()
    , _modified(false)
{
}

ScanShow::ScanShow(const dml::core::Show& show, QObject* parent)
    : QObject(parent)
    , dml::core::Show(show)
    , _modified(false)
{
}

ScanShow::ScanShow(const dml::scan::ScanShow& show, QObject* parent)
    : QObject(parent)
    , dml::core::Show(show)
    , _modified(show.modified())
{
}

ScanShow& ScanShow::operator=(const ScanShow& scanShow)
{
    if(this != &scanShow)
    {
        (dml::core::Show)(*this) = dml::core::Show::operator=(scanShow);
    }
    return *this;
}

void ScanShow::refreshGUI()
{
    emit notifyReload();
}

void ScanShow::addSubtitles(const dml::core::Subtitles &subtitles)
{
    QMutexLocker mutexLocker(&_locker);

    dml::core::SubtitlesList subtitlesList = this->subtitlesList();
    subtitlesList.push_back(subtitles);
    setSubtitlesList(subtitlesList);
    _modified = true;
}

void ScanShow::addAudio(const dml::core::Audio& audio)
{
    QMutexLocker mutexLocker(&_locker);

    dml::core::AudioList audioList = this->audioList();
    audioList.push_back(audio);
    setAudioList(audioList);
    _modified = true;
}

void ScanShow::addVideo(const dml::core::Video& video)
{
    QMutexLocker mutexLocker(&_locker);

    dml::core::VideoList videoList = this->videoList();
    videoList.push_back(video);
    setVideoList(videoList);
    _modified = true;
}
