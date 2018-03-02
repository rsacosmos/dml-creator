#include "core/Subtitles.h"
#include <core/Tools.h>

using namespace dml::core;

Subtitles::~Subtitles()
{
}

Subtitles::Subtitles(const std::string &dml)
{
}

std::string Subtitles::path() const
{
    return _path;
}

void Subtitles::setPath(const std::string &path)
{
    _path = path;
}

std::string Subtitles::language() const
{
    return _language;
}

void Subtitles::setLanguage(const std::string &language)
{
    _language = language;
}

bool Subtitles::operator==(const Subtitles& rhs) const
{
    if (rhs._path != _path) return false;
    if (rhs._language != _language) return false;
    return true;
}