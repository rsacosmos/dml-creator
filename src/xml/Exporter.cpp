#include <xml/Exporter.h>
#include <xml/Tools.h>

#include "conf/Configuration.h"

static const std::string INFORMATION_TAGNAME("information");
static const std::string PRODUCER_TAGNAME("producer");
static const std::string COPYRIGHT_TAGNAME("copyright");
static const std::string PUBLICATIONDATE_TAGNAME("publicationdate");
static const std::string DESCRIPTION_TAGNAME("description");
static const std::string AUDIO_TAGNAME("audio");
static const std::string AUDIONAME_TAGNAME("audioname");
static const std::string PICTUREFRAMERATE_TAGNAME("pictureframerate");
static const std::string LANGUAGE_TAGNAME("language");
static const std::string SPEAKER_TAGNAME("speaker");
static const std::string PATH_TAGNAME("path");
static const std::string CHANNEL_TAGNAME("channel");
static const std::string FRAMES_TAGNAME("frames");
static const std::string STEREOSCOPIC_TAGNAME("stereoscopic");
static const std::string FRAMESEQUENCE_TAGNAME("framesequence");
static const std::string FIRSTFRAME_TAGNAME("firstframe");
static const std::string LASTFRAME_TAGNAME("lastframe");
static const std::string SHOW_TAGNAME("show");
static const std::string SUBTITLES_TAGNAME("subtitles");
static const std::string RESOLUTION_TAGNAME("resolution");
static const std::string COLORBITDEPTH_TAGNAME("colorbitdepth");
static const std::string PREVIEW_TAGNAME("preview");
static const std::string VIDEO_TAGNAME("video");
static const std::string VIDEONAME_TAGNAME("videoname");
static const std::string FRAMERATE_TAGNAME("framerate");
static const std::string FRAMECOUNT_TAGNAME("framecount");

dml::xml::Element dml::xml::Exporter::toDml(const dml::core::Audio& audio)
{
    Element audioElement(AUDIO_TAGNAME);

    Element audionameElement(AUDIONAME_TAGNAME);
    std::string audioName = audio.audioName();
    if (!audioName.empty()) audionameElement.appendText(Tools::escapeText(audioName));
    audioElement.appendElement(audionameElement);

    double pictureframerate = audio.pictureFrameRate();
    Element pictureframerateElement(PICTUREFRAMERATE_TAGNAME);
    pictureframerateElement.appendText(Tools::doubleToString(pictureframerate));
    audioElement.appendElement(pictureframerateElement);

    std::string language = audio.language();
    Element languageElement(LANGUAGE_TAGNAME);
    if (!language.empty()) languageElement.appendText(language);
    audioElement.appendElement(languageElement);

    core::Audio::ChannelMap channelMap = audio.channelMap();
    for (core::Audio::ChannelMap::const_iterator it=channelMap.begin() ; it!=channelMap.end(); it++)
    {
        core::Audio::Speaker type = it->first;
        const std::string& path = it->second;

        if (!path.empty())
        {
            Element speakerElement(SPEAKER_TAGNAME);

            // FIXME Remove the 'conf' dependancy by moving speakers in core
            speakerElement.appendText(dml::conf::Configuration::speakerToString(type));

            Element pathElement(PATH_TAGNAME);
            pathElement.appendText(Tools::escapeText(path));

            Element channelElement(CHANNEL_TAGNAME);
            channelElement.appendElement(speakerElement);
            channelElement.appendElement(pathElement);

            audioElement.appendElement(channelElement);
        }
    }

    return audioElement;
}

dml::xml::Element dml::xml::Exporter::toDml(const dml::core::Frames& frames)
{
    Element framesElement(FRAMES_TAGNAME);

    core::Frames::Type type = frames.type();
    if (type!=core::Frames::Video2d)
    {
        Element stereoscopicElement(STEREOSCOPIC_TAGNAME);
        stereoscopicElement.appendText(dml::core::Frames::typeToString(type));
        framesElement.appendElement(stereoscopicElement);
    }

    std::vector<dml::core::FrameSequence> frameSequences = frames.frameSequences();
    for (unsigned int frameSequenceIndex=0 ; frameSequenceIndex<frameSequences.size(); frameSequenceIndex++)
    {
        const core::FrameSequence& frameSequence = frameSequences.at(frameSequenceIndex);
        framesElement.appendElement(toDml(frameSequence));
    }

    return framesElement;
}

dml::xml::Element dml::xml::Exporter::toDml(const dml::core::FrameSequence& frameSequence)
{
    Element framesequenceElement(FRAMESEQUENCE_TAGNAME);

    std::string framespath = frameSequence.framespath();
    Element pathElement(PATH_TAGNAME);
    pathElement.appendText(Tools::escapeText(framespath));
    framesequenceElement.appendElement(pathElement);

    int firstframe = frameSequence.firstframe();
    Element firstframeElement(FIRSTFRAME_TAGNAME);
    firstframeElement.appendText(Tools::intToString(firstframe));
    framesequenceElement.appendElement(firstframeElement);

    int lastframe = frameSequence.lastframe();
    Element lastframeElement(LASTFRAME_TAGNAME);
    lastframeElement.appendText(Tools::intToString(lastframe));
    framesequenceElement.appendElement(lastframeElement);

    return framesequenceElement;
}


