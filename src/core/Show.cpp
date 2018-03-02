#include "core/Show.h"
#include <core/Tools.h>

using namespace dml::core;

Show::Show()
{
}

Show::Show(const Show& show)
    : _rootPath(show.rootPath())
    , _information(show.information())
    , _videoList(show.videoList())
    , _audioList(show.audioList())
    , _subtitlesList(show.subtitlesList())
{
}

std::string Show::rootPath() const
{
    return _rootPath;
}

void Show::setRootPath(const std::string &rootPath)
{
    _rootPath = rootPath;
}

void Show::setAudioList(const AudioList& audioList)
{
    if (audioList==_audioList) return;
    _audioList = audioList;
}

void Show::setSubtitlesList(const SubtitlesList& subtitlesList)
{
    if (subtitlesList==_subtitlesList) return;
    _subtitlesList = subtitlesList;
}

bool Show::isNull() const
{
    if (!_rootPath.empty())
        return false;
    if (!_information.isNull())
        return false;
    if (!_videoList.empty())
        return false;
    if (!_audioList.empty())
        return false;
    if (!_subtitlesList.empty())
        return false;

    return true;
}

void Show::setInformation(const Information& information)
{
    if (information==_information) return;
    _information = information;
}

void Show::setVideoList(const VideoList& videoList)
{
    if (videoList==_videoList) return;
    _videoList = videoList;
}

void Show::clear()
{
    _subtitlesList.clear();
    _videoList.clear();
    _audioList.clear();

    _rootPath        = "";

    _information.clear();
}
