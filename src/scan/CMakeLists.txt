# Add SCAN_LIBRARY define for export.h
add_definitions(-DSCAN_LIBRARY)

file(GLOB HEADERS "${DML_SOURCE_DIR}/include/scan/*.h")
file(GLOB SOURCES "*.cpp")

add_library(dmlScan SHARED ${SOURCES} ${HEADERS})

target_link_libraries(dmlScan
    Qt5::Core
)
