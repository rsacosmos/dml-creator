#include "conf/Configuration.h"

using namespace dml::conf;


dml::core::Audio::ChannelMap s_audioSpeakerToString;
dml::core::Frames::TypeMap s_framesTypeToString;


class AudioStaticDataConstructor
{
public:
    AudioStaticDataConstructor()
    {
        if (s_audioSpeakerToString.empty())
        {
            s_audioSpeakerToString[dml::core::Audio::SpeakerFrontLeft]          = "SPEAKER_FRONT_LEFT";
            s_audioSpeakerToString[dml::core::Audio::SpeakerFrontCenter]        = "SPEAKER_FRONT_CENTER";
            s_audioSpeakerToString[dml::core::Audio::SpeakerFrontRight]         = "SPEAKER_FRONT_RIGHT";
            s_audioSpeakerToString[dml::core::Audio::SpeakerLowFrequency]       = "SPEAKER_LOW_FREQUENCY";
            s_audioSpeakerToString[dml::core::Audio::SpeakerBackLeft]           = "SPEAKER_BACK_LEFT";
            s_audioSpeakerToString[dml::core::Audio::SpeakerBackRight]          = "SPEAKER_BACK_RIGHT";
            s_audioSpeakerToString[dml::core::Audio::SpeakerFrontLeftOfCenter]  = "SPEAKER_FRONT_LEFT_OF_CENTER";
            s_audioSpeakerToString[dml::core::Audio::SpeakerFrontRightOfCenter] = "SPEAKER_FRONT_RIGHT_OF_CENTER";
            s_audioSpeakerToString[dml::core::Audio::SpeakerBackCenter]         = "SPEAKER_BACK_CENTER";
            s_audioSpeakerToString[dml::core::Audio::SpeakerSideLeft]           = "SPEAKER_SIDE_LEFT";
            s_audioSpeakerToString[dml::core::Audio::SpeakerSideRight]          = "SPEAKER_SIDE_RIGHT";
            s_audioSpeakerToString[dml::core::Audio::SpeakerLeftHigh]           = "SPEAKER_LEFT_HIGH";
            s_audioSpeakerToString[dml::core::Audio::SpeakerRightHigh]          = "SPEAKER_RIGHT_HIGH";
            s_audioSpeakerToString[dml::core::Audio::StereoMix]                 = "STEREO_MIX";
        }
    }
};

AudioStaticDataConstructor s_audioStaticDataConstructor;

class FramesStaticDataConstructor
{
public:
    FramesStaticDataConstructor()
    {
        if (s_framesTypeToString.empty())
        {
            s_framesTypeToString[dml::core::Frames::StereoscopicLeft]       = "STEREOSCOPIC_LEFT";
            s_framesTypeToString[dml::core::Frames::StereoscopicRight]      = "STEREOSCOPIC_RIGHT";
            s_framesTypeToString[dml::core::Frames::Video2d]                = "VIDEO_2D";
        }
    }
};

FramesStaticDataConstructor s_framesStaticDataConstructor;

std::string Configuration::speakerToString(dml::core::Audio::Speaker type)
{
    dml::core::Audio::ChannelMap::iterator it = s_audioSpeakerToString.find(type);
    if (it==s_audioSpeakerToString.end()) return std::string();
    return it->second;
}

dml::core::Audio::Speaker Configuration::speakerFromString(const std::string &value)
{
    // Without any advanced map type, our only solution is to parse the map linearly.
    for (dml::core::Audio::ChannelMap::const_iterator it=s_audioSpeakerToString.begin() ; it!=s_audioSpeakerToString.end() ; ++it)
    {
        if (it->second == value)
            return it->first;
    }
    return dml::core::Audio::InvalidSpeaker;
}

std::string Configuration::framesTypeToString(dml::core::Frames::Type type)
{
    dml::core::Frames::TypeMap::iterator it = s_framesTypeToString.find(type);
    if (it==s_framesTypeToString.end()) return std::string();
    return it->second;
}

dml::core::Frames::Type Configuration::framesTypeFromString(const std::string &value)
{
    // Without any advanced map type, our only solution is to parse the map linearly.
    for (dml::core::Frames::TypeMap::const_iterator it=s_framesTypeToString.begin() ; it!=s_framesTypeToString.end() ; ++it)
    {
        if (it->second == value)
            return it->first;
    }
    return dml::core::Frames::Video2d;
}