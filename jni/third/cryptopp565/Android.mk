LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libcryptopp
LOCAL_SRC_FILES := android-build/$(TARGET_ARCH_ABI)/lib/libcryptopp.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/android-build/include
LOCAL_CPP_FEATURES := rtti exceptions 
include $(PREBUILT_STATIC_LIBRARY)




