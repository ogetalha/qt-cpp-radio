cmake_minimum_required(VERSION 3.16)

project(qtradio VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
# mpg123
pkg_check_modules(MPG123 REQUIRED IMPORTED_TARGET libmpg123)

# PortAudio
pkg_check_modules(PORTAUDIO REQUIRED IMPORTED_TARGET portaudio-2.0)
qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appqtradio
    main.cpp
    radiostatus.cpp
    radiostatus.h
)

qt_add_qml_module(appqtradio
    URI qtradio
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES radiostatus.h radiostatus.cpp
        RESOURCES README.md
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqtradio PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqtradio
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appqtradio PRIVATE
${CURL_INCLUDE_DIRS}
${MPG123_INCLUDE_DIRS}
${PORTAUDIO_INCLUDE_DIRS}
)

target_link_libraries(appqtradio
    PRIVATE Qt6::Quick
    ${CURL_LIBRARIES}
    PkgConfig::MPG123
    PkgConfig::PORTAUDIO
    pthread
)

include(GNUInstallDirs)
install(TARGETS appqtradio
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
