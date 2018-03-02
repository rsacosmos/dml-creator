ADD_DEFINITIONS(-DCORE_LIBRARY)

SET(TARGET_NAME Core)

SET(HEADER_PATH ${DML_SOURCE_DIR}/include/${TARGET_NAME})

SET(TARGET_MOC_HEADERS

)

# Headers NOT needing any MOC pass
SET(TARGET_HEADERS
    ${TARGET_MOC_HEADERS}

    ${HEADER_PATH}/Export.h
    ${HEADER_PATH}/Audio.h
    ${HEADER_PATH}/Frames.h
    ${HEADER_PATH}/FrameSequence.h
    ${HEADER_PATH}/Information.h
    ${HEADER_PATH}/Show.h
    ${HEADER_PATH}/Subtitles.h
    ${HEADER_PATH}/Tools.h
    ${HEADER_PATH}/Video.h
)

SET(TARGET_SOURCES
    Audio.cpp
    Frames.cpp
    FrameSequence.cpp
    Information.cpp
    Show.cpp
    Subtitles.cpp
    Tools.cpp
    Video.cpp
)

SETUP_SHARED(${TARGET_NAME})

LINK_WITH_VARIABLES(${TARGET_NAME}

)

LINK_AGAINST_VLD_IF_NEEDED(${TARGET_NAME})
