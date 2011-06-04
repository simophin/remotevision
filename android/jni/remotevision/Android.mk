LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(trim $(BOOST_ROOT)),)
BOOST_ROOT := /usr/include
endif

LOCAL_MODULE := RemoteVision
LOCAL_SRC_FILES := lib/libRemoteVision.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include $(BOOST_ROOT)

include $(PREBUILT_SHARED_LIBRARY)