#ifndef __DML_CORE_FRAMES_H__
#define __DML_CORE_FRAMES_H__

#include "Export.h"
#include "core/FrameSequence.h"

#include <string>
#include <vector>
#include <map>

namespace dml {
namespace core {

class CORE_EXPORT Frames
{
public:

    enum Type
    {
        Video2d,            ///< Video is not stereoscopic.
        StereoscopicLeft,   ///< Video is stereoscopic & represents left part of video.
        StereoscopicRight   ///< Video is stereoscopic & represents right part of video.
    };

    typedef std::map<Type, std::string> TypeMap;


    Frames(const std::string &dml = "");

    void setType(const Type &type);
    Type type() const { return _type; }

    void setFrameSequences(const FrameSequenceList &frameSequences);
    FrameSequenceList frameSequences() const { return _frameSequences; }

    // Comparison operators
    bool operator==(const Frames&) const;
    bool operator!=(const Frames& rhs) const { return !(operator==(rhs)); }

    static std::string typeToString(Type);
    static Type typeFromString(const std::string&);

private:
    Type _type;
    FrameSequenceList _frameSequences;
};
typedef std::vector<Frames> FramesList;

} // namespace core
} // namespace dml

#endif // __DML_CORE_FRAMES_H__
