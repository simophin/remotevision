CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(RemoteVision)

# Search path
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CmakeModules)

# Platforms
option(BUILD_ANDROID "Building for ANDROID (library for ndk)" FALSE)
option(BUILD_SYMBIAN "Building for Symbian (library)" FALSE)

if(BUILD_ANDROID)
add_definitions(-DANDROID)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti ")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
set(Linux TRUE)
add_definitions(-DOS_LINUX)
add_definitions(-DOS_UNIX)
set(BUILD_LINUX TRUE)
endif()

if(WIN32)
add_definitions(-DOS_WIN32)
add_definitions(-include platform/win32/win32_config.h)
set(BUILD_WIN32 TRUE)
endif()

if(APPLE)
add_definitions(-DOS_APPLE)
set(BUILD_APPLE TRUE)
endif()

# Find some necessary packages
find_package(FFMPEG REQUIRED)

# Setup includes
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${FFMPEG_INCLUDE_DIR} 
)

# Setup flags
if(CMAKE_COMPILER_IS_GNUCXX)
add_definitions(-D__STDC_CONSTANT_MACROS)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
endif()

add_definitions(-DSTATIC_INITIALIZER)

# Files
FILE(GLOB SOURCES *.cpp)
ADD_SUBDIRECTORY(medium)
ADD_SUBDIRECTORY(commands)
ADD_SUBDIRECTORY(providers)
ADD_SUBDIRECTORY(codecs)
ADD_SUBDIRECTORY(vsources)
ADD_SUBDIRECTORY(3rdparty)
ADD_SUBDIRECTORY(platform)
ADD_LIBRARY(RemoteVision SHARED ${SOURCES})

TARGET_LINK_LIBRARIES(RemoteVision ${DEPEND_LIBS})


INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/  DESTINATION include
    FILES_MATCHING PATTERN "*.h"
    REGEX "android|CmakeModules|build|.git" EXCLUDE)
INSTALL(TARGETS RemoteVision
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

ADD_SUBDIRECTORY(test)

IF(NOT BUILD_ANDROID)
ADD_SUBDIRECTORY(tools)
ENDIF()