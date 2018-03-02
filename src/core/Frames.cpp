#include "core/Frames.h"
#include <core/Tools.h>

using namespace dml::core;

Frames::Frames(const std::string &dml):
    _type(Video2d)
{
}

void Frames::setType(const Type &type)
{
    _type = type;
}

void Frames::setFrameSequences(const FrameSequenceList &frameSequences)
{
    _frameSequences = frameSequences;
}

std::string Frames::typeToString(Frames::Type type)
{
    if (type == StereoscopicLeft)
            return "left";
    if (type == StereoscopicRight)
            return "right";
    return "";
}

Frames::Type Frames::typeFromString(const std::string &value)
{
    if (value == "left")
            return StereoscopicLeft;
    if (value == "right")
            return StereoscopicRight;
    return Video2d;
}

bool Frames::operator==(const Frames& rhs) const
{
    if (rhs._type != _type) return false;
    if (rhs._frameSequences != _frameSequences) return false;
    return true;
}

