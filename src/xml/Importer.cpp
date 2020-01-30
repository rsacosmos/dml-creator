#include <xml/Importer.h>
#include <xml/Element.h>
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

// TODO using namespace dml::xml is accepted here.

dml::core::Show dml::xml::Importer::fromDml(const std::string& dml)
{
    Element showElement = Element::fromString(dml);
    return fromDml(showElement);
}

dml::core::Show dml::xml::Importer::fromDml(const Element& showElement)
{
    core::Show show;

    core::VideoList videoList;
    for (Element videoElement = showElement.firstChildElement(VIDEO_TAGNAME);
        !videoElement.isNull();
        videoElement = videoElement.nextSiblingElement(VIDEO_TAGNAME))
    {
        core::Video video = fromDml(videoElement, core::Video());
        videoList.push_back(video);
    }
    show.setVideoList(videoList);

    core::AudioList audioList;
    for (Element audioElement = showElement.firstChildElement(AUDIO_TAGNAME);
        !audioElement.isNull();
        audioElement = audioElement.nextSiblingElement(AUDIO_TAGNAME))
    {
        core::Audio audio = fromDml(audioElement, core::Audio());
        audioList.push_back(audio);
    }
    show.setAudioList(audioList);

    core::SubtitlesList subtitlesList;
    for (Element subtitlesElement = showElement.firstChildElement(SUBTITLES_TAGNAME);
        !subtitlesElement.isNull();
        subtitlesElement = subtitlesElement.nextSiblingElement(SUBTITLES_TAGNAME))
    {
        core::Subtitles subtitles = fromDml(subtitlesElement, core::Subtitles());
        subtitlesList.push_back(subtitles);
    }
    show.setSubtitlesList(subtitlesList);

    Element informationElement = showElement.firstChildElement(INFORMATION_TAGNAME);
    core::Information information;

    std::string producer = informationElement.firstChildElement(PRODUCER_TAGNAME).text();
    information.setProducer(producer);

    std::string copyright = informationElement.firstChildElement(COPYRIGHT_TAGNAME).text();
    information.setCopyright(copyright);

    std::string publicationDate = informationElement.firstChildElement(PUBLICATIONDATE_TAGNAME).text();
    if (!publicationDate.empty())
    {
        information.setPublicationDate(publicationDate);
    }

    std::string description = informationElement.firstChildElement(DESCRIPTION_TAGNAME).text();
    information.setDescription(description);

    show.setInformation(information);

    return show;
}

dml::core::Audio dml::xml::Importer::fromDml(const Element& audioElement, const dml::core::Audio& defaultValue)
{
    core::Audio audio(defaultValue);

    audio.setPictureFrameRate(Tools::stringToDouble(audioElement.firstChildElement(PICTUREFRAMERATE_TAGNAME).text()));
    audio.setLanguage(audioElement.firstChildElement(LANGUAGE_TAGNAME).text());
    audio.setAudioName(audioElement.firstChildElement(AUDIONAME_TAGNAME).text());

    core::Audio::ChannelMap channelMap;
    for (Element channelElement=audioElement.firstChildElement(CHANNEL_TAGNAME);
         !channelElement.isNull();
         channelElement=channelElement.nextSiblingElement(CHANNEL_TAGNAME))
    {
        core::Audio::Speaker channelType = conf::Configuration::speakerFromString(channelElement.firstChildElement(SPEAKER_TAGNAME).text());
        std::string path = channelElement.firstChildElement(PATH_TAGNAME).text();

        channelMap[channelType] = path;
    }
    audio.setChannelMap(channelMap);

    return audio;
}

dml::core::Video dml::xml::Importer::fromDml(const Element& videoElement, const dml::core::Video& defaultValue)
{
    dml::core::Video video(defaultValue);

    std::string videoname = videoElement.firstChildElement(VIDEONAME_TAGNAME).text();
    video.setVideoName(videoname);

    std::string framerate = videoElement.firstChildElement(FRAMERATE_TAGNAME).text();
    video.setFramerate(Tools::stringToDouble(framerate));

    std::string framecount = videoElement.firstChildElement(FRAMECOUNT_TAGNAME).text();
    video.setFramecount(Tools::stringToInt(framecount));

    std::string resolution = videoElement.firstChildElement(RESOLUTION_TAGNAME).text();
    video.setResolution(Tools::stringToInt(resolution));

    std::string colorbitdepth = videoElement.firstChildElement(COLORBITDEPTH_TAGNAME).text();
    video.setColorBitsDepth(Tools::stringToInt(colorbitdepth));

    std::string preview = videoElement.firstChildElement(PREVIEW_TAGNAME).text();
    video.setPreview(preview);

    for (Element framesElement=videoElement.firstChildElement(FRAMES_TAGNAME);
        !framesElement.isNull();
        framesElement=framesElement.nextSiblingElement(FRAMES_TAGNAME))
    {
        dml::core::Frames frames = fromDml(framesElement, dml::core::Frames());

        std::vector<dml::core::Frames> framesList = video.framesList();
        framesList.push_back(frames);
        video.setFramesList(framesList);
    }

    return video;
}

dml::core::Frames dml::xml::Importer::fromDml(const Element& framesElement, const dml::core::Frames& defaultValue)
{
    dml::core::Frames frames(defaultValue);


    std::string type = framesElement.firstChildElement(STEREOSCOPIC_TAGNAME).text();
    frames.setType(dml::core::Frames::typeFromString(type));

    std::vector<dml::core::FrameSequence> frameSequences;
    for (Element frameSequenceElement = framesElement.firstChildElement(FRAMESEQUENCE_TAGNAME);
        !frameSequenceElement.isNull();
        frameSequenceElement = frameSequenceElement.nextSiblingElement(FRAMESEQUENCE_TAGNAME))
    {
        dml::core::FrameSequence frameSequence = fromDml(frameSequenceElement, dml::core::FrameSequence());
        frameSequences.push_back(frameSequence);
    }
    frames.setFrameSequences(frameSequences);

    return frames;
}

dml::core::FrameSequence dml::xml::Importer::fromDml(const Element& frameSequenceElement, const dml::core::FrameSequence& defaultValue)
{
    dml::core::FrameSequence frameSequence(defaultValue);

    std::string path = frameSequenceElement.firstChildElement(PATH_TAGNAME).text();
    frameSequence.setFramespath(path);

    std::string firstframe = frameSequenceElement.firstChildElement(FIRSTFRAME_TAGNAME).text();
    frameSequence.setFirstframe(Tools::stringToInt(firstframe));

    std::string lastframe = frameSequenceElement.firstChildElement(LASTFRAME_TAGNAME).text();
    frameSequence.setLastframe(Tools::stringToInt(lastframe));

    return frameSequence;
}

dml::core::Subtitles dml::xml::Importer::fromDml(const Element& subtitlesElement, const dml::core::Subtitles& defaultValue)
{
    core::Subtitles subtitles(defaultValue);

    std::string path = subtitlesElement.firstChildElement(PATH_TAGNAME).text();
    subtitles.setPath(path);

    std::string language = subtitlesElement.firstChildElement(LANGUAGE_TAGNAME).text();
    subtitles.setLanguage(language);

    return subtitles;
}
