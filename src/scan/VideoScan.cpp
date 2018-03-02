#include <scan/VideoScan.h>

#include <core/Frames.h>
#include <scan/MainScan.h>

#include <QRegExp>

using namespace dml::scan;


bool VideoScan::isVideoImage(const dml::conf::Configuration& configuration, const QString &filename)
{
    QRegExp exp(configuration.videoExtensionPattern());
    return exp.exactMatch(filename);
}

dml::core::Frames::Type VideoScan::getFramesType(const dml::conf::Configuration& configuration, const QString& pattern, const QString &filename)
{
    QRegExp exp(pattern);
    exp.indexIn(filename);

    QStringList texts = exp.capturedTexts();
    texts.removeDuplicates();

    dml::conf::Configuration::FramesTypeList framesTypeList = configuration.framesTypeList();
    for (dml::conf::Configuration::FramesTypeList::const_iterator it=framesTypeList.begin() ; it!=framesTypeList.end() ; it++)
    {
        dml::core::Frames::Type type = it.key();

        foreach(const QString &typeString, it.value())
        {
            foreach(const QString &text, texts)
            {
                if (text != typeString)
                    continue;

                return type;
            }            
        }
    }
    // Default type.
    return dml::core::Frames::Video2d;
}

void VideoScan::guessSequenceFromTo(const dml::conf::Configuration& configuration, const QString& rootPath, const QString &filename, QString& path, unsigned int& firstFrameIndex, unsigned int& lastFrameIndex)
{
    QString relativeFilePath = MainScan::toLocalPath(rootPath, filename);

    QDir dir = QFileInfo(filename).absoluteDir();
    QStringList fileList = MainScan::getFileList(configuration, dir);
    QMap<QString, QStringList> relativeFolderPathsByRegularExpressions = generateAllRegularExpressions(configuration, rootPath, fileList);
    QList<QString> relativeGeneralFilePaths = relativeFolderPathsByRegularExpressions.keys();

    QString relativeGeneralFilePath = getRegularExpressionWhichMatch(relativeGeneralFilePaths, relativeFilePath);
    QStringList relativeFrameSequenceFilePaths = relativeFolderPathsByRegularExpressions.value(relativeGeneralFilePath);
    foreach(const QString& relativeFrameSequenceFilePath, relativeFrameSequenceFilePaths)
    {
        QRegExp regExp(relativeFrameSequenceFilePath);
        if(regExp.exactMatch(relativeFilePath))
        {
            QString absoluteFrameSequenceFilePath = MainScan::toAbsolutePath(rootPath, relativeFrameSequenceFilePath);
            QString absoluteFrameSequenceFolderPath = MainScan::folderPathFromFilePath(absoluteFrameSequenceFilePath);            

            QStringList localFiles = MainScan::getFileList(configuration, dir, QRegExp(absoluteFrameSequenceFilePath));
            QList<unsigned int> sequenceNumber = capAllFrameIndex(localFiles, absoluteFrameSequenceFilePath);

            QList<QList<unsigned int> > subSequenceNumber = subdiviseContiguousInteger(sequenceNumber);
            int index = 0;
            regularExpressionCapFirstInt(QRegExp(absoluteFrameSequenceFilePath), filename, index);
            QList<unsigned int> frameIndexes = getFrameIndexesWhichMatch(subSequenceNumber, index);

            path = refactoringFrameSequencePath(relativeFrameSequenceFilePath);
            firstFrameIndex = frameIndexes.first();
            lastFrameIndex = frameIndexes.last();
        }
    }
}

QString VideoScan::refactoringFrameSequencePath(QString path)
{
    QRegExp rx("\\(\\\\d\\{(\\d+)\\}\\)");
    int pos = 0;
    while ((pos = rx.indexIn(path, pos)) != -1)
    {
        path.replace(pos, rx.matchedLength(), "%0" + rx.cap(1));
    }
    return path;
}

QString VideoScan::refactoringStereoscopicTypesPath(const dml::conf::Configuration& configuration, QString path)
{
    return path.replace(configuration.stereoscopicTypes(), configuration.stereoscopicTypeListRef());
}

QList<unsigned int> VideoScan::capAllFrameIndex(const QStringList& localFiles, const QString& absoluteGeneralFilePath)
{
    QList<unsigned int> sequenceNumber;
    foreach(const QString &localFile, localFiles)
    {
        int number = 0;
        if(regularExpressionCapFirstInt(QRegExp(absoluteGeneralFilePath), localFile, number))
        {
            sequenceNumber.append(number);
        }
    }
    return sequenceNumber;
}

QString VideoScan::getRegularExpressionWhichMatch(const QList<QString>& regularExpressions, const QString& relativeFilePath)
{
    foreach(const QString& regularExpression, regularExpressions)
    {
        if(QRegExp(regularExpression).exactMatch(relativeFilePath))
            return regularExpression;
    }
    return QString();
}

QList<unsigned int> VideoScan::getFrameIndexesWhichMatch(const QList<QList<unsigned int> >& subSequenceNumber, const unsigned int& index)
{
    foreach(const QList<unsigned int>& sequencNumber, subSequenceNumber)
    {
        if(sequencNumber.contains(index))
            return sequencNumber;
    }
    return QList<unsigned int>();
}

bool VideoScan::regularExpressionCap(const QRegExp& regExp, const QString& string, QStringList& matches)
{
    if(!regExp.exactMatch(string))
        return false;
    regExp.indexIn(string);
    matches = regExp.capturedTexts();
    return true;
}

bool VideoScan::regularExpressionCapFirstInt(const QRegExp& regExp, const QString& string, int& number)
{
    QStringList matches;                    
    if(regularExpressionCap(regExp, string, matches))
    {
        bool insert = false;
        foreach(const QString& match, matches)
        {
            bool ok = false;
            int n = match.toInt(&ok);
            if(ok)
            {
                number = n;
                return true;
            }
        }
    }
    return false;
}

QList<QList<unsigned int> > VideoScan::subdiviseContiguousInteger(QList<unsigned int>& allInteger)
{
    QList<QList<unsigned int> > res;

    if(allInteger.isEmpty())
        return res;

    qSort(allInteger);

    unsigned int prev = allInteger.first();
    QList<unsigned int>* currentList = new QList<unsigned int>();
    currentList->append(prev);

    for(int i = 1; i < allInteger.size(); ++i)
    {
        unsigned int current = allInteger.at(i);
        if((prev + 1) != current)
        {
            res.append(QList<unsigned int>(*currentList));
            delete currentList;
            currentList = new QList<unsigned int>();
        }
        prev = current;
        currentList->append(prev);
    }
    res.append(QList<unsigned int>(*currentList));
    delete currentList;

    return res;
}

QList<QString> VideoScan::generateRegularExpressionSequenceFrameFromString(const QRegExp& regExp, const QString& str)
{
    QList<QString> regularExpressions;

    QString fileName = MainScan::fileNameFromFilePath(str);
    QString filePath = MainScan::folderPathFromFilePath(str);

    int pos = 0;
    while((pos = regExp.indexIn(fileName, pos)) != -1)
    {
        QString textCapured = regExp.cap(1);

        QString after(QString("(\\d{%1})").arg(textCapured.size()));
        QString fileNameCopy(fileName);
        fileNameCopy = fileNameCopy.replace(regExp.pos(1), textCapured.size(), after);

        QString relativeFilePath;
        if(!filePath.isEmpty())
        {
            relativeFilePath = filePath + '/' + fileNameCopy;
        }
        else
        {
            relativeFilePath = fileNameCopy;
        }
        regularExpressions << relativeFilePath;
        pos = regExp.pos(1) + textCapured.size();
    }
    return regularExpressions;
}

QMap<QString, QStringList> VideoScan::generateAllRegularExpressions(const dml::conf::Configuration& configuration, const QString& rootPath, const QStringList &files)
{
    QMap<QString, QStringList> relativeFolderPathsByRegularExpressions;
    foreach(const QString &file, files)
    {
        if (!isVideoImage(configuration, file))
            continue;

        QString relativeFilePath = MainScan::toLocalPath(rootPath, file);
        QList<QString> regularExpression;
        QList<QString> regularExpressionsFrames;
        foreach(QString patternCentral, configuration.sequenceFramePatternList())
        {
            regularExpressionsFrames << generateRegularExpressionSequenceFrameFromString(QRegExp(patternCentral), relativeFilePath);
        }

        QStringList sequenceRegExps(configuration.subSequenceFramesPatternList());
        sequenceRegExps.append(configuration.framesPatternList());
        foreach(const QString& regExpFrames, regularExpressionsFrames)
        {
            regularExpression.append(MainScan::generateRegularExpressions(sequenceRegExps, regExpFrames));
        }

        //check if exist
        bool oneRegExpExist = false;
        for(int i = 0; i < regularExpression.size() ; ++i)
        {
            if(relativeFolderPathsByRegularExpressions.contains(regularExpression.at(i)))
            {
                QStringList value = relativeFolderPathsByRegularExpressions.value(regularExpression.at(i));
                if(value.contains(regularExpressionsFrames.at(i)))
                {
                    oneRegExpExist = true;
                    break;
                }                
            }
        }

        //insert
        if(!oneRegExpExist)
        {
            for(int i = 0; i < regularExpression.size() ; ++i)
            {
                QStringList list;
                if(relativeFolderPathsByRegularExpressions.contains(regularExpression.at(i)))
                {
                    list = relativeFolderPathsByRegularExpressions.value(regularExpression.at(i));
                }
                list << regularExpressionsFrames.at(i);
                relativeFolderPathsByRegularExpressions.insert(regularExpression.at(i), list);
            }
        } 
    }
    return relativeFolderPathsByRegularExpressions;
}
