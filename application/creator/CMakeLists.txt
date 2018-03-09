file(GLOB HEADERS "*.h")
file(GLOB UIS "*.ui")
file(GLOB SOURCES "*.cpp")
file(GLOB QRC "*.qrc")

# As the UIS headers will be generated in the BINARY_DIR, we need to add it as an INCLUDE_DIRECTORY
include_directories(${CMAKE_CURRENT_BINARY_DIR})

qt5_wrap_ui(UI_HEADERS ${UIS})
qt5_wrap_cpp(MOC_SOURCES ${HEADERS})
qt5_add_resources(QRC_SOURCES ${QRC})
set(PLATFORM_SPECIFIC_CONTROL WIN32)

add_executable(Creator
    WIN32
    ${SOURCES} ${HEADERS}
    ${QRC} ${QRC_SOURCES}
    ${HEADERS} ${MOC_SOURCES}
    ${UIS} ${UI_HEADERS}
)
target_link_libraries(Creator
    Qt5::WinMain
    Qt5::Core
    Qt5::Widgets
    Qt5::Xml
    Qt5::XmlPatterns
    dmlScan
    dmlCore
    dmlConf
    dmlXml
)

# install(TARGETS Creator
    # RUNTIME DESTINATION ./bin 
    # LIBRARY DESTINATION ./bin
    # ARCHIVE DESTINATION ./bin
# )

# install(FILES ${QtCore_location} DESTINATION ./bin)
# install(FILES ${QtWidgets_location} DESTINATION ./bin)
# install(FILES ${QtXmlPatterns_location} DESTINATION ./bin)

# install(FILES ${Qt5PluginsDir}/platforms/qwindows.dll DESTINATION ./bin/platforms)
# install(FILES ${Qt5PluginsDir}/platforms/qoffscreen.dll DESTINATION ./bin/platforms)
# install(FILES ${Qt5PluginsDir}/platforms/qminimal.dll DESTINATION ./bin/platforms)

# install(FILES "config.xml" DESTINATION ./data)
# install(FILES "${DML_SOURCE_DIR}/doc/AFDI_standards.xsd" DESTINATION ./data)

# present the packaging option only if we have the cpack command defined (effectively >= 2.6.0)
# if(CMAKE_CPACK_COMMAND)
    # Set CPack options.
    # set(CPACK_PACKAGE_VERSION_MAJOR  ${DML_CREATOR_MAJOR_VERSION})
    # set(CPACK_PACKAGE_VERSION_MINOR  ${DML_CREATOR_MINOR_VERSION})
    # set(CPACK_PACKAGE_VERSION_PATCH  ${DML_CREATOR_PATCH_VERSION})

    # set(CPACK_PACKAGE_FILE_NAME "DML-Creator-$ENV{DML_CREATOR_VERSION_NAME}")
    # set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "AFDI\\\\DML\\\\DML-Creator")

    # include(CPack)
# endif()

