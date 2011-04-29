if(UNIX OR CYGWIN)
add_subdirectory(posix)
endif()

if(WIN32)
add_subdirectory(win32)
endif()

set(SOURCES ${SOURCES} ${PLATFORM_SOURCES} PARENT_SCOPE)
set(DEPEND_LIBS ${DEPEND_LIBS} ${PLATFORM_DEPEND_LIBS} PARENT_SCOPE)