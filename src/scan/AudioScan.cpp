#include <scan/AudioScan.h>

#include <core/Audio.h>
#include <scan/MainScan.h>

#include <QRegExp>

using namespace dml::scan;


bool AudioScan::isAudio(const dml::conf::Configuration& configuration, const QString &filename)
{
    QRegExp exp(configuration.audioExtensionPattern());
    return exp.exactMatch(filename);
}

QString AudioScan::refactoringStereoscopicTypesPath(const dml::conf::Configuration& configuration, QString path)
{
    return path.replace(configuration.speakersTypes(), configuration.speakerTypeListRef());
}

dml::core::Audio::Speaker AudioScan::getAudioType(const dml::conf::Configuration& configuration, const QString& pattern, const QString &filename)
{
    QRegExp exp(pattern);
    exp.indexIn(filename);

    QStringList texts = exp.capturedTexts();
    texts.removeDuplicates();

    dml::conf::Configuration::AudioTypeList audioTypeList = configuration.audioTypeList();
    for (dml::conf::Configuration::AudioTypeList::const_iterator it=audioTypeList.begin() ; it!=audioTypeList.end() ; it++)
    {
        dml::core::Audio::Speaker type = it.key();

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
    return dml::core::Audio::StereoMix;
}

QMap<QString, QStringList> AudioScan::generateAllRegularExpressions(const dml::conf::Configuration& configuration, const QString& rootPath, const QStringList &files)
{
    QMap<QString, QStringList> relativeFolderPathsByRegularExpressions;
    foreach(const QString &file, files)
    {
        if (!isAudio(configuration, file))
            continue;

        QString relativeFilePath = MainScan::toLocalPath(rootPath, file);

        QString regularExpression = MainScan::generateRegularExpressions(configuration.audioPatternList(), relativeFilePath);
        QStringList filePaths;
        //check if exist
        if(relativeFolderPathsByRegularExpressions.contains(regularExpression))
        {
            filePaths = relativeFolderPathsByRegularExpressions.value(regularExpression);
        }
        //insert
        filePaths << relativeFilePath;
        relativeFolderPathsByRegularExpressions.insert(regularExpression, filePaths);
    }
    return relativeFolderPathsByRegularExpressions;
}
