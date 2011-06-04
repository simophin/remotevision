#include "rv_jni.h"
#include "rv_service.h"
#include "3rdparty/ffmpeg/FFMpeg.h"

#include <android/log.h>
#include "medium/TCPServerSocket.h"
#include "medium/TCPSocketAddress.h"



#include <string>

jint Java_com_lfcinvention_RemoteVision_VideoService_nativeCreateServer
(JNIEnv *env, jobject obj, jstring addr, jint port)
{
	const char * addrBuf = env->GetStringUTFChars(addr,0);
	__android_log_print(0,LOG_TAG, "Will bind on address %s", addrBuf);

	std::string actualAddr = addrBuf;
	env->ReleaseStringUTFChars(addr,addrBuf);

	int aport = (int)port;

	RvService *service = new RvService;
	int server_socket= socket(AF_INET, SOCK_STREAM, 0);
	if (service->serverSocket < 0) goto create_server_socket_failed;

	TCPSocketAddress saddr (actualAddr,aport);
	TCPServerSocket *serverSocket = new TCPServerSocket(server_socket);
	if (serverSocket->bind(&saddr) != 0) goto bind_server_socket_failed;
	if (serverSocket->listen(2) != 0)   goto listen_server_socket_failed;

	service->serverSocket = serverSocket;
	return (jint)service;

	listen_server_socket_failed:
	bind_server_socket_failed:
	delete serverSocket;
	create_server_socket_failed:
	delete service;

	return 0;
}



void Java_com_lfcinvention_RemoteVision_VideoService_nativeDestroyServer
(JNIEnv *env, jobject obj, jint server)
{
	RvService *s = (RvService *)server;
	delete s;
}




jstring Java_com_lfcinvention_RemoteVision_VideoService_nativeGetBoundAddress
(JNIEnv *env, jobject obj, jint server)
{
	RvService *s = (RvService *)server;
	std::string info = s->
}



void Java_com_lfcinvention_RemoteVision_VideoService_nativeStartServer
(JNIEnv *env, jobject obj, jint server, jboolean start)
{
}



jint JNI_OnLoad(JavaVM *env, void*)
{
	FFMpeg::init();

	return JNI_VERSION_1_4;
}


