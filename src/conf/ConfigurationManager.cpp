#include "conf/ConfigurationManager.h"

#include "conf/Configuration.h"
#include <QDomElement>

using namespace dml::conf;


void ConfigurationManager::loadConfigurationData(const QDomElement &config)
{
    _configuration.setIgnoredFileList(loadPatternList(config, "ignoredFileList"));    

    _configuration.setSubtitlesExtensionPattern(readDomElementValue(config, "subtitlesExtensionPattern"));
    _configuration.setAudioExtensionPattern(readDomElementValue(config, "audioExtensionPattern"));
    _configuration.setVideoExtensionPattern(readDomElementValue(config, "videoImageExtensionPattern"));
    _configuration.setTrailerExtensionPattern(readDomElementValue(config, "trailerExtensionPattern"));

    _configuration.setSubtitlesExtensionSupported(readDomElementValue(config, "subtitlesExtensionSupported"));
    _configuration.setAudioExtensionSupported(readDomElementValue(config, "audioExtensionSupported"));
    _configuration.setVideoImageExtensionSupported(readDomElementValue(config, "videoImageExtensionSupported"));
    _configuration.setTrailerExtensionSupported(readDomElementValue(config, "trailerExtensionSupported"));

    QDomElement defaultValueList = config.firstChildElement("defaultValueList");
    if (!defaultValueList.isNull())
    {
        _configuration.setDefaultSubtitlesLanguage(readDomElementValue(defaultValueList, "subtitlesLanguage"));
        _configuration.setDefaultAudioLanguage(readDomElementValue(defaultValueList, "audioLanguage"));
        _configuration.setDefaultFramerate(readDomElementValue(defaultValueList, "framerate").toDouble());
        _configuration.setDefaultColorBitDepth(readDomElementValue(defaultValueList, "colorBitDepth").toInt());
    }

    _configuration.setMinVideoFramesCount(readDomElementValue(config, "minVideoFramesCount").toInt());

    _configuration.setLanguagePatternList(loadPatternList(config, "languagePatternList"));
    _configuration.setFrameratePatternList(loadPatternList(config, "frameratePatternList"));

    QDomElement frames = config.firstChildElement("frames");
    loadFramesConfiguration(frames);

    QDomElement audio = config.firstChildElement("audio");
    loadAudioConfiguration(audio);
}

void ConfigurationManager::loadFramesConfiguration(const QDomElement& frames)
{
    if (!frames.isNull())
    {
        _configuration.setSequenceFramePatternList(loadPatternList(frames, "sequenceFramePatternList"));
        _configuration.setSubSequenceFramesPatternList(loadPatternList(frames, "subSequenceFramesPatternList"));

        QDomElement stereoscopicTypeList = frames.firstChildElement("stereoscopicTypeList");
        if (!stereoscopicTypeList.isNull())
        {
            QString stereoscopicTypes;
            _configuration.setStereoscopicTypeListRef(stereoscopicTypeList.attribute("ref"));

            QDomElement type = stereoscopicTypeList.firstChildElement("type");
            while (!type.isNull())
            {
                QString ref  = type.attribute("ref");
                QString value = type.text();

                dml::core::Frames::Type framesType = dml::conf::Configuration::framesTypeFromString(ref.toStdString());

                dml::conf::Configuration::FramesTypeList framesTypeList = _configuration.framesTypeList();
                QStringList list = framesTypeList[framesType];
                list << value;

                if(!stereoscopicTypes.isEmpty())
                {
                    stereoscopicTypes += "|";
                }
                stereoscopicTypes += value;

                framesTypeList[framesType] = list;

                _configuration.setFramesTypeList(framesTypeList);

                type = type.nextSiblingElement("type");
            }
            _configuration.setStereoscopicTypes(stereoscopicTypes);
        }
        QStringList framesPatternList = loadPatternList(frames, "framesPatternList");
        QStringList framesPatternListReplaced;
        foreach(const QString& framesPattern, framesPatternList)
        {
            framesPatternListReplaced << QString(framesPattern).replace(_configuration.stereoscopicTypeListRef(), _configuration.stereoscopicTypes());
        }
        _configuration.setFramesPatternList(framesPatternListReplaced);
    }     
}

void ConfigurationManager::loadAudioConfiguration(const QDomElement& audio)
{
    if (!audio.isNull())
    {
        QDomElement audioTypeList = audio.firstChildElement("audioTypeList");
        if (!audioTypeList.isNull())
        {
            QString speakerTypes;

            _configuration.setSpeakerTypeListRef(audioTypeList.attribute("ref"));

            QDomElement type = audioTypeList.firstChildElement("type");
            while (!type.isNull())
            {
                QString ref  = type.attribute("ref");
                QString value = type.text();

                dml::core::Audio::Speaker speakerType = dml::conf::Configuration::speakerFromString(ref.toStdString());

                dml::conf::Configuration::AudioTypeList audioTypeList = _configuration.audioTypeList();
                QStringList list = audioTypeList[speakerType];
                list << value;

                if(!speakerTypes.isEmpty())
                {
                    speakerTypes += "|";
                }
                speakerTypes += value;

                audioTypeList[speakerType] = list;

                _configuration.setAudioTypeList(audioTypeList);

                type = type.nextSiblingElement("type");
            }
            _configuration.setSpeakersTypes(speakerTypes);
        }
        QStringList audioPatternList = loadPatternList(audio, "audioPatternList");
        QStringList audioPatternListReplaced;
        foreach(const QString& framesPattern, audioPatternList)
        {
            audioPatternListReplaced << QString(framesPattern).replace(_configuration.speakerTypeListRef(), _configuration.speakersTypes());
        }
        _configuration.setAudioPatternList(audioPatternListReplaced);
    }     
}

QStringList ConfigurationManager::loadPatternList(const QDomElement &root, const QString &childName) const
{
    QStringList list;

    QDomElement child = root.firstChildElement(childName);
    if (child.isNull())
        return list;

    QDomElement regExp = child.firstChildElement("regExp");
    while (!regExp.isNull())
    {
        QString value = regExp.text();
        list << value;

        regExp = regExp.nextSiblingElement("regExp");
    }

    return list;
}

QString ConfigurationManager::readDomElementValue(const QDomElement &root, const QString &elementName) const
{
    QDomElement child = root.firstChildElement(elementName);
    if (child.isNull())
        return "";

    return child.text();
}
