
ADD_EXECUTABLE(NetworkTest TestNetwork.cpp)
TARGET_LINK_LIBRARIES(NetworkTest RemoteVision pthread)

ADD_EXECUTABLE(ServerTest TestServer.cpp)
TARGET_LINK_LIBRARIES(ServerTest RemoteVision pthread)

ADD_EXECUTABLE(ClientTest TestClient.cpp)
TARGET_LINK_LIBRARIES(ClientTest RemoteVision pthread)