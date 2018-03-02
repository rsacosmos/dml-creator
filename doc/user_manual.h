/*!
  * \mainpage Dml Creator SDK reference guide
  * Dml Creator SDK is provided by R.S.A. COSMOS under the LGPL-v3 licence based on Association of Fulldome Innovators (AFDI) standards.
  * Written in c++ and Qt-5 for graphical part, this project permits to represent a fulldome show in a Dml file.
  *
  * \section section1 Dml format description
  * This implementation of Dml SDK is based on version 1 of standard that can be read here: http://imersa.org/images/standards/AFDI%20standards%20v1.0.pdf
  *
  * \section section2 Installing Dml Creator SDK
  * <b>Warning:</b> this section is only for Microsoft Windows platforms.<br /><br/>
  * Dml Creator SDK is provided with a NSIS installer that installs on system:<br />
  * <ul><li>Dml Createor applications (binaries),</li>
  * <li>Dml Creator SDK (c++ headers files & static libraries for Microsoft Windows (VS 2010), Mac OS X (10.11) & Linux Ubuntu (14.04),</li>
  * <li>Dml Creator SDK documentation.</li></ul>
  *
  * \section section3 Compiling Dml Creator SDK
  * To compile Dml Creator SDK :
  * <ul>
  * <li>Install c++ compiler,</li>
  * <li>Install Qt-5,</li>
  * <li>Install CMake,</li>
  * <li>Generate project with CMake,</li>
  * <li>Build project.</li>
  * </ul>
  *
  * \section section4 Using Dml Creator SDK
  * Dml Creator SDK provides :
  * <ul>
  * <li>c++ headers,</li>
  * <li>Microsoft VS 2010 static librairies (32 bits),</li>
  * <li>Mac OS X 10.11 static libraries (64 bits),</li>
  * <li>Linux Ubuntu 14.04 static libraries (64 bits).</li>
  * </ul>
  * You must copy headers & static libraries to the folder of your project.
  *
  * \section section5 Using Dml Creator application
  * Dml Creator application can open or create Dml file. Import function automatically fills Dml structure with found files.
  *
  * \subsection subsection1 Graphical Dml Creator application
  * Graphical application provided by Dml Creator SDK permits to open/create Dml files.<br />
  * The menu entry File > Import scan provided path to automatically create Dml file.
  *
  * \subsection subsection2 Command-line Dml Creator application
  * Command-line usage:<br />
  * \code dml-creator-console -i {root_path} - c {config_file} -o {output} \endcode
  * where:
  * <ul>
  * <li>root_path is the path where fulldome show must be found (recursively),</li>
  * <li>config_file is the full filename of configuration file,</li>
  * <li>output is the full Dml filename writtent by application.</li>
  * </ul>
  *
  * \subsection subsection3 Configuration file
  * Configuration file content is described below:
  * <table>
  * <caption id="multi_row">Configuration file structure</caption>
  * <tr><th>Node<th>Cardinality<th>Description
  * <tr><td>dmlScan<td>[1..1]<td>Root node
  * <tr><td>&nbsp;&nbsp;&nbsp;ignoredFileList<td>[0..1]<td>Ignored files during scan operations. File is ignored if one or more regular expression matches.
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;regExp<td>[0..*]<td>Regular expression for ignored files
  * <tr><td>&nbsp;&nbsp;&nbsp;languagePatternList<td>[0..1]<td>Regular expressions list that permit to get language from filename.
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;regExp<td>[0..*]<td>Regular expression for capturing language. Must contain only one capturing text.
  * <tr><td>&nbsp;&nbsp;&nbsp;audioTypePatternList<td>[0..1]<td>List that declares all audio type patterns.
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pattern<td>[0..*]<td>Audio type pattern (e.g. LF)
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pattern\@ref<td>[1..1]<td>Audio type (e.g. SPEAKER_FRONT_LEFT)
  * <tr><td>&nbsp;&nbsp;&nbsp;trailerPatternList<td>[0..1]<td>Regular expressions list that determines if provided filename is a preview video file.
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;regExp<td>[0..*]<td>Pattern for preview video file.
  * <tr><td>&nbsp;&nbsp;&nbsp;audioPatternList<td>[0..1]<td>Regular expressions list that determines if provided filename is an audio file.
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;regExp<td>[0..*]<td>Pattern for audio file.
  * <tr><td>&nbsp;&nbsp;&nbsp;pictureFrameRatePatternList<td>[0..1]<td>Regular expressions list that permit to get picture framerate from filename.
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;regExp<td>[0..*]<td>Regular expression for capturing framerate. Must contain only one capturing text.
  * <tr><td>&nbsp;&nbsp;&nbsp;subtitleExtensionPattern<td>[1..1]<td>Subtitle filename pattern.
  * <tr><td>&nbsp;&nbsp;&nbsp;audioExtensionPattern<td>[1..1]<td>Audio filename pattern.
  * <tr><td>&nbsp;&nbsp;&nbsp;videoImageExtensionPattern<td>[1..1]<td>Video frames filenames pattern.
  * <tr><td>&nbsp;&nbsp;&nbsp;defaultValueList<td>[1..1]<td>Default value list.
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;subtitleLanguage<td>[1..1]<td>Default subtitle language.
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;audioLanguage<td>[1..1]<td>Default audio language.
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;framerate<td>[1..1]<td>Default video framerate (in frames per second).
  * <tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pictureFrameRate<td>[1..1]<td>Default video framerate for associated audio (in frames per second).
  * <tr><td>&nbsp;&nbsp;&nbsp;minVideoFramesCount<td>[1..1]<td>Minimal frames number into a directory to consider that directory as containing video frames.
  * <tr><td>&nbsp;&nbsp;&nbsp;imageResolutionCheck<td>[0..1]<td>Must be <b>true</b> or <b>false</b>. If <b>true</b>, directories that contains images that are not on the same resolution are skip.
  * <tr><td>&nbsp;&nbsp;&nbsp;videoName<td>[1..1]<td>Regular expression that returns video name from video full filename. Must contain only one capturing text.
  * </table>
  *
  * Configuration file example:
  * \code
<dmlScan>
    <ignoredFileList>
        <regExp>^.*Thumbs.*</regExp>
    </ignoredFileList>
    <languagePatternList>
        <regExp>.*_(..)_.*</regExp>
        <regExp>.*\.(..)\..*</regExp>
    </languagePatternList>
    <audioTypePatternList>
        <pattern ref="SPEAKER_FRONT_LEFT">LF</pattern>
    </audioTypePatternList>
    <trailerPatternList>
        <regExp>^.*Trailer.*</regExp>
    </trailerPatternList>
    <audioPatternList>
        <regExp>.*_(..)_.*</regExp>
    </audioPatternList>
    <pictureFrameRatePatternList>
        <regExp>.*_(..)fps_.*</regExp>
    </pictureFrameRatePatternList>

    <subtitleExtensionPattern>^.*\.srt$</subtitleExtensionPattern>
    <audioExtensionPattern>^.*\.(ac3|wav|mp3)$</audioExtensionPattern>
    <videoImageExtensionPattern>^.*\.(jpg|jpeg)$</videoImageExtensionPattern>
    
    <defaultValueList>
        <subtitleLanguage>en</subtitleLanguage>
        <audioLanguage>en</audioLanguage>
        <framerate>24</framerate>
        <pictureFrameRate>24</pictureFrameRate>
    </defaultValueList>
    
    <minVideoFramesCount>100</minVideoFramesCount>
    <imageResolutionCheck>true</imageResolutionCheck>
    <videoName>/*(.*)/[.^/]*</videoName>
</dmlScan>
\endcode
  */