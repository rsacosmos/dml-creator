#include "scan/MainScan.h"

#include <QEventLoop>
#include <QTimer>
#include <QSize>
#include <QImage>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QMetaEnum>

#include <core/Subtitles.h>
#include <core/Video.h>
#include <core/Audio.h>

#include <scan/AudioScan.h>
#include <scan/SubtitlesScan.h>
#include <scan/VideoScan.h>
#include <scan/PreviewScan.h>

static bool s_signalInitializationDone = false;

using namespace dml::scan;

MainScan::MainScan(ScanShow& show, const dml::conf::Configuration &configuration)
  : _show(show)
  , _running(false)
  , _configuration(configuration)
{
    // Register custom classes.
    if (!s_signalInitializationDone)
    {
        qRegisterMetaType<dml::core::Subtitles>("dml::core::Subtitles");
        qRegisterMetaType<dml::core::Audio>("dml::core::Audio");
        qRegisterMetaType<dml::core::Video>("dml::core::Video");
        qRegisterMetaType<ScanStep>("ScanStep");

        s_signalInitializationDone = true;
    }
}

void MainScan::start(const QString &path)
{
    _rootPath = path;
    _running  = true;

    QThread::start();
}

void MainScan::stop()
{
    _running = false;
}

void MainScan::run()
{
    // List all directories.
    emit notifyProgress(ScanPaths);
    QStringList paths = getDirectoryList();

    if (!_running)
    {
        emit notifyProgress(Idle);
        emit notifyFinished();
        return;
    }

    // List all files.
    emit notifyProgress(ScanFiles);
    QStringList files = getFileList(_configuration, paths);

    if (!_running)
    {
        emit notifyProgress(Idle);
        emit notifyFinished();
        return;
    }

    // List all subtitles.
    emit notifyProgress(ScanSubtitles);
    importSubtitlesList(files);
    _show.refreshGUI();

    if (!_running)
    {
        emit notifyProgress(Idle);
        emit notifyFinished();
        return;
    }

    // List all audio.
    emit notifyProgress(ScanAudio);
    importAudioList(files);
    _show.refreshGUI();

    if (!_running)
    {
        emit notifyProgress(Idle);
        emit notifyFinished();
        return;
    }

    // List all video.
    emit notifyProgress(ScanVideo);
    importVideoList(files);
    _show.refreshGUI();

    if (!_running)
    {
        emit notifyProgress(Idle);
        emit notifyFinished();
        return;
    }

    // Import all preview files.
    emit notifyProgress(ScanPreview);
    importPreviewList(files);
    _show.refreshGUI();

    emit notifyProgress(Idle);
    emit notifyFinished();
}


QStringList MainScan::getDirectoryList() const
{
    QStringList result = getSubdirectoryList(_rootPath);
    result.append(_rootPath);
    return result;
}

QStringList MainScan::getSubdirectoryList(const QString &path) const
{
    QDir dir(path);
    QFileInfoList paths = dir.entryInfoList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

    QStringList list;

    foreach(QFileInfo info, paths)
    {
        QString p = info.absoluteFilePath();

        // Add all sub-directories.
        list << p;

        // Recursive call.
        QStringList r_files = getSubdirectoryList(p);
        list << r_files;
    }

    return list;
}

QStringList MainScan::getFileList(const dml::conf::Configuration& configuration, const QStringList &pathList)
{
    QStringList list;

    foreach(QString path, pathList)
    {
        list << getFileList(configuration, path);
    }

    return list;
}

QStringList MainScan::getFileList(const dml::conf::Configuration& configuration, const QString &path)
{
    return getFileList(configuration, QDir(path));
}

QStringList MainScan::getFileList(const dml::conf::Configuration& configuration, const QDir &dir)
{
    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

    QStringList list;

    foreach(QFileInfo info, files)
    {
        QString p = info.canonicalFilePath();
        bool ignored = isIgnoredFile(configuration, p);
        if (ignored)
            continue;

        list << p;
    }

    return list;
}

QStringList MainScan::getFileList(const dml::conf::Configuration& configuration, const QString &path, const QRegExp& regExp)
{
    return getFileList(configuration, QDir(path), regExp);
}

QStringList MainScan::getFileList(const dml::conf::Configuration& configuration, const QDir &dir, const QRegExp& regExp)
{
    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

    QStringList list;

    foreach(QFileInfo info, files)
    {
        QString p = info.canonicalFilePath();
        bool ignored = isIgnoredFile(configuration, p);
        if (ignored || !regExp.exactMatch(p))
            continue;

        list << p;
    }

    return list;
}

bool MainScan::isIgnoredFile(const dml::conf::Configuration& configuration, const QString &path) 
{
    foreach(const QString &pattern, configuration.ignoredFileList())
    {
        QRegExp exp(pattern);
        if (exp.exactMatch(path))
            return true;
    }
    return false;
}

