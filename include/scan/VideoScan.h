#ifndef _DML_VIDEOSCAN_H
#define _DML_VIDEOSCAN_H

#include "Export.h"

#include <QString>

#include <conf/Configuration.h>

namespace dml { namespace core { class Frames; }}

namespace dml {
    namespace scan {

class SCAN_EXPORT VideoScan
{
public:

  static bool isVideoImage(const dml::conf::Configuration& configuration, const QString &filename);

  static dml::core::Frames::Type getFramesType(const dml::conf::Configuration& configuration, const QString& pattern, const QString &filename);
  static QString refactoringFrameSequencePath(QString path);
  static QString VideoScan::refactoringStereoscopicTypesPath(const dml::conf::Configuration& configuration, QString path);

  static QList<unsigned int> capAllFrameIndex(const QStringList& localFiles, const QString& absoluteGeneralFilePath);
  static QList<QList<unsigned int> > subdiviseContiguousInteger(QList<unsigned int>& allInteger);
  static QList<unsigned int> getFrameIndexesWhichMatch(const QList<QList<unsigned int> >& subSequenceNumber, const unsigned int& index);

  static QMap<QString, QStringList> generateAllRegularExpressions(const dml::conf::Configuration& configuration, const QString& rootPath, const QStringList &files);
  static QList<QString> generateRegularExpressionSequenceFrameFromString(const QRegExp& regExp, const QString& str);

  static QString getRegularExpressionWhichMatch(const QList<QString>& regularExpressions, const QString& relativeFilePath);

  static bool regularExpressionCap(const QRegExp& regExp, const QString& string, QStringList& matches);
  static bool regularExpressionCapFirstInt(const QRegExp& regExp, const QString& string, int& number);

  static void guessSequenceFromTo(const dml::conf::Configuration& configuration, const QString& rootPath, const QString &filename, QString& path, unsigned int& firstFrameIndex, unsigned int& lastFrameIndex);
};

} // namespace scan
} // namespace dml

#endif
