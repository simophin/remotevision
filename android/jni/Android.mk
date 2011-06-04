LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := RemoteVisionJni
LOCAL_SRC_FILES := jni.cpp

include $(BUILD_SHARED_LIBRARY)