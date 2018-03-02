#include "scan/SubtitlesScan.h"

#include <QRegExp>

using namespace dml::scan;


bool SubtitlesScan::isSubtitles(const dml::conf::Configuration& configuration, const QString &filename)
{
    QRegExp exp(configuration.subtitlesExtensionPattern());
    return exp.exactMatch(filename);
}
