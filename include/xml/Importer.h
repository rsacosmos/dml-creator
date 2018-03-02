#ifndef __DML_XML_IMPORTER_H__
#define __DML_XML_IMPORTER_H__

#include "xml/Export.h"

#include <string>

#include <core/Show.h>
#include <core/Video.h>
#include <core/Audio.h>
#include <core/Frames.h>
#include <core/FrameSequence.h>
#include <core/Subtitles.h>

#include <xml/Element.h>

struct _xmlDoc;
struct _xmlNode;

namespace dml {
namespace xml {

class XML_EXPORT Importer
{
public:
    static core::Show fromDml(const Element&);
    static core::Show fromDml(const std::string&); // for convenience

private:
    static core::Audio fromDml(const Element&, const core::Audio& defaultValue);
    static core::Video fromDml(const Element&, const core::Video& defaultValue);
    static core::Frames fromDml(const Element&, const core::Frames& defaultValue);
    static core::FrameSequence fromDml(const Element&, const core::FrameSequence& defaultValue);
    static core::Subtitles fromDml(const Element&, const core::Subtitles& defaultValue);

    core::Show _show;
}; // class Element

} // namespace xml
} // namespace dml

#endif // __DML_XML_IMPORTER_H__
