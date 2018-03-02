#include <xml/Element.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sstream>
#include <string.h>

static const std::string XML_DEFAULT_HEADER("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

void dml::xml::Element::appendElement(const Element& child)
{
    xmlAddChild(_node, child._node);
}

std::string dml::xml::Element::toString(int tab) const
{
    std::string result;
    bool first = (NULL==_node->doc);
    if (first)
    {
        // XML document initialization
        xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
        xmlDocSetRootElement(doc, _node);
        result = XML_DEFAULT_HEADER;
    }
    result += dump();
    if (first)
    {
        // delete doc ?
    }
    return result;
}

dml::xml::Element::Element(const std::string& tagName)
    : _node(xmlNewNode(NULL, BAD_CAST tagName.c_str()))
{
}

void dml::xml::Element::appendText(const std::string& text)
{
    xmlNodePtr node = xmlNewText(BAD_CAST text.c_str());
    xmlAddChild(_node, node);
}

std::string dml::xml::Element::dump() const
{
    if (!_node)
        return "";

    if (!_node->doc)
        return "";

    xmlBuffer *buffer = xmlBufferCreate();
    if (!buffer)
        return "";

    xmlNodeDump(buffer, _node->doc, _node, 0, 1);

    const char *out = (const char *) xmlBufferContent(buffer);
    std::string output;
    if (out)
        output = out;

    xmlBufferFree(buffer);

    return output;
}

dml::xml::Element dml::xml::Element::fromString(const std::string& content)
{
    xmlDoc *doc = xmlReadMemory(content.c_str(), static_cast<int>(content.size()), "noname.xml", NULL, 0);
    if (doc == NULL) {
        return Element();
    }
    return Element(doc->children);
}

bool dml::xml::Element::isNull() const
{
    return NULL==_node;
}

dml::xml::Element dml::xml::Element::firstChildElement(const std::string& tagname) const
{
    if (!isNull())
    {
        for (xmlNodePtr node = xmlFirstElementChild(_node);
            NULL!=node;
            node = xmlNextElementSibling(node))
        {
            if (tagname==std::string((const char*)node->name))
            {
                return Element(node);
            }
        }
    }
    return Element();
}

dml::xml::Element dml::xml::Element::nextSiblingElement(const std::string& tagname) const
{
    if (!isNull())
    {
        for (xmlNodePtr node = xmlNextElementSibling(_node);
            NULL!=node;
            node = xmlNextElementSibling(node))
        {
            if (tagname==std::string((const char*)node->name))
            {
                return Element(node);
            }
        }
    }
    return Element();
}

std::string dml::xml::Element::text() const
{
    std::string result;
    if (!isNull())
    {
        if (!_node->children) return std::string();
        if ("text"!=std::string((const char*)_node->children->name)) return std::string();
        if (!_node->children->content) return std::string();
        xmlChar *xmlText = _node->children->content;
        result = std::string((const char*)xmlText);
    }
    return result;
}
