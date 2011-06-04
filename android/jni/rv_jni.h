/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_lfcinvention_RemoteVision_VideoService */

#ifndef _Included_com_lfcinvention_RemoteVision_VideoService
#define _Included_com_lfcinvention_RemoteVision_VideoService
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_lfcinvention_RemoteVision_VideoService
 * Method:    nativeCreateServer
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_lfcinvention_RemoteVision_VideoService_nativeCreateServer
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_lfcinvention_RemoteVision_VideoService
 * Method:    nativeDestroyServer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_lfcinvention_RemoteVision_VideoService_nativeDestroyServer
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_lfcinvention_RemoteVision_VideoService
 * Method:    nativeStartServer
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_com_lfcinvention_RemoteVision_VideoService_nativeStartServer
  (JNIEnv *, jobject, jint, jboolean);

/*
 * Class:     com_lfcinvention_RemoteVision_VideoService
 * Method:    nativeGetBoundAddress
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_lfcinvention_RemoteVision_VideoService_nativeGetBoundAddress
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
