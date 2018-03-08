#ifndef _DML_SUBTITLESSCAN_H
#define _DML_SUBTITLESSCAN_H

#include "Export.h"

#include <QString>

#include <conf/Configuration.h>

namespace dml {
namespace scan {

class SCAN_EXPORT SubtitlesScan
{
public:
  static bool isSubtitles(const dml::conf::Configuration& configuration, const QString &filename);
};

} // namespace scan
} // namespace dml

#endif
