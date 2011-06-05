
IF(NOT BUILD_ANDROID AND NOT BUILD_SYMBIAN)
ADD_EXECUTABLE(NetworkTest TestNetwork.cpp)
TARGET_LINK_LIBRARIES(NetworkTest RemoteVision pthread)

ADD_EXECUTABLE(ServerTest TestServer.cpp)
TARGET_LINK_LIBRARIES(ServerTest RemoteVision pthread)

ADD_EXECUTABLE(ClientTest TestClient.cpp)
TARGET_LINK_LIBRARIES(ClientTest RemoteVision pthread)

ADD_EXECUTABLE(FFMPEGTest TestFFMPEG.cpp)
TARGET_LINK_LIBRARIES(FFMPEGTest RemoteVision pthread)

ADD_EXECUTABLE(UtilsTest TestUtils.cpp)
TARGET_LINK_LIBRARIES(UtilsTest)

ADD_SUBDIRECTORY(VideoPreviewer)
ENDIF()