# Add CORE_LIBRARY define for export.h
add_definitions(-DCORE_LIBRARY)

file(GLOB HEADERS "${DML_SOURCE_DIR}/include/core/*.h")
file(GLOB SOURCES "*.cpp")

add_library(dmlCore SHARED ${SOURCES} ${HEADERS})
