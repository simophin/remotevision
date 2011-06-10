/*
 * rv_jni.cpp
 *
 *  Created on: 2011-6-10
 *      Author: simophin
 */


#include "rv_jni.h"



jint Java_com_lfcinvention_RemoteVision_VideoService_nativeCreateServerInstance
(JNIEnv *env, jobject instance, jobject nativeError, jint serverType, jstring host, jint port)
{
}



jobject Java_com_lfcinvention_RemoteVision_VideoService_nativeGetPreviewImage
(JNIEnv *env, jobject instance, jint server, jobject imgBuf)
{
}



jobject Java_com_lfcinvention_RemoteVision_VideoService_nativeDestroyServerInstance
(JNIEnv *env, jobject instance, jint server)
{
}



jobject Java_com_lfcinvention_RemoteVision_VideoService_nativeStartService
(JNIEnv *env, jobject instance, jint server)
{
}



jobject Java_com_lfcinvention_RemoteVision_VideoService_nativeStopService
(JNIEnv *env, jobject instance, jint server)
{
}

