#ifndef __DML_CORE_FRAMESEQUENCE_H__
#define __DML_CORE_FRAMESEQUENCE_H__

#include "Export.h"

#include <string>
#include <vector>

namespace dml {
namespace core {

class CORE_EXPORT FrameSequence
{
public:

    FrameSequence(const std::string &dml = "");

    void setFramespath(const std::string &framespath);
    std::string framespath() const { return _framespath; }

    void setFirstframe(unsigned int firstframe);
    unsigned int firstframe() const { return _firstframe; }

    void setLastframe(unsigned int lastframe);
    unsigned int lastframe() const { return _lastframe; }

    // Comparison operators
    bool operator==(const FrameSequence&) const;
    bool operator!=(const FrameSequence& rhs) const { return !(operator==(rhs)); }

private:
    std::string _framespath;
    unsigned int _firstframe;
    unsigned int _lastframe;
};
typedef std::vector<FrameSequence> FrameSequenceList;

} // namespace core
} // namespace dml

#endif // __DML_CORE_FRAMESEQUENCE_H__
