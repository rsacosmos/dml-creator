#ifndef __DML_CORE_VIDEO_H__
#define __DML_CORE_VIDEO_H__

#include "Export.h"

#include <deque>

#include <core/Frames.h>

namespace dml {
namespace core {

class CORE_EXPORT Video
{
public:
    virtual ~Video();

    Video(const std::string &dml = "");

    //videoName
    std::string videoName() const;
    void setVideoName(const std::string &videoName);

    //framerate
    double framerate() const;
    void setFramerate(double framerate);

    //framecount
    unsigned int framecount() const;
    void setFramecount(unsigned int framecount);

    //resolution
    unsigned int resolution() const;
    void setResolution(unsigned int resolution);

    //colorBitsDepth
    unsigned int colorBitsDepth() const;
    void setColorBitsDepth(unsigned int colorBitsDepth);

    //preview
    std::string preview() const;
    void setPreview(const std::string &preview);

    //framesList
    const FramesList& framesList() const { return _framesList; }
    void setFramesList(const FramesList& framesList) { _framesList = framesList; }

    //helpers
    bool findFramesByType(const Frames::Type& type, Frames& frames) const;
    void addOrReplaceFrames(const Frames& frames);

    //Comparison operators
    bool operator==(const Video&) const;
    bool operator!=(const Video& rhs) const { return !(operator==(rhs)); }

private:
    void updateFrameCount() const;

    std::string _videoName;
    double _framerate;
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
