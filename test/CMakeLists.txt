
IF(NOT BUILD_ANDROID AND NOT BUILD_SYMBIAN)

#IF(TRUE)
#ADD_EXECUTABLE(NetworkTest TestNetwork.cpp)
#TARGET_LINK_LIBRARIES(NetworkTest RemoteVision pthread)

ADD_EXECUTABLE(ServerTest TestServer.cpp)
TARGET_LINK_LIBRARIES(ServerTest RemoteVision pthread)

#ADD_EXECUTABLE(ClientTest TestClient.cpp)
#TARGET_LINK_LIBRARIES(ClientTest RemoteVision pthread)

ADD_EXECUTABLE(FFMPEGTest TestFFMPEG.cpp)
TARGET_LINK_LIBRARIES(FFMPEGTest RemoteVision pthread)

ADD_SUBDIRECTORY(VideoPreviewer)

#ENDIF()

ADD_EXECUTABLE(UtilsTest TestUtils.cpp)
TARGET_LINK_LIBRARIES(UtilsTest RemoteVision pthread)
ENDIF()
