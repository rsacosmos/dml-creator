#ifndef _DML_AUDIOSCAN_H
#define _DML_AUDIOSCAN_H

#include "Export.h"

#include <QString>
#include <QMap>

#include <conf/Configuration.h>

namespace dml {
    namespace scan {

class SCAN_EXPORT AudioScan
{
public:
    static bool isAudio(const dml::conf::Configuration& configuration, const QString &filename);
    static QString AudioScan::refactoringStereoscopicTypesPath(const dml::conf::Configuration& configuration, QString path);
    static dml::core::Audio::Speaker getAudioType(const dml::conf::Configuration& configuration, const QString& pattern, const QString &filename);
    static QMap<QString, QStringList> generateAllRegularExpressions(const dml::conf::Configuration& configuration, const QString& rootPath, const QStringList &files);
};

} // namespace scan
} // namespace dml

#endif
