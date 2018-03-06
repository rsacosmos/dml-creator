# Add CONF_LIBRARY define for export.h
add_definitions(-DCONF_LIBRARY)

file(GLOB HEADERS "${DML_SOURCE_DIR}/include/conf/*.h")
file(GLOB SOURCES "*.cpp")

add_library(dmlConf SHARED ${SOURCES} ${HEADERS})

target_link_libraries(dmlConf
    Qt5::Core
    Qt5::Xml
)

