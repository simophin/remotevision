#include "rv_jni.h"
#include "3rdparty/ffmpeg/FFMpeg.h"

#include <android/log.h>
#include "medium/TCPServerSocket.h"
#include "medium/TCPSocketAddress.h"
#include "medium/TCPFFMpegServer.h"



#include <string>

jint Java_com_lfcinvention_RemoteVision_VideoService_nativeCreateServer
(JNIEnv *env, jobject obj, jstring addr, jint port)
{
	char aBuf[30];
	const char * addrBuf = env->GetStringUTFChars(addr,0);
	__android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, "Will bind on address %s", addrBuf);
	strncpy(aBuf,addrBuf,sizeof(aBuf));
	env->ReleaseStringUTFChars(addr,addrBuf);
	aBuf[sizeof(aBuf)-1] = 0;
	int aport = (int)port;


	std::string actualAddr = aBuf;
	TCPFFMpegServer *server = new TCPFFMpegServer(actualAddr,aport);

	if (!server->init("/dev/video0")) {
		__android_log_print(ANDROID_LOG_FATAL,LOG_TAG,"While init server: %s",
				server->getLastError().getErrorString().c_str());
		goto init_server_failed;
	}else{
		return (jint)server;

	}


	init_server_failed:
	delete server;
	return 0;
}



void Java_com_lfcinvention_RemoteVision_VideoService_nativeDestroyServer
(JNIEnv *env, jobject obj, jint server)
{
	TCPFFMpegServer *s = (TCPFFMpegServer *)server;
	delete s;
}




jstring Java_com_lfcinvention_RemoteVision_VideoService_nativeGetBoundAddress
(JNIEnv *env, jobject obj, jint server)
{
	TCPFFMpegServer *s = (TCPFFMpegServer *)server;
	std::string info = s->getBoundInfo();
	jstring ret = env->NewStringUTF(info.c_str());
	return ret;
}



void Java_com_lfcinvention_RemoteVision_VideoService_nativeStartServer
(JNIEnv *env, jobject obj, jint server, jboolean jstart)
{
	TCPFFMpegServer *s = (TCPFFMpegServer *)server;
	bool start = (bool)jstart;

	if (start) {
		if (!s->start()){
			__android_log_print(ANDROID_LOG_FATAL,LOG_TAG,"While init server: %s",
							s->getLastError().getErrorString().c_str());
		}
	}else{
		s->stop();
	}
}



jint JNI_OnLoad(JavaVM *env, void*)
{
	FFMpeg::init();

	return JNI_VERSION_1_4;
}


