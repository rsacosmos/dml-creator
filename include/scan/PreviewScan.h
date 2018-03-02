#ifndef _DML_PREVIEWSCAN_H
#define _DML_PREVIEWSCAN_H

#include "Export.h"

#include <QString>

#include <conf/Configuration.h>
#include <scan/ScanShow.h>

namespace dml {
    namespace scan {

class SCAN_EXPORT PreviewScan
{
public:

    static bool isPreview(const dml::conf::Configuration& configuration, const QString &filename);
    static QString qStringRoot(const QString &a, const QString &b);
    static void insertPreviewInExistingVideo(dml::scan::ScanShow& show, QStringList& previewPaths);

};

} // namespace scan
} // namespace dml

#endif
