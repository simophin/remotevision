#include "rv_jni.h"
#include "3rdparty/ffmpeg/FFMpeg.h"

#include <android/log.h>
#include "medium/TCPServerSocket.h"
#include "medium/TCPSocketAddress.h"
#include "medium/TCPFFMpegServer.h"

extern "C" {
#include <libavutil/avutil.h>
}

#include "RString.h"

jint Java_com_lfcinvention_RemoteVision_VideoService_nativeCreateServer
(JNIEnv *env, jobject obj, jstring addr, jint port)
{
	Error ec;
	char aBuf[30];
	const char * addrBuf = env->GetStringUTFChars(addr,0);
	__android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, "Will bind on address %s", addrBuf);
	strncpy(aBuf,addrBuf,sizeof(aBuf));
	env->ReleaseStringUTFChars(addr,addrBuf);
	aBuf[sizeof(aBuf)-1] = 0;
	int aport = (int)port;


	String actualAddr = aBuf;
	TCPFFMpegServer *server = new TCPFFMpegServer(actualAddr,aport);

	ec = server->init("/dev/video0");
	if ( ec.isError()) {
		__android_log_print(ANDROID_LOG_FATAL,LOG_TAG,"While init server: %s",
				ec.getErrorString().c_str());
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
	String info = s->getBoundInfo();
	jstring ret = env->NewStringUTF(info.c_str());
	return ret;
}



void Java_com_lfcinvention_RemoteVision_VideoService_nativeStartServer
(JNIEnv *env, jobject obj, jint server, jboolean jstart)
{
	TCPFFMpegServer *s = (TCPFFMpegServer *)server;
	Error ec;
	bool start = (bool)jstart;

	if (start) {
		ec = s->start();
		if (ec.isError()){
			__android_log_print(ANDROID_LOG_FATAL,LOG_TAG,"While init server: %s",
							ec.getErrorString().c_str());
		}
	}else{
		s->stop();
	}
}


void ffmpeg_log_android (void *ptr , int level,const char * fmt, va_list va) {
	__android_log_vprint(ANDROID_LOG_DEBUG,"RemoteVision", fmt,va);
}

jint JNI_OnLoad(JavaVM *env, void*)
{
	FFMpeg::init();

	av_log_set_callback(ffmpeg_log_android);
	av_log_set_level(AV_LOG_INFO);

	return JNI_VERSION_1_4;
}


