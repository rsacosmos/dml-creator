# Add SCAN_LIBRARY define for export.h
add_definitions(-DSCAN_LIBRARY)

set(HEADERS "${DML_SOURCE_DIR}/include/scan/AudioScan.h"
            "${DML_SOURCE_DIR}/include/scan/PreviewScan.h"
            "${DML_SOURCE_DIR}/include/scan/SubtitlesScan.h"
            "${DML_SOURCE_DIR}/include/scan/VideoScan.h")
set(QT_HEADERS "${DML_SOURCE_DIR}/include/scan/MainScan.h"
               "${DML_SOURCE_DIR}/include/scan/ScanShow.h")
file(GLOB SOURCES "*.cpp")

QT5_WRAP_CPP(QT_SOURCES ${QT_HEADERS})

add_library(dmlScan SHARED ${SOURCES} ${QT_SOURCES} ${HEADERS} ${QT_HEADERS})

target_link_libraries(dmlScan
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
    Qt5::Multimedia
    dmlConf
    dmlCore
)
