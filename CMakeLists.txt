cmake_minimum_required(VERSION 3.5)

project(webbis-process-analyzer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES webbis-process-analyzer_ru_RU.ts)

set(INCLUDE_DIR include)
set(SOURCE_DIR src)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${SOURCE_DIR}/logger/LogLevel.cpp
        ${SOURCE_DIR}/logger/Logger.cpp
        ${SOURCE_DIR}/process/ProcessInfo.cpp
        ${SOURCE_DIR}/process/ProcessStatus.cpp
        ${SOURCE_DIR}/process/analysis/MemoryBlock.cpp
        ${SOURCE_DIR}/process/analysis/MemoryPerms.cpp
        ${SOURCE_DIR}/ui/page/ListProcessPage.cpp
        ${SOURCE_DIR}/ui/page/ProcessDetailPage.cpp
        ${SOURCE_DIR}/ui/page/IPage.cpp
        ${SOURCE_DIR}/FindValueType.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(webbis-process-analyzer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET webbis-process-analyzer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(webbis-process-analyzer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(webbis-process-analyzer
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(webbis-process-analyzer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(webbis-process-analyzer PUBLIC ${INCLUDE_DIR})

set_target_properties(webbis-process-analyzer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if (WIN32)
    #target_sources(webbis-process-analyzer PRIVATE)
elseif(UNIX)
    target_sources(webbis-process-analyzer PUBLIC
            ${SOURCE_DIR}/process/analysis/LinuxProcessMemoryParser.cpp
            ${SOURCE_DIR}/process/detail/LinuxProcessFinder.cpp
            ${SOURCE_DIR}/process/memory/LinuxMemoryFinder.cpp
            ${SOURCE_DIR}/process/memory/LinuxMemoryRegionFilter.cpp
    )
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(webbis-process-analyzer)
endif()
