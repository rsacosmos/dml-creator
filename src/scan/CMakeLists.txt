ADD_DEFINITIONS(-DSCAN_LIBRARY)

SET(TARGET_NAME Scan)

SET(HEADER_PATH ${DML_SOURCE_DIR}/include/${TARGET_NAME})

SET(TARGET_MOC_HEADERS
    ${HEADER_PATH}/MainScan.h
    ${HEADER_PATH}/ScanShow.h
)

# Headers NOT needing any MOC pass
SET(TARGET_HEADERS
    ${TARGET_MOC_HEADERS}

    ${HEADER_PATH}/Export.h
    ${HEADER_PATH}/AudioScan.h
    ${HEADER_PATH}/VideoScan.h
    ${HEADER_PATH}/SubtitlesScan.h
    ${HEADER_PATH}/PreviewScan.h
)

SET(TARGET_SOURCES
    MainScan.cpp
    ScanShow.cpp
    AudioScan.cpp
    VideoScan.cpp
    SubtitlesScan.cpp
    PreviewScan.cpp
)

SETUP_SHARED(${TARGET_NAME})

LINK_INTERNAL(${TARGET_TARGETNAME} 
    ${TARGET_DEFAULT_PREFIX}Core
    ${TARGET_DEFAULT_PREFIX}Conf
)

LINK_WITH_VARIABLES(${TARGET_TARGETNAME}
    Qt5Core_LIBRARIES
    Qt5Gui_LIBRARIES
    Qt5Xml_LIBRARIES
    Qt5Multimedia_LIBRARIES
)

LINK_AGAINST_VLD_IF_NEEDED(${TARGET_TARGETNAME})


INSTALL(FILES ${QtCore_location} DESTINATION ./bin)
INSTALL(FILES ${QtGui_location} DESTINATION ./bin)
INSTALL(FILES ${QtXml_location} DESTINATION ./bin)
INSTALL(FILES ${QtMultimedia_location} DESTINATION ./bin)
INSTALL(FILES ${QtNetwork_location} DESTINATION ./bin)


# Install imageformats.
#INSTALL(FILES ${Qt5PluginsDir}/imageformats/qdds.dll DESTINATION ./plugins/imageformats)