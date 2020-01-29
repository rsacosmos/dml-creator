cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(DML)

option(ENABLE_GUI "Enable the GUI support (requires: Qt5). Default is OFF." OFF)

if(ENABLE_GUI)
    # Qt5
    find_package(Qt5Core)
    find_package(Qt5Gui)
    find_package(Qt5Widgets)
    find_package(Qt5Xml)
    find_package(Qt5XmlPatterns)
    find_package(Qt5Multimedia)
    include_directories(
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Xml_INCLUDE_DIRS}
        ${Qt5XmlPatterns_INCLUDE_DIRS}
        ${Qt5Multimedia_INCLUDE_DIRS}
    )
    get_target_property(QtCore_location Qt5::Core LOCATION)
    get_target_property(QtGui_location Qt5::Gui LOCATION)
    get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
    get_target_property(QtXml_location Qt5::Xml LOCATION)
    get_target_property(QtXmlPatterns_location Qt5::XmlPatterns LOCATION)
    get_target_property(QtMultimedia_location Qt5::Multimedia LOCATION)
    get_target_property(QtNetwork_location Qt5::Network LOCATION)
    get_filename_component(Qt5Libs ${QtCore_location} PATH)
    set(Qt5PluginsDir "${Qt5Libs}/../plugins")
endif(ENABLE_GUI)

# Include and link
include_directories(${DML_SOURCE_DIR}/include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_DEBUG_POSTFIX "_debug")

# Subdirectories
add_subdirectory(src)
add_subdirectory(application)
