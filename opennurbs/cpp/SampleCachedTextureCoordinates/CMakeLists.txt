cmake_minimum_required(VERSION 3.16)

project(SampleCachedTextureCoordinates CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to a clone of https://github.com/mcneel/opennurbs
set(OPENNURBS_DIR "" CACHE PATH "Path to the openNURBS source folder")
if(NOT EXISTS "${OPENNURBS_DIR}/opennurbs.h")
  message(FATAL_ERROR "Set OPENNURBS_DIR to a clone of https://github.com/mcneel/opennurbs")
endif()

add_subdirectory("${OPENNURBS_DIR}" opennurbs)

add_executable(SampleCachedTextureCoordinates SampleCachedTextureCoordinates.cpp)
target_link_libraries(SampleCachedTextureCoordinates PRIVATE opennurbsStatic)

if(MSVC)
  target_compile_definitions(SampleCachedTextureCoordinates PRIVATE UNICODE _UNICODE)
endif()
