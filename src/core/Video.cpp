#include "core/Video.h"
#include <core/Tools.h>

using namespace dml::core;

Video::~Video()
{
}

Video::Video(const std::string &dml):
    _framerate(0)
  , _framecount(0)
  , _resolution(0)
  , _colorBitsDepth(0)
{
}

std::string Video::videoName() const
{
    return _videoName;
}

void Video::setVideoName(const std::string &title)
{
    _videoName = title;
}

double Video::framerate() const
{
    return _framerate;
}

void Video::setFramerate(double framerate)
{
    _framerate = framerate;
}

unsigned int Video::framecount() const
{
    return _framecount;
}

void Video::setFramecount(unsigned int framecount)
{
    _framecount = framecount;
}

unsigned int Video::resolution() const
{
    return _resolution;
}

void Video::setResolution(unsigned int resolution)
{
    _resolution = resolution;
}

unsigned int Video::colorBitsDepth() const
{
    return _colorBitsDepth;
}

void Video::setColorBitsDepth(unsigned int colorBitsDepth)
{
    _colorBitsDepth = colorBitsDepth;
}

std::string Video::preview() const
{
    return _preview;
}

void Video::setPreview(const std::string &preview)
{
    _preview = preview;
}

void Video::updateFrameCount() const
{
    _framecount = 0;

    for (unsigned int i=0 ; i<_framesList.size() ; i++)
    {
        const dml::core::Frames &frames = _framesList.at(i);
        std::vector<FrameSequence> frameSequences = frames.frameSequences();
        for (unsigned int i=0 ; i<frameSequences.size() ; i++)
        {
            const dml::core::FrameSequence &frameSequence = frameSequences.at(i);

            int last  = frameSequence.lastframe();
            int first = frameSequence.firstframe();

            int n = last - first + 1;

            _framecount += n;
        }
    }
}

bool Video::operator==(const Video& rhs) const
{
    if (rhs._videoName != _videoName) return false;
    if (rhs._framerate != _framerate) return false;
    if (rhs._framecount != _framecount) return false;
    if (rhs._resolution != _resolution) return false;
    if (rhs._preview != _preview) return false;
    if (rhs._colorBitsDepth != _colorBitsDepth) return false;
    if (rhs._framesList != _framesList) return false;
    return true;
}

bool Video::findFramesByType(const Frames::Type& type, Frames& frames) const
{
    for(int i = 0; i < _framesList.size(); ++i)
    {
        if(_framesList.at(i).type() == type)
        {
            frames = _framesList.at(i);
            return true;
        }
    }
    return false;
}

void Video::addOrReplaceFrames(const Frames& frames)
{
    for(int i = 0; i < _framesList.size(); ++i)
    {
        if(_framesList.at(i).type() == frames.type())
        {
            _framesList.at(i) = frames;
            return;
        }
    }
    _framesList.push_back(frames);
}