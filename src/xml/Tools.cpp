#include <xml/Tools.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sstream>
#include <string.h>

std::string Tools::doubleToString(double value)
{
    std::ostringstream s;
    s << value;
    return s.str();
}

double Tools::stringToDouble(const std::string &value)
{
    double d;
    std::istringstream s(value);
    s >> d;
    return d;
}

std::string Tools::intToString(int value)
{
    std::ostringstream s;
    s << value;
    return s.str();
}

int Tools::stringToInt(const std::string &value)
{
    int i;
    std::istringstream s(value);
    s >> i;
    return i;
}

xmlNode *Tools::rootNode(xmlDoc *document, const std::string &name)
{
    if (!document)
        return 0;

    // Get root.
    xmlNode *root = document->children;
    if (!root)
        return 0;

    if (!root->name)
        return 0;

    if (strcmp((const char*) root->name, name.c_str()))
        return 0;

    return root;
}

std::string Tools::nodeName(xmlNode *node)
{
    if (!node)
        return "";

    if (!node->name)
        return "";

    return (const char *) node->name;
}

std::string Tools::nodeValue(xmlNode *node)
{
    if (!node)
        return "";

    if (!node->last)
        return "";

    if (!node->last->content)
        return "";

    return (const char *) node->last->content;
}


bool Tools::isXmlValid(const std::string &xmlContent, const std::string &xsdContent)
{
    xmlDoc *doc = xmlReadMemory(xmlContent.c_str(), static_cast<int>(xmlContent.size()), "noname.xml", NULL, 0);
    if (!doc)
        return false;

    bool valid = false;

    xmlSchemaParserCtxtPtr pctxt = xmlSchemaNewMemParserCtxt(xsdContent.c_str(), static_cast<int>(xsdContent.size()));
    if (pctxt)
    {
        xmlSchemaPtr schema = xmlSchemaParse(pctxt);
        if (schema)
        {
            xmlSchemaValidCtxtPtr vctxt = xmlSchemaNewValidCtxt(schema);
            if (vctxt)
            {
                if (xmlSchemaValidateDoc(vctxt, doc) == 0)
                    valid = true;

                xmlSchemaFreeValidCtxt(vctxt);
            }
            xmlSchemaFree(schema);
        }
        xmlSchemaFreeParserCtxt(pctxt);
    }

    xmlFreeDoc(doc);

    return valid;
}

std::string Tools::escapeText(const std::string &text)
{
    std::string output;

    for (unsigned int i=0 ; i<text.length() ; i++)
    {
        if (text.at(i) == '<')
            output += "&lt;";
        else if (text.at(i) == '>')
            output += "&gt;";
        else if (text.at(i) == '\'')
            output += "&apos;";
        else if (text.at(i) == '\"')
            output += "&quot;";
        else if (text.at(i) == '&')
            output += "&amp;";
        else
            output += text.at(i);
    }

    return output;
}

bool Tools::isDigit(const char c)
{
    if (c >= '0' && c <= '9')
        return true;
    return false;
}
