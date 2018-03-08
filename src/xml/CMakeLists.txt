# Add XML_LIBRARY define for export.h
add_definitions(-DXML_LIBRARY)

file(GLOB HEADERS "${DML_SOURCE_DIR}/include/xml/*.h")
file(GLOB SOURCES "*.cpp")

add_library(dmlXml SHARED ${SOURCES} ${HEADERS})

find_package(libxml2 REQUIRED)
message(STATUS "LIBXML2_INCLUDE_DIR=${LIBXML2_INCLUDE_DIR}")
include_directories(${LIBXML2_INCLUDE_DIR})
message(STATUS "LIBXML2_LIBRARIES=${LIBXML2_LIBRARIES}")
link_directories(${LIBXML2_LIBRARIES})
target_link_libraries(dmlXml "${LIBXML2_LIBRARIES}\\libxml2.lib" dmlCore dmlConf)
