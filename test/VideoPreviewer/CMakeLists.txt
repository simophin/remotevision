find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork) 

if(QT4_QMAKE_FOUND)

include(${QT_USE_FILE})

file(GLOB UI_FILES *.ui)
file(GLOB_RECURSE HDRS *.h) 
file(GLOB SRCS *.cpp)


QT4_WRAP_UI(UI_HDRS ${UI_FILES})
QT4_WRAP_CPP(MOC_HDR_SRCS ${HDRS})


add_executable(VideoPreviewer WIN32 MACOSX_BUNDLE ${UI_HDRS} ${MOC_HDR_SRCS} ${SRCS} )
include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(VideoPreviewer ${QT_LIBRARIES} RemoteVision)
endif()