#include "core/Show.h"
#include <core/Tools.h>

using namespace dml::core;

void Information::setProducer(const std::string &producer)
{
    _producer = producer;
}

void Information::setCopyright(const std::string &copyright)
{
    _copyright = copyright;
}

void Information::setDescription(const std::string &description)
{
    _description = description;
}

void Information::setPublicationDate(const std::string &publicationDate)
{
    _publicationDate = publicationDate;
    _hasPublicationDate = true;
}

bool Information::isNull() const
{
    if (!_producer.empty())
        return false;
    if (!_copyright.empty())
        return false;
    if (!_description.empty())
        return false;
    if (!_publicationDate.empty())
        return false;

    return true;
}

void Information::clear()
{
    _producer.clear();
    _copyright.clear();
    _description.clear();
    _publicationDate.clear();
}
