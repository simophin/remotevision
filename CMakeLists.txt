CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(RemoteVision)

# Search path
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CmakeModules)

# Platforms
option(BUILD_ANDROID "Building for ANDROID (library for ndk)" FALSE)
option(BUILD_SYMBIAN "Building for Symbian (library)" FALSE)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
set(Linux TRUE)
add_definitions(-DOS_LINUX)
set(BUILD_LINUX TRUE)
endif()

if(WIN32)
add_definitions(-DOS_WIN32)
set(BUILD_WIN32 TRUE)
endif()

if(APPLE)
add_definitions(-DOS_APPLE)
set(BUILD_APPLE TRUE)
endif()

# Find some necessary packages
find_package(FFMPEG REQUIRED)
#find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED)

# Setup includes
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${FFMPEG_INCLUDE_DIR} 
    ${Boost_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIRS})

# Setup flags
if(CMAKE_COMPILER_IS_GNUCXX)
add_definitions(-D__STDC_CONSTANT_MACROS)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
endif()

MESSAGE(STATUS ${FFMPEG_LIBRARIES})

# Files
FILE(GLOB SOURCES *.cpp)
ADD_SUBDIRECTORY(medium)
ADD_SUBDIRECTORY(platform)
ADD_SUBDIRECTORY(commands)
ADD_SUBDIRECTORY(providers)
ADD_SUBDIRECTORY(codecs)
ADD_LIBRARY(RemoteVision SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(RemoteVision ${DEPEND_LIBS})

ADD_SUBDIRECTORY(test)