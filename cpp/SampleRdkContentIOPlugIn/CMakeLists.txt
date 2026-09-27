cmake_minimum_required(VERSION 3.21)
project(SampleRdkContentIOPlugIn LANGUAGES CXX)

set(RHINO_SDK "${CMAKE_CURRENT_SOURCE_DIR}/../SDK")

set(SOURCES
    SampleRdkContentIOPlugIn.cpp
    SampleRdkContentIOPlugInRdkPlugIn.cpp
    SampleRdkContentIOPlugInRhinoPlugIn.cpp
)

if(WIN32)
    # The MFC DLL entry point. Rhino for Mac does not use MFC.
    list(APPEND SOURCES SampleRdkContentIOPlugInApp.cpp stdafx.cpp)

    # Without the .rc the plug-in has no version, and Options > Plug-ins shows
    # it blank.  The .rc includes no SDK headers, so the resource compiler does
    # not run the SDK checks.
    list(APPEND SOURCES SampleRdkContentIOPlugIn.rc)

    # CMake's default MSVC flags define WIN32.  Rhino for Windows is 64-bit only
    # and rhinoSdkChecks.h stops the build unless WIN64 is defined and WIN32 is not.
    string(REPLACE "/DWIN32" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    set(CMAKE_MFC_FLAG 2)  # MFC in a shared DLL
endif()

add_library(SampleRdkContentIOPlugIn MODULE ${SOURCES})
target_include_directories(SampleRdkContentIOPlugIn PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}" "${RHINO_SDK}/inc")
set_target_properties(SampleRdkContentIOPlugIn PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

if(WIN32)
    # Rhino plug-ins always link the release CRT and MFC, even when built with
    # debugging information.  A plug-in built against the debug CRT will not load,
    # so RHINO_DEBUG_PLUGIN takes the place of _DEBUG.
    set_target_properties(SampleRdkContentIOPlugIn PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" SUFFIX ".rhp")
    target_compile_definitions(SampleRdkContentIOPlugIn PRIVATE
        $<$<CONFIG:Debug>:RHINO_DEBUG_PLUGIN>)

    # RHINO_LIB_DIR is pasted into the SDK's #pragma comment(lib, ...) lines, which
    # the linker resolves relative to the build directory, so it has to be absolute.
    target_compile_definitions(SampleRdkContentIOPlugIn PRIVATE
        WIN64 _WINDOWS _USRDLL _AFXDLL _UNICODE UNICODE
        RHINO_LIB_DIR="${RHINO_SDK}/lib")

    # stdafx.h has to be compiled first in every source file.
    target_compile_options(SampleRdkContentIOPlugIn PRIVATE
        "/FI${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h")
endif()

if(APPLE)
    # stdafx.h has to be compiled first in every source file.
    target_compile_options(SampleRdkContentIOPlugIn PRIVATE
        "-include${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h")

    target_compile_options(SampleRdkContentIOPlugIn PRIVATE
        -x objective-c++ -fobjc-arc -fno-operator-names)

    target_compile_definitions(SampleRdkContentIOPlugIn PRIVATE
        ON_COMPILER_CLANG ON_RUNTIME_APPLE RHINO_APPLE=1 _GNU_SOURCE MY_ZCALLOC
        Z_PREFIX _UNICODE RHINO_V6_READY RHINO_THIRD_PARTY_OSX_PLUGIN_COMPILE
        $<$<CONFIG:Debug>:_DEBUG=1 ON__DEBUG> $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>)

    # ARC needs the Objective-C runtime.
    target_link_options(SampleRdkContentIOPlugIn PRIVATE -fobjc-link-runtime)

    # The real frameworks are found inside Rhino at run time.
    target_link_libraries(SampleRdkContentIOPlugIn PRIVATE
        "${RHINO_SDK}/lib/RhCore.tbd"
        "${RHINO_SDK}/lib/OpenNURBS.tbd"
        "${RHINO_SDK}/lib/RhMaterialEditor.tbd")

    # Rhino for Mac is arm64 only.  A plug-in is a loadable bundle, so the
    # package type has to be BNDL; CMake's default plist says APPL and Rhino
    # will not load that.
    set_target_properties(SampleRdkContentIOPlugIn PROPERTIES
        BUNDLE TRUE BUNDLE_EXTENSION rhp OSX_ARCHITECTURES arm64
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
        MACOSX_BUNDLE_EXECUTABLE_NAME SampleRdkContentIOPlugIn
        MACOSX_BUNDLE_BUNDLE_NAME SampleRdkContentIOPlugIn
        MACOSX_BUNDLE_GUI_IDENTIFIER com.mcneel.samples.SampleRdkContentIOPlugIn
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
        MACOSX_BUNDLE_BUNDLE_VERSION 1)
endif()
