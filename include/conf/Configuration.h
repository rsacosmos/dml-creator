#ifndef CONF_CONFIGURATION_H
#define CONF_CONFIGURATION_H

#include "conf/Export.h"

#include <QList>
#include <QString>
#include <QStringList>
#include <QPair>
#include <QRegExp>
#include <QMap>

#include "core/Audio.h"
#include "core/FRames.h"

namespace dml
{

namespace conf
{

/** Implementation of the Configuration class. */
class CONF_EXPORT Configuration
{
public:    
    Configuration() {}


    typedef QPair<QRegExp, QString> PatternReplaceable;
    typedef QMap<dml::core::Audio::Speaker, QStringList> AudioTypeList;
    typedef QMap<dml::core::Frames::Type, QStringList> FramesTypeList;

    QStringList subSequenceFramesPatternList () const { return _subSequenceFramesPatternList; }
    void setSubSequenceFramesPatternList(const QStringList& subSequenceFramesPatternList) { _subSequenceFramesPatternList = subSequenceFramesPatternList; }

    QStringList ignoredFileList() const { return _ignoredFileList; }
    void setIgnoredFileList(const QStringList& ignoredFileList) { _ignoredFileList = ignoredFileList; }

    QStringList sequenceFramePatternList() const { return _sequenceFramePatternList; }
    void setSequenceFramePatternList(const QStringList& sequenceFramePatternList) { _sequenceFramePatternList = sequenceFramePatternList; }

    QStringList languagePatternList() const { return _languagePatternList; }
    void setLanguagePatternList(const QStringList& languagePatternList) { _languagePatternList = languagePatternList; }

    QStringList frameratePatternList() const { return _frameratePatternList; }
    void setFrameratePatternList(const QStringList& frameratePatternList) { _frameratePatternList = frameratePatternList; }

    QStringList audioPatternList() const { return _audioPatternList; }
    void setAudioPatternList(const QStringList& audioPatternList) { _audioPatternList = audioPatternList; }

    QStringList framesPatternList() const { return _framesPatternList; }
    void setFramesPatternList(const QStringList& framesPatternList) { _framesPatternList = framesPatternList; }

    AudioTypeList audioTypeList() const { return _audioTypeList; }
    void setAudioTypeList(const AudioTypeList& audioTypeList) { _audioTypeList = audioTypeList; }

    FramesTypeList framesTypeList() const { return _framesTypeList; }
    void setFramesTypeList(const FramesTypeList& framesTypeList) { _framesTypeList = framesTypeList; }

    QString subtitlesExtensionPattern() const { return _subtitlesExtensionPattern; }
    void setSubtitlesExtensionPattern(const QString& subtitlesExtensionPattern) { _subtitlesExtensionPattern = subtitlesExtensionPattern; }

    QString audioExtensionPattern() const { return _audioExtensionPattern; }
    void setAudioExtensionPattern(const QString& audioExtensionPattern) { _audioExtensionPattern = audioExtensionPattern; }

    QString videoExtensionPattern() const { return _videoExtensionPattern; }
    void setVideoExtensionPattern(const QString& videoExtensionPattern) { _videoExtensionPattern = videoExtensionPattern; }

    QString trailerExtensionPattern() const { return _trailerExtensionPattern; }
    void setTrailerExtensionPattern(const QString& trailerExtensionPattern) { _trailerExtensionPattern = trailerExtensionPattern; }

    QString subtitlesExtensionSupported() const { return _subtitlesExtensionSupported; }
    void setSubtitlesExtensionSupported(const QString& subtitlesExtensionSupported) { _subtitlesExtensionSupported = subtitlesExtensionSupported; }

    QString audioExtensionSupported() const { return _audioExtensionSupported; }
    void setAudioExtensionSupported(const QString& audioExtensionSupported) { _audioExtensionSupported = audioExtensionSupported; }

