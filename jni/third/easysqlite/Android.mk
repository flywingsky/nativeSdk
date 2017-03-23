LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libEasySqlite
LOCAL_SRC_FILES := android-build/$(TARGET_ARCH_ABI)/lib/libEasySqlite.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/android-build/include
LOCAL_CPP_FEATURES := rtti exceptions 
#include $(PREBUILT_SHARED_LIBRARY)
include $(PREBUILT_STATIC_LIBRARY)



