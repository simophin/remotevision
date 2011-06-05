LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := RemoteVision
LOCAL_SRC_FILES := $(shell find $(RV_SRC_DIR) -name '*.cpp' | grep -v -E "android\/|build\/|CmakeModules\/|win32\/|test\/")
LOCAL_CFLAGS := -DANDROID \
				  -DOS_LINUX -DOS_UNIX \
				  -DBOOST_NO_EXCEPTIONS \
				  -DBOOST_NO_RTTI \
				  -DBOOST_EXCEPTION_DISABLE \
				  -D__STDC_CONSTANT_MACROS
LOCAL_LDLIBS := -L$(FFMPEG_ROOT)/lib -lswscale -lavdevice -lavformat -lavcodec -lavutil
LOCAL_C_INCLUDES := \
	$(RV_SRC_DIR) \
	$(BOOST_ROOT) \
	$(FFMPEG_ROOT)/include 
include $(BUILD_SHARED_LIBRARY)
