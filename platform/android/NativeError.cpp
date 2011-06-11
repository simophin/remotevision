/*
 * NativeError.cpp
 *
 *  Created on: 2011-6-11
 *      Author: simophin
 */

#include "NativeError.h"


jfieldID NativeError::javaErrorStringField;
jfieldID NativeError::javaIsSuccessField;
jclass   NativeError::javaNativeError;
jmethodID NativeError::javaNativeErrorConstructor;


Error NativeError::fromJava(JNIEnv *env, jobject obj)
{
	Error ret;

	jboolean isSuccess = env->GetBooleanField(obj,javaIsSuccessField);
	jstring  errorString = (jstring)env->GetObjectField(obj,javaErrorStringField);

	const char *str = env->GetStringUTFChars(errorString,NULL);
	ret.setErrorString(str);
	env->ReleaseStringUTFChars(errorString,str);

	return ret;
}

jobject NativeError::fromNative(JNIEnv *env, const Error & err)
{
	jobject ret = env->NewObject(javaNativeError, javaNativeErrorConstructor);

	fromNative(env,ret,err);

	return ret;
}

void NativeError::fromNative(JNIEnv *env, jobject obj, const Error & err)
{
	env->SetBooleanField(obj,javaIsSuccessField, err.isSuccess());
	env->SetObjectField(obj,javaErrorStringField, env->NewStringUTF(err.getErrorString().c_str()));
}

void NativeError::init(JNIEnv *env) {
	javaNativeError = env->FindClass("com/lfcinvention/RemoteVision/NativeError");
	javaIsSuccessField = env->GetFieldID(javaNativeError,"isSuccess","Z");
	javaErrorStringField = env->GetFieldID(javaNativeError,"errorString","Ljava/lang/String;");
	javaNativeErrorConstructor = env->GetMethodID(javaNativeError,"<init>","()V");
}
