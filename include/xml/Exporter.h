#ifndef __DML_XML_EXPORTER_H__
#define __DML_XML_EXPORTER_H__

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

class XML_EXPORT Exporter
{
public:
    static Element toDml(const core::Show& show);

private:
    static Element toDml(const dml::core::Video&);
    static Element toDml(const dml::core::Audio&);
    static Element toDml(const dml::core::Frames&);
    static Element toDml(const dml::core::FrameSequence&);
    static Element toDml(const dml::core::Subtitles&);
}; // class Element

} // namespace xml
} // namespace dml

#endif // __DML_XML_EXPORTER_H__
