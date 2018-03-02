#ifndef __DML_XML_ELEMENT_H__
#define __DML_XML_ELEMENT_H__

#include "xml/Export.h"

#include <string>

struct _xmlDoc;
struct _xmlNode;

namespace dml {
namespace xml {

class XML_EXPORT Element
{
public:
    Element(const std::string& tagName);
    void appendElement(const Element& child);
    void appendText(const std::string& text);
    std::string dump() const;

    Element firstChildElement(const std::string& tagname) const;
    Element nextSiblingElement(const std::string& tagname) const;

    bool isNull() const;

    std::string text() const;

    std::string toString(int tab) const;
    static Element fromString(const std::string&);

private:
    Element(_xmlNode *node=NULL) : _node(node) {}
    _xmlNode *_node;
}; // class Element

} // namespace xml
} // namespace dml

#endif // __DML_XML_ELEMENT_H__
