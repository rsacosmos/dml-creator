#include "core/Tools.h"
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
