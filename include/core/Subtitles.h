#ifndef __DML_CORE_SUBTITLES_H__
#define __DML_CORE_SUBTITLES_H__

#include "core/Export.h"

#include <string>
#include <vector>

namespace dml {
namespace core {

class CORE_EXPORT Subtitles
{
public:
    virtual ~Subtitles();

    Subtitles(const std::string &dml = "");

    void setPath(const std::string &path);
    std::string path() const;

    void setLanguage(const std::string &language);
    std::string language() const;

    // Comparison operators
    bool operator==(const Subtitles&) const;
    bool operator!=(const Subtitles& rhs) const { return !(operator==(rhs)); }

private:
    std::string _path;
    std::string _language;
};

typedef std::vector<Subtitles> SubtitlesList;
typedef std::vector<Subtitles>::iterator SubtitlesListIterator;
typedef std::vector<Subtitles>::const_iterator SubtitlesListConstIterator;

} // namespace dml
} // namespace core

#endif // __DML_CORE_SUBTITLES_H__
