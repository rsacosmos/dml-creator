#ifndef _TOOLS_H
#define _TOOLS_H

#include "Export.h"

#include <string>

struct _xmlDoc;
struct _xmlNode;

class CORE_EXPORT Tools
{
public:
    static std::string doubleToString(double value);

    static double stringToDouble(const std::string &value);

    static std::string intToString(int value);

    static int stringToInt(const std::string &value);

    static _xmlNode *rootNode(_xmlDoc *document, const std::string &name);

    static std::string nodeName(_xmlNode *node);

    static std::string nodeValue(_xmlNode *node);

    static std::string dumpNode(_xmlNode *node);

    static bool isXmlValid(const std::string &xmlContent, const std::string &xsdContent);

    static std::string escapeText(const std::string &text);

    static bool isDigit(const char c);
};
#endif
