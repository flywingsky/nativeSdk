LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


PROJECT_ROOT       := $(LOCAL_PATH)

#### 配置第三方开源库路径
CRYPTO_INCLUDE_PATH := ${PROJECT_ROOT}/third/cryptopp565/android-build/include

CRYPTO_LIB_PATH     := ${PROJECT_ROOT}/third/cryptopp565/android-build/$(TARGET_ARCH_ABI)

CURL_INCLUDE_PATH := ${PROJECT_ROOT}/third/libcurl/android-build/include

CURL_LIB_PATH     := ${PROJECT_ROOT}/third/libcurl/android-build/$(TARGET_ARCH_ABI)

EASYSQLITE_INCLUDE_PATH := ${PROJECT_ROOT}/third/easysqlite/android-build/include

EASYSQLITE_LIB_PATH     := ${PROJECT_ROOT}/third/easysqlite/android-build/$(TARGET_ARCH_ABI)

JSON11_INCLUDE_PATH := ${PROJECT_ROOT}/third/json11

### 设置头文件路径
LOCAL_C_INCLUDES += $(PROJECT_ROOT)\
                    ${CRYPTO_INCLUDE_PATH} \
										${CURL_INCLUDE_PATH} \
										${EASYSQLITE_INCLUDE_PATH} \
										${JSON11_INCLUDE_PATH} \


LOCAL_SRC_FILES := test.cpp\
                   utils/string_utils.cpp\
									 net/http_request.cpp\
									 net/http_response.cpp\
									 net/http_client.cpp\
									 net/curl_wrapper.cpp\
									 utils/log.cpp


LOCAL_SRC_FILES += third/json11/json11.cpp


LOCAL_MODULE := weimiSDK

LOCAL_LDLIBS += -llog -lz\

LOCAL_STATIC_LIBRARIES := libcurl

LOCAL_CPPFLAGS	:= -std=gnu++11 -fpermissive


#include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)

include $(BUILD_EXECUTABLE)

### 引入第三房开源库

$(call import-add-path-optional,${PROJECT_ROOT}/third)

$(call import-module, cryptopp565)
$(call import-module, easysqlite)
$(call import-module, libcurl)
