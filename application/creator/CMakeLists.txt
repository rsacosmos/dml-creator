SET(TARGET_NAME
    Creator
)

# Headers needing a MOC pass (those with a Q_OBJECT macro)
SET(TARGET_MOC_HEADERS
    AudioWidget.h
    InformationWidget.h
    MainWindow.h
    SubtitlesWidget.h
    VideoWidget.h
    FramesWidget.h
    ProgressDialog.h
)

SET(TARGET_UIS
    AudioWidget.ui
    InformationWidget.ui
    MainWindow.ui
    ProgressDialog.ui
    VideoWidget.ui
    FramesWidget.ui
    SubtitlesWidget.ui
)

# Headers NOT needing any MOC pass
SET(TARGET_HEADERS
)

SET(TARGET_SOURCES
    AudioWidget.cpp
    InformationWidget.cpp
    main.cpp
    MainWindow.cpp
    SubtitlesWidget.cpp
    VideoWidget.cpp
    FramesWidget.cpp
    ProgressDialog.cpp
)

SET(TARGET_QRC
    resources/resources.qrc
)

# As the UIS headers will be generated in the BINARY_DIR, we need to add it as an INCLUDE_DIRECTORY
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT5_WRAP_UI(TARGET_UI_HEADERS ${TARGET_UIS})
QT5_WRAP_CPP(TARGET_MOC_SOURCES ${TARGET_MOC_HEADERS})
QT5_ADD_RESOURCES(TARGET_QRC_SOURCES ${TARGET_QRC})
SET(PLATFORM_SPECIFIC_CONTROL WIN32)
ADD_EXECUTABLE(${TARGET_NAME} ${PLATFORM_SPECIFIC_CONTROL}
               ${TARGET_HEADERS} ${TARGET_SOURCES}
               ${TARGET_QRC} ${TARGET_QRC_SOURCES}
               ${TARGET_QML}
               ${TARGET_MOC_HEADERS} ${TARGET_MOC_SOURCES}
               ${TARGET_UIS} ${TARGET_UI_HEADERS}
)
TARGET_LINK_LIBRARIES(${TARGET_NAME} Qt5::WinMain)
INSTALL(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ./bin 
        LIBRARY DESTINATION ./bin
        ARCHIVE DESTINATION ./bin)


TARGET_LINK_LIBRARIES(${TARGET_NAME}
    ${TARGET_DEFAULT_PREFIX}Scan
    ${TARGET_DEFAULT_PREFIX}Core
    ${TARGET_DEFAULT_PREFIX}Conf
    ${TARGET_DEFAULT_PREFIX}Xml
)

INSTALL(FILES ${QtCore_location} DESTINATION ./bin)
INSTALL(FILES ${QtWidgets_location} DESTINATION ./bin)
INSTALL(FILES ${QtXmlPatterns_location} DESTINATION ./bin)

INSTALL(FILES ${Qt5PluginsDir}/platforms/qwindows.dll DESTINATION ./bin/platforms)
INSTALL(FILES ${Qt5PluginsDir}/platforms/qoffscreen.dll DESTINATION ./bin/platforms)
INSTALL(FILES ${Qt5PluginsDir}/platforms/qminimal.dll DESTINATION ./bin/platforms)

INSTALL(FILES "config.xml" DESTINATION ./data)
INSTALL(FILES "${DML_SOURCE_DIR}/doc/AFDI_standards.xsd" DESTINATION ./data)

# present the packaging option only if we have the cpack command defined (effectively >= 2.6.0)
IF(CMAKE_CPACK_COMMAND)
        
        # Set CPack options.
        SET(CPACK_PACKAGE_VERSION_MAJOR  ${DML_CREATOR_MAJOR_VERSION})
        SET(CPACK_PACKAGE_VERSION_MINOR  ${DML_CREATOR_MINOR_VERSION})
        SET(CPACK_PACKAGE_VERSION_PATCH  ${DML_CREATOR_PATCH_VERSION})
       
        SET(CPACK_PACKAGE_FILE_NAME "DML-Creator-$ENV{DML_CREATOR_VERSION_NAME}")
        SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
        SET(CPACK_PACKAGE_INSTALL_DIRECTORY "AFDI\\\\DML\\\\DML-Creator")
    
        INCLUDE(CPack)
ENDIF()