void MainScan::importSubtitlesList(const QStringList &files)
{
    foreach(const QString &file, files)
    {
        if (!SubtitlesScan::isSubtitles(_configuration, file))
            continue;

        QString relativePath     = toLocalPath(_rootPath, file);
        QString language = getLanguage(_configuration, relativePath);
        if(language.isEmpty())
        {
            language = _configuration.defaultSubtitlesLanguage().toLower();
        }

        dml::core::Subtitles subtitles;
        subtitles.setLanguage(language.toStdString());
        subtitles.setPath(relativePath.toStdString());

        _show.addSubtitles(subtitles);

        if (!_running)
            return;
    }
}


void MainScan::importPreviewList(const QStringList &files)
{
    QStringList relativeFilePaths;
    foreach(const QString &file, files)
    {
        if (!PreviewScan::isPreview(_configuration, file))
            continue;

        QString name = toLocalPath(_rootPath, file);
        relativeFilePaths << name;

        if (!_running)
            return;
    }
    PreviewScan::insertPreviewInExistingVideo(_show, relativeFilePaths);
    foreach(const QString &relativeFilePath, relativeFilePaths)
    {
        core::Video video;
        video.setVideoName(relativeFilePath.toStdString());

        QString absoluteFilePath = toAbsolutePath(_rootPath, relativeFilePath);

        QMediaPlayer player;
        player.setMedia(QUrl::fromLocalFile(absoluteFilePath));

        QEventLoop loop;
        QTimer timer;
        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        connect(&player, SIGNAL(metaDataChanged()), &loop, SLOT(quit()));
        timer.start(2000);
        loop.exec();

        QSize resolution = player.metaData(QMediaMetaData::Resolution).value<QSize>();
        qreal framerate = player.metaData(QMediaMetaData::VideoFrameRate).value<qreal>();

        video.setResolution(resolution.width());
        video.setColorBitsDepth(_configuration.defaultColorBitDepth());
        video.setFramerate((framerate == 0.) ? _configuration.defaultFramerate() : framerate);
        video.setPreview(relativeFilePath.toStdString());
        _show.addVideo(video);                
    }
}

void MainScan::importAudioList(const QStringList &files)
{
    QMap<QString, QStringList> relativeFolderPathsByRegularExpressions = AudioScan::generateAllRegularExpressions(_configuration, _rootPath, files);
    if (!_running)
        return;

    QList<QString> regularExpressions = relativeFolderPathsByRegularExpressions.keys();
    foreach(const QString &regExp, regularExpressions)
    {
        QString title = AudioScan::refactoringStereoscopicTypesPath(_configuration, regExp);

        core::Audio audio;

        QString language = getLanguage(_configuration, title);
        if(language.isEmpty())
        {
            language = _configuration.defaultAudioLanguage().toLower();
        }
        audio.setLanguage(language.toStdString());
        double frameRate = getFramerate(_configuration, title);
        audio.setPictureFrameRate(frameRate);
        audio.setAudioName(title.toStdString());

        QStringList relativeGeneralFilePaths = relativeFolderPathsByRegularExpressions.value(regExp);

        dml::core::Audio::ChannelMap channelMap;

        foreach(const QString &relativeGeneralFilePath, relativeGeneralFilePaths)
        {
            dml::core::Audio::Speaker type = AudioScan::getAudioType(_configuration, regExp, relativeGeneralFilePath);
            channelMap[type] = relativeGeneralFilePath.toStdString();
        }
        audio.setChannelMap(channelMap);
        _show.addAudio(audio);
    }
}


