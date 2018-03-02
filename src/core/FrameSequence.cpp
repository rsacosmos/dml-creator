#include "core/FrameSequence.h"
#include <core/Tools.h>

using namespace dml::core;

FrameSequence::FrameSequence(const std::string &dml):
   _firstframe(0)
  , _lastframe(0)
{
}

void FrameSequence::setFramespath(const std::string &framespath)
{
    _framespath = framespath;
}

void FrameSequence::setFirstframe(unsigned int firstframe)
{
    _firstframe = firstframe;
}

void FrameSequence::setLastframe(unsigned int lastframe)
{
    _lastframe = lastframe;
}

bool FrameSequence::operator==(const FrameSequence& rhs) const
{
    if (rhs._framespath != _framespath) return false;
    if (rhs._firstframe != _firstframe) return false;
    if (rhs._lastframe != _lastframe) return false;
    return true;
}

