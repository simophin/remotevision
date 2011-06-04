LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := RemoteVisionJni
LOCAL_SRC_FILES := rv_jni.cpp
LOCAL_SHARED_LIBRARIES := RemoteVision
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

include remotevision/Android.mk