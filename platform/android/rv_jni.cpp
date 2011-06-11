/*
 * rv_jni.cpp
 *
 *  Created on: 2011-6-10
 *      Author: simophin
 */


#include "rv_jni.h"
#include "NativeError.h"
#include "medium/TCPFFMpegServer.h"
#include "providers/FFMpegVideoProvider.h"
#include "3rdparty/ffmpeg/FFMpeg.h"

jint Java_com_lfcinvention_RemoteVision_VideoService_nativeCreateServerInstance
(JNIEnv *env, jobject instance, jobject nativeError, jint serverType, jstring host, jint port)
{
	TCPFFMpegServer::ConnType type ;
	if (serverType == 0) type = TCPFFMpegServer::SERVER;
	else type = TCPFFMpegServer::RELAY;

	std::string addr;
	const char *p = env->GetStringUTFChars(host,NULL);
	addr = p;
	env->ReleaseStringUTFChars(host,p);

	TCPFFMpegServer *server = new TCPFFMpegServer(type,addr,port);
	Error ec = server->init("/dev/video0", FFMpegVideoProvider::DEVICE_TYPE_CAPTURE);
	if (ec.isError()) {
		delete server;
		server = 0;
	}
	NativeError::fromNative(env,nativeError,ec);
	return (jint)server;
}



jobject Java_com_lfcinvention_RemoteVision_VideoService_nativeGetPreviewImage
(JNIEnv *env, jobject instance, jint server, jobject imgBuf)
{
	//TODO: Implement get preview image
	Error ec;
	return NativeError::fromNative(env,ec);
}



jobject Java_com_lfcinvention_RemoteVision_VideoService_nativeDestroyServerInstance
(JNIEnv *env, jobject instance, jint server)
{
	TCPFFMpegServer *s = (TCPFFMpegServer *)server;
	delete s;
	Error ec;
	return NativeError::fromNative(env,ec);
}



jobject Java_com_lfcinvention_RemoteVision_VideoService_nativeStartService
(JNIEnv *env, jobject instance, jint server)
{
	TCPFFMpegServer *s = (TCPFFMpegServer *)server;
	Error ec = s->start();
	return NativeError::fromNative(env,ec);
}



jobject Java_com_lfcinvention_RemoteVision_VideoService_nativeStopService
(JNIEnv *env, jobject instance, jint server)
{
	TCPFFMpegServer *s = (TCPFFMpegServer *)server;
	Error ec = s->stop();
	return NativeError::fromNative(env,ec);
}
jstring Java_com_lfcinvention_RemoteVision_VideoService_nativeGetBoundAddress(JNIEnv *env, jobject obj, jint server)
{
	TCPFFMpegServer *s = (TCPFFMpegServer *)server;
	std::string info = s->getBoundInfo();

	return env->NewStringUTF(info.c_str());
}


jint JNI_OnLoad(JavaVM * vm, void * preserved) {
	JNIEnv *env = NULL;
	vm->GetEnv((void **)&env,JNI_VERSION_1_4);
	NativeError::init(env);
	FFMpeg::init();
	return JNI_VERSION_1_4;
}





