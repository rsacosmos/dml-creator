#ifndef _DML_MAINSCAN_H
#define _DML_MAINSCAN_H

#include "Export.h"

#include <QString>
#include <QDir>
#include <QThread>

#include <scan/ScanShow.h>
#include <conf/Configuration.h>

class QDomElement;
namespace dml { namespace core { class Subtitles; }}
namespace dml { namespace core { class Video; }}

namespace dml {
    namespace scan {

class SCAN_EXPORT MainScan: public QThread
{
    Q_OBJECT
public:

    enum ScanStep
    {
        Idle,           ///< No scan in progress.
        ScanPaths,      ///< Get all directories.
        ScanFiles,      ///< Get all files.
        ScanSubtitles,   ///< Lists all subtitles.
        ScanAudio,      ///< Lists all audios.
        ScanVideo,       ///< Lists all video frames.
        ScanPreview    ///< Lists all previews.
    };

    MainScan(scan::ScanShow& show, const dml::conf::Configuration &configuration);

    void start(const QString &path);
    void stop();

    static double getFramerate(const dml::conf::Configuration& configuration, const QString &filename);
    static QString getLanguage(const dml::conf::Configuration& configuration, const QString &filename);

    static QString toLocalPath(const QString &rootPath, const QString &filename);
    static QString toAbsolutePath(const QString &rootPath, const QString &filename);

    static QString generateRegularExpressions(const QStringList& sequenceRegExps, const QString& str);
    static QString generateRegularExpressionsRecursive(const QStringList patterns, int pos, QString str);

    static bool isIgnoredFile(const dml::conf::Configuration& configuration, const QString &path);
    static QString folderPathFromFilePath(const QString& filePath);
    static QString fileNameFromFilePath(const QString& filePath);

    static QStringList getFileList(const dml::conf::Configuration& configuration, const QString &path, const QRegExp& regExp);
    static QStringList getFileList(const dml::conf::Configuration& configuration, const QDir &dir, const QRegExp& regExp);
    static QStringList getFileList(const dml::conf::Configuration& configuration, const QString& path);
    static QStringList getFileList(const dml::conf::Configuration& configuration, const QDir &dir);
    static QStringList getFileList(const dml::conf::Configuration& configuration, const QStringList &pathList);


signals:
    void notifyProgress(ScanStep step);
    void notifyFinished();

protected:
    virtual void run();

private:
    QStringList getDirectoryList() const;
    QStringList getSubdirectoryList(const QString& path) const;

    void importSubtitlesList(const QStringList &files);
    void importPreviewList(const QStringList &files);
    void importAudioList(const QStringList &files);
    void importVideoList(const QStringList &files);

    QString _rootPath;
    bool    _running;

    dml::conf::Configuration _configuration;
    scan::ScanShow&     _show;
};

} // namespace scan
} // namespace dml

#endif
