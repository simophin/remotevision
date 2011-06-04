LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := RemoteVision
LOCAL_SRC_FILES := lib/libRemoteVision.so
LOCAL_EXPORT_C_INCLUDES := include

include $(PREBUILT_SHARED_LIBRARY)