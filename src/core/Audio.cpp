#include "core/Audio.h"
#include <core/Tools.h>

using namespace dml::core;

Audio::Audio(const std::string &dml)
  : _pictureFrameRate(0.0)
{
}

Audio::Audio(const Audio& other)
    : _audioName(other.audioName())
    , _channelMap(other.channelMap())
    , _language(other.language())
    , _pictureFrameRate(other.pictureFrameRate())
{
}


void Audio::setAudioName(const std::string &audioName)
{
    _audioName = audioName;
}

void Audio::setPictureFrameRate(double pictureFrameRate)
{
    _pictureFrameRate = pictureFrameRate;
}

void Audio::setLanguage(const std::string &language)
{
    _language = language;
}

void Audio::setChannelMap(const ChannelMap &channelMap)
{
    _channelMap = channelMap;
}

bool Audio::operator==(const Audio& rhs) const
{
    if (rhs._audioName != _audioName) return false;
    if (rhs._pictureFrameRate != _pictureFrameRate) return false;
    if (rhs._language != _language) return false;
    if (rhs._channelMap != _channelMap) return false; // <-- TODO Check this comparision test for std::map
    return true;
}
