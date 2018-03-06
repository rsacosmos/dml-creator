#ifndef __DML_CORE_SHOW_H__
#define __DML_CORE_SHOW_H__

#include "Export.h"

#include "Subtitles.h"
#include "Audio.h"
#include "Video.h"
#include "Information.h"

namespace dml {
namespace core {

class CORE_EXPORT Show
{
public:
    Show();
    Show(const Show& show);
    
    void clear();

    std::string rootPath() const; // TODO Remove this : the specification doesn't talk about any rootPath
    void setRootPath(const std::string &rootPath); // TODO Remove this

    void setInformation(const Information& information);
    const Information& information() const { return _information; }

    void setVideoList(const VideoList& videoList);
    const VideoList& videoList() const { return _videoList; }

    void setAudioList(const AudioList& audioList);
    const AudioList& audioList() const { return _audioList; }

    void setSubtitlesList(const SubtitlesList& subtitlesList);
    const SubtitlesList& subtitlesList() const { return _subtitlesList; }

    bool isNull() const;

private:
    std::string _rootPath; // TODO Remove this
    Information _information;
    VideoList _videoList;
    AudioList _audioList;
    SubtitlesList _subtitlesList;
}; // class Show

} // namespace core
} // namespace dml

#endif // __DML_CORE_SHOW_H__