void MainScan::importVideoList(const QStringList &files)
{
    QMap<QString, QStringList> relativeFolderPathsByRegularExpressions = VideoScan::generateAllRegularExpressions(_configuration, _rootPath, files);
    if (!_running)
        return;

    QList<QString> regularExpressions = relativeFolderPathsByRegularExpressions.keys();
    foreach(const QString &regExp, regularExpressions)
    {
        QString videoName = VideoScan::refactoringFrameSequencePath(VideoScan::refactoringStereoscopicTypesPath(_configuration, regExp));

        core::Video video;
        video.setVideoName(videoName.toStdString());
        video.setFramecount(0);
        bool videoIsEmpty = true;

        QStringList relativeGeneralFilePaths = relativeFolderPathsByRegularExpressions.value(regExp);
        foreach(const QString &relativeGeneralFilePath, relativeGeneralFilePaths)
        {
            QString absoluteGeneralFilePath = toAbsolutePath(_rootPath, relativeGeneralFilePath);
            QString absoluteGeneralFolderPath = folderPathFromFilePath(absoluteGeneralFilePath);            

            QStringList localFiles = getFileList(_configuration, absoluteGeneralFolderPath, QRegExp(absoluteGeneralFilePath));
            QList<unsigned int> sequenceNumber = VideoScan::capAllFrameIndex(localFiles, absoluteGeneralFilePath);
            if (!_running)
                return;

            QList<QList<unsigned int> > subSequenceNumber = VideoScan::subdiviseContiguousInteger(sequenceNumber);
            foreach(const QList<unsigned int> &sequence, subSequenceNumber)
            {
                if (sequence.size() >= _configuration.minVideoFramesCount())
                {       
                    if(videoIsEmpty)
                    {
                        QImage image(localFiles.first());
                        video.setResolution(image.size().width());
                        video.setColorBitsDepth(image.bitPlaneCount());

                        double pictureFrameRate = getFramerate(_configuration, videoName);
                        video.setFramerate(pictureFrameRate);
                    }
                    videoIsEmpty = false;

                    QString absoluteGeneralStereoscopicFilePath = toAbsolutePath(_rootPath, regExp);
                    dml::core::Frames::Type type = VideoScan::getFramesType(_configuration, absoluteGeneralStereoscopicFilePath, localFiles.first());

                    dml::core::Frames frames;
                    if(!video.findFramesByType(type, frames))
                    {
                        frames.setType(type);
                    }
                    dml::core::FrameSequence frameSequence;

                    frameSequence.setFirstframe(sequence.first());
                    frameSequence.setLastframe(sequence.last());
                    frameSequence.setFramespath(VideoScan::refactoringFrameSequencePath(relativeGeneralFilePath).toStdString());
                    std::vector<core::FrameSequence> frameSequences = frames.frameSequences();
                    frameSequences.push_back(frameSequence);
                    frames.setFrameSequences(frameSequences);

                    video.setFramecount(video.framecount() + sequence.last()- sequence.first() +1);

                    video.addOrReplaceFrames(frames);
                }
            }
        }
        if(!videoIsEmpty)
        {
            _show.addVideo(video);                 
        }
    }
}

QString MainScan::toLocalPath(const QString &rootPath, const QString &filename)
{
    return QDir(rootPath).relativeFilePath(filename);
}

QString MainScan::toAbsolutePath(const QString &rootPath, const QString &filename)
{
    return QDir(rootPath).absoluteFilePath(filename);
}

QString MainScan::getLanguage(const dml::conf::Configuration& configuration, const QString &filename)
{
    foreach(const QString &pattern, configuration.languagePatternList())
    {
        QRegExp regExp(pattern);

        int pos = 0;
        while((pos = regExp.indexIn(filename, pos)) != -1)
        {
            QString textCapured = regExp.cap(1);

            const QMetaObject &mo = QLocale::staticMetaObject;
            int index = mo.indexOfEnumerator("Language");
            QMetaEnum metaEnum = mo.enumerator(index);
            bool ok = false;
            QLocale::Language language = (QLocale::Language) metaEnum.keyToValue(textCapured.toStdString().c_str(), &ok);
            if(ok)
            {
                return QLocale(language).bcp47Name();
            }
            else
            {
                QLocale locale(textCapured.toLower());
                if(locale.language() != QLocale::C)
                {
                    return locale.bcp47Name();
                }
             }         
            pos = regExp.pos(1) + textCapured.size();
        }
    }
    return "";
}

double MainScan::getFramerate(const dml::conf::Configuration& configuration, const QString &filename)
{
    foreach(const QString& pattern, configuration.frameratePatternList())
    {
        QRegExp regExp(pattern);

        int pos = 0;
        while((pos = regExp.indexIn(filename, pos)) != -1)
        {
            QString textCapured = regExp.cap(1);

            bool ok = false;
            double frameRate = textCapured.toDouble(&ok);
            if(ok)
            {
                while(frameRate > 100)
                {
                    frameRate /= 10;
                }
                return frameRate;
            }
            pos = regExp.pos(1) + textCapured.size();
        }
    }
    return configuration.defaultFramerate();
}






QString MainScan::folderPathFromFilePath(const QString& filePath)
{
    return filePath.section('/', 0, -2);
}

QString MainScan::fileNameFromFilePath(const QString& filePath)
{
    return filePath.section('/', -1, -1);
}

QString MainScan::generateRegularExpressions(const QStringList& sequenceRegExps, const QString& str)
{
    return generateRegularExpressionsRecursive(sequenceRegExps, 0, str);
}

QString MainScan::generateRegularExpressionsRecursive(const QStringList patterns, int pos, QString str)
{
    QString regularExpression;

    if(!patterns.isEmpty())
    {
        QRegExp regExp(patterns.first());
        QString replaceBy = QString("(%1)").arg(patterns.first().section("(", -1, -1).section(")", 0, 0));

        while((pos = regExp.indexIn(str, pos)) != -1)
        {
            QString textCapured = regExp.cap(1);

            str.replace(regExp.pos(1), textCapured.size(), replaceBy);
            pos = regExp.pos(1) + qMax(replaceBy.size(), textCapured.size());
        }

        QStringList nextPatternReplaceables(patterns);
        nextPatternReplaceables.removeFirst();
        regularExpression = generateRegularExpressionsRecursive(nextPatternReplaceables, 0, str);
    }
    else
    {
        regularExpression = str;
    }
    return regularExpression;
}