dml::xml::Element dml::xml::Exporter::toDml(const dml::core::Show& show)
{
    Element showElement(SHOW_TAGNAME);

    const core::VideoList& videoList = show.videoList();
    for (unsigned int videoIndex=0; videoIndex<videoList.size(); ++videoIndex)
    {
        const core::Video& video = videoList.at(videoIndex);
        showElement.appendElement(toDml(video));
    }

    const core::AudioList& audioList = show.audioList();
    for (unsigned int audioIndex=0; audioIndex<audioList.size(); ++audioIndex)
    {
        core::Audio audio = audioList.at(audioIndex);
        showElement.appendElement(toDml(audio));
    }

    const core::SubtitlesList& subtitlesList = show.subtitlesList();
    for (unsigned int subtitlesIndex=0; subtitlesIndex<subtitlesList.size(); ++subtitlesIndex)
    {
        dml::core::Subtitles subtitles = subtitlesList.at(subtitlesIndex);
        showElement.appendElement(toDml(subtitles));
    }

    Element informationElement(INFORMATION_TAGNAME);

    Element producerElement(PRODUCER_TAGNAME);
    std::string producer = show.information().producer();
    if (!producer.empty()) producerElement.appendText(producer);
    informationElement.appendElement(producerElement);

    Element copyrightElement(COPYRIGHT_TAGNAME);
    std::string copyright = show.information().copyright();
    if (!copyright.empty()) copyrightElement.appendText(copyright);
    informationElement.appendElement(copyrightElement);

    bool hasPublicationDate = show.information().hasPublicationDate();
    if (hasPublicationDate)
    {
        Element publicationDateElement(PUBLICATIONDATE_TAGNAME);
        std::string publicationDate = show.information().publicationDate();
        if (!publicationDate.empty()) publicationDateElement.appendText(publicationDate);
        informationElement.appendElement(publicationDateElement);
    }

    Element descriptionElement(DESCRIPTION_TAGNAME);
    std::string description = show.information().description();
    if (!description.empty()) descriptionElement.appendText(description);
    informationElement.appendElement(descriptionElement);

    showElement.appendElement(informationElement);

    return showElement;
}

dml::xml::Element dml::xml::Exporter::toDml(const dml::core::Subtitles& subtitles)
{
    std::string path = subtitles.path();
    Element pathElement(PATH_TAGNAME);
    pathElement.appendText(Tools::escapeText(path));

    std::string language = subtitles.language();
    Element languageElement(LANGUAGE_TAGNAME);
    pathElement.appendText(language);

    Element subtitlesElement(SUBTITLES_TAGNAME);
    subtitlesElement.appendElement(pathElement);
    subtitlesElement.appendElement(languageElement);
    return subtitlesElement;
}

dml::xml::Element dml::xml::Exporter::toDml(const dml::core::Video& video)
{
    Element videoElement(VIDEO_TAGNAME);

    std::string videoName = video.videoName();
    Element videoNameElement(VIDEONAME_TAGNAME);
    videoNameElement.appendText(Tools::escapeText(videoName));
    videoElement.appendElement(videoNameElement);

    int framerate = video.framerate();
    Element framerateElement(FRAMERATE_TAGNAME);
    framerateElement.appendText(Tools::intToString(framerate));
    videoElement.appendElement(framerateElement);

    int framecount = video.framecount();
    Element framecountElement(FRAMECOUNT_TAGNAME);
    framecountElement.appendText(Tools::intToString(framecount));
    videoElement.appendElement(framecountElement);

    int resolution = video.resolution();
    if (resolution>0)
    {
        Element resolutionElement(RESOLUTION_TAGNAME);
        resolutionElement.appendText(Tools::intToString(resolution));
        videoElement.appendElement(resolutionElement);
    }

    int colorBitsDepth = video.colorBitsDepth();
    if (colorBitsDepth>0)
    {
        Element colorbitElement(COLORBITDEPTH_TAGNAME);
        colorbitElement.appendText(Tools::intToString(colorBitsDepth));
        videoElement.appendElement(colorbitElement);
    }

    std::string preview = video.preview();
    Element previewElement(PREVIEW_TAGNAME);
    if (!preview.empty()) previewElement.appendText(Tools::escapeText(preview));
    videoElement.appendElement(previewElement);

    core::FramesList framesList = video.framesList();
    for (unsigned int framesIndex=0 ; framesIndex<framesList.size(); framesIndex++)
    {
        const core::Frames& frames = framesList.at(framesIndex);
        videoElement.appendElement(toDml(frames));
    }

    return videoElement;
}

