cmake_minimum_required(VERSION 3.21)
project(SampleEventWatcher LANGUAGES CXX)

set(RHINO_SDK "${CMAKE_CURRENT_SOURCE_DIR}/../SDK")

set(SOURCES
    SampleEventWatcherPlugIn.cpp
    SampleRhinoEventWatcher.cpp
    cmdSampleEventWatcher.cpp
    cmdSampleIdleWatcher.cpp
    cmdSampleNotUndoable.cpp
    cmdSampleUndoable.cpp
)

if(WIN32)
    # The MFC DLL entry point. Rhino for Mac does not use MFC.
    list(APPEND SOURCES SampleEventWatcherApp.cpp stdafx.cpp)
endif()

add_library(SampleEventWatcher MODULE ${SOURCES})
target_include_directories(SampleEventWatcher PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}" "${RHINO_SDK}/inc")
set_target_properties(SampleEventWatcher PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

if(APPLE)
    # stdafx.h has to be compiled first in every source file.
    target_compile_options(SampleEventWatcher PRIVATE
        "-include${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h")

    target_compile_options(SampleEventWatcher PRIVATE
        -x objective-c++ -fobjc-arc -fno-operator-names)

    target_compile_definitions(SampleEventWatcher PRIVATE
        ON_COMPILER_CLANG ON_RUNTIME_APPLE RHINO_APPLE=1 _GNU_SOURCE MY_ZCALLOC
        Z_PREFIX _UNICODE RHINO_V6_READY RHINO_THIRD_PARTY_OSX_PLUGIN_COMPILE
        $<$<CONFIG:Debug>:_DEBUG=1 ON__DEBUG> $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>)

    # ARC needs the Objective-C runtime.
    target_link_options(SampleEventWatcher PRIVATE -fobjc-link-runtime)

    # The real frameworks are found inside Rhino at run time.
    target_link_libraries(SampleEventWatcher PRIVATE
        "${RHINO_SDK}/lib/RhCore.tbd"
        "${RHINO_SDK}/lib/OpenNURBS.tbd"
        "${RHINO_SDK}/lib/RhMaterialEditor.tbd")

    # Rhino for Mac is arm64 only.  A plug-in is a loadable bundle, so the
    # package type has to be BNDL; CMake's default plist says APPL and Rhino
    # will not load that.
    set_target_properties(SampleEventWatcher PROPERTIES
        BUNDLE TRUE BUNDLE_EXTENSION rhp OSX_ARCHITECTURES arm64
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
        MACOSX_BUNDLE_EXECUTABLE_NAME SampleEventWatcher
        MACOSX_BUNDLE_BUNDLE_NAME SampleEventWatcher
        MACOSX_BUNDLE_GUI_IDENTIFIER com.mcneel.samples.SampleEventWatcher
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
        MACOSX_BUNDLE_BUNDLE_VERSION 1)
endif()
