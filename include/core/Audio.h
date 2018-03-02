#ifndef __DML_CORE_AUDIO_H__
#define __DML_CORE_AUDIO_H__

#include "core/Export.h"

#include <vector>
#include <map>

namespace dml {
namespace core {

class CORE_EXPORT Audio
{
public:

    enum Speaker
    {
        InvalidSpeaker = -1,
        SpeakerFrontLeft,
        SpeakerFrontCenter,
        SpeakerFrontRight,
        SpeakerLowFrequency,
        SpeakerBackLeft,
        SpeakerBackRight,
        SpeakerFrontLeftOfCenter,
        SpeakerFrontRightOfCenter,
        SpeakerBackCenter,
        SpeakerSideLeft,
        SpeakerSideRight,
        SpeakerLeftHigh,
        SpeakerRightHigh,
        StereoMix,
        SpeakerCount
    };

    typedef std::map<Speaker, std::string> ChannelMap;

    Audio(const std::string &dml = "");
    Audio(const Audio& other);

    void setAudioName(const std::string &audioName);
    std::string audioName() const { return _audioName; }

    void setPictureFrameRate(double pictureFrameRate);
    double pictureFrameRate() const { return _pictureFrameRate; }

    void setLanguage(const std::string &language);
    std::string language() const { return _language; }

    void setChannelMap(const ChannelMap &channelMap);
    const ChannelMap& channelMap() const { return _channelMap; }
   
    // Comparison operators
    bool operator==(const Audio&) const;
    bool operator!=(const Audio& rhs) const { return !(operator==(rhs)); }

private:
    std::string _audioName;
    double _pictureFrameRate;
    std::string _language;
    ChannelMap _channelMap;
};

typedef std::vector<Audio> AudioList;
typedef std::vector<Audio>::iterator AudioListIterator;
typedef std::vector<Audio>::const_iterator AudioListConstIterator;

} // namespace core
} // namespace dml

#endif // __DML_CORE_AUDIO_H__