    QString videoImageExtensionSupported() const { return _videoImageExtensionSupported; }
    void setVideoImageExtensionSupported(const QString& videoImageExtensionSupported) { _videoImageExtensionSupported = videoImageExtensionSupported; }

    QString trailerExtensionSupported() const { return _trailerExtensionSupported; }
    void setTrailerExtensionSupported(const QString& trailerExtensionSupported) { _trailerExtensionSupported = trailerExtensionSupported; }

    QString stereoscopicTypes() const { return _stereoscopicTypes; }
    void setStereoscopicTypes(const QString& stereoscopicTypes) { _stereoscopicTypes = stereoscopicTypes; }

    QString speakersTypes() const { return _speakersTypes; }
    void setSpeakersTypes(const QString& speakersTypes) { _speakersTypes = speakersTypes; }

    QString stereoscopicTypeListRef() const { return _stereoscopicTypeListRef; }
    void setStereoscopicTypeListRef(const QString& stereoscopicTypeListRef) { _stereoscopicTypeListRef = stereoscopicTypeListRef; }

    QString speakerTypeListRef() const { return _speakerTypeListRef; }
    void setSpeakerTypeListRef(const QString& speakerTypeListRef) { _speakerTypeListRef = speakerTypeListRef; }

    QString defaultSubtitlesLanguage() const { return _defaultSubtitlesLanguage; }
    void setDefaultSubtitlesLanguage(const QString& defaultSubtitlesLanguage) { _defaultSubtitlesLanguage = defaultSubtitlesLanguage; }

    QString defaultAudioLanguage() const { return _defaultAudioLanguage; }
    void setDefaultAudioLanguage(const QString& defaultAudioLanguage) { _defaultAudioLanguage = defaultAudioLanguage; }

    double defaultFramerate() const { return _defaultFramerate; }
    void setDefaultFramerate(const double& defaultFramerate) { _defaultFramerate = defaultFramerate; }

    int defaultColorBitDepth() const { return _defaultColorBitDepth; }
    void setDefaultColorBitDepth(const int& defaultColorBitDepth) { _defaultColorBitDepth = defaultColorBitDepth; }

    int minVideoFramesCount() const { return _minVideoFramesCount; }
    void setMinVideoFramesCount(const int& minVideoFramesCount) { _minVideoFramesCount = minVideoFramesCount; }

    static std::string speakerToString(dml::core::Audio::Speaker type);
    static dml::core::Audio::Speaker speakerFromString(const std::string& value);

    static std::string framesTypeToString(dml::core::Frames::Type type);
    static dml::core::Frames::Type framesTypeFromString(const std::string& value);

private:
    QStringList                         _subSequenceFramesPatternList;

    QStringList                         _ignoredFileList;
    QStringList                         _sequenceFramePatternList;
    QStringList                         _languagePatternList;
    QStringList                         _frameratePatternList;
    QStringList                         _audioPatternList;
    QStringList                         _framesPatternList;
    
    AudioTypeList                       _audioTypeList;
    FramesTypeList                      _framesTypeList;

    QString                             _subtitlesExtensionPattern;
    QString                             _audioExtensionPattern;
    QString                             _videoExtensionPattern;
    QString                             _trailerExtensionPattern;

    QString                             _subtitlesExtensionSupported;
    QString                             _audioExtensionSupported;
    QString                      		_videoImageExtensionSupported;
    QString                             _trailerExtensionSupported;

    QString                             _stereoscopicTypes;
    QString                             _speakersTypes;

    QString                             _stereoscopicTypeListRef;
    QString                             _speakerTypeListRef;

    QString                             _defaultSubtitlesLanguage;
    QString                             _defaultAudioLanguage;

    double                              _defaultFramerate;

    int                                 _defaultColorBitDepth;
    int                                 _minVideoFramesCount;
}; // class Configuration

} // namespace conf

} // namespace dml

#endif // CONF_CONFIGURATION_H