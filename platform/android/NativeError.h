/*
 * NativeError.h
 *
 *  Created on: 2011-6-11
 *      Author: simophin
 */

#ifndef NATIVEERROR_H_
#define NATIVEERROR_H_

#include <jni.h>
#include "Error.h"

class NativeError {
public:
	static Error fromJava (JNIEnv *, jobject);
	static jobject  fromNative (JNIEnv *, const Error &);
	static void fromNative(JNIEnv *, jobject, const Error &);

public:
	static jfieldID javaErrorStringField;
	static jfieldID javaIsSuccessField;
	static jclass   javaNativeError;
	static jmethodID javaNativeErrorConstructor;

	static void init(JNIEnv *env);

};

#endif /* NATIVEERROR_H_ */
