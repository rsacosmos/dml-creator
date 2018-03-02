#ifndef __DML_CORE_VIDEO_H__
#define __DML_CORE_VIDEO_H__

#include "core/Export.h"

#include <deque>

#include <core/Frames.h>

namespace dml {
namespace core {


    class CORE_EXPORT Video
{
public:
    virtual ~Video();

    Video(const std::string &dml = "");

    std::string videoName() const;

    void setVideoName(const std::string &videoName);

    unsigned int framerate() const;

    void setFramerate(unsigned int framerate);

    unsigned int framecount() const;

    void setFramecount(unsigned int framecount);

    unsigned int resolution() const;

    void setResolution(unsigned int resolution);

    unsigned int colorBitsDepth() const;

    void setColorBitsDepth(unsigned int colorBitsDepth);

    std::string preview() const;

    void setPreview(const std::string &preview);

    void setFramesList(const FramesList& framesList) { _framesList = framesList; }
    const FramesList& framesList() const { return _framesList; }

    bool findFramesByType(const Frames::Type& type, Frames& frames) const;
    void addOrReplaceFrames(const Frames& frames);

    // Comparison operators
    bool operator==(const Video&) const;
    bool operator!=(const Video& rhs) const { return !(operator==(rhs)); }

private:
    void updateFrameCount() const;

    std::string _videoName;
    unsigned int _framerate;
    unsigned int _resolution;
    unsigned int _colorBitsDepth;
    std::string _preview;
    FramesList _framesList;

    mutable unsigned int    _framecount;
}; // class Video

typedef std::deque<Video> VideoList;
typedef std::deque<Video>::iterator VideoListIterator;
typedef std::deque<Video>::const_iterator VideoListConstIterator;

} // namespace core
} // namespace dml


#endif // __DML_CORE_VIDEO_H__
