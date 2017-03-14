unix{
INCLUDEPATH +=/home/genglei-cuan/ssd/tools/Android/android-sdk-linux/ndk-bundle/platforms/android-23/arch-arm/usr/include
INCLUDEPATH +=/home/genglei-cuan/ssd/work/weimi/weimi-native-sdk/jni/third/cryptopp565/android-build/include
INCLUDEPATH +=/home/genglei-cuan/ssd/work/weimi/weimi-native-sdk/jni/third/easysqlite/android-build/include
INCLUDEPATH +=/home/genglei-cuan/ssd/work/weimi/weimi-native-sdk/jni/third/json11
INCLUDEPATH +=/home/genglei-cuan/ssd/work/weimi/weimi-native-sdk/jni/third/libcurl/android-build/include
}

macx{
INCLUDEPATH +=/Users/a-00/tools/android-ndk-r12b/platforms/android-23/arch-arm/usr/include
INCLUDEPATH +=/Users/a-00/work/workcode/nativeSdk/jni/third/cryptopp565/android-build/include
INCLUDEPATH +=/Users/a-00/work/workcode/nativeSdk/jni/third/easysqlite/android-build/include
INCLUDEPATH +=/Users/a-00/work/workcode/nativeSdk/jni/third/json11
INCLUDEPATH +=/Users/a-00/work/workcode/nativeSdk/jni/third/libcurl/android-build/include
}

win32 {

}

INCLUDEPATH += jni

HEADERS += \
    jni/net/http_client.h \
    jni/base/macros.h \
    jni/net/http_request.h \
    jni/net/http_response.h \
    jni/net/curl_wrapper.h \
    jni/utils/log.h \
    jni/utils/string_utils.h \
    jni/kms/respon_json/authsms_json.h \
    jni/kms/request/request_macros.h \
    jni/kms/request/request_test.h \
    jni/utils/file_utils.h \
    jni/utils/hash_utils.h \
    jni/file/file.h \
    jni/date/date.h \
    jni/date/time.h \
    jni/codec/base64.h \
    jni/encrypt/aes_engine.h \
    jni/encrypt/default_key.h \
    jni/utils/jni_utils.h \
    jni/weimi/wei_mi_struct.h \
    jni/weimi/weimi_macros.h \
    jni/weimi/weimi_write.h \
    jni/weimi/weimi.h \
    jni/encrypt/encrypt.h \
    jni/encrypt/encrypt_macros.h \
    jni/weimi/weimi_read.h \
    jni/weimi/weimi_jni.h


SOURCES += \
    jni/net/http_client.cpp \
    jni/net/http_request.cpp \
    jni/net/http_response.cpp \
    jni/net/curl_wrapper.cpp \
    jni/utils/log.cpp \
    jni/utils/string_utils.cpp \
    jni/kms/respon_json/authsms_json.cpp \
    jni/kms/request/request_test.cpp \
    jni/utils/file_utils.cpp \
    jni/utils/hash_utils.cpp \
    jni/file/file.cpp \
    jni/date/date.cpp \
    jni/date/time.cpp \
    jni/codec/base64.cpp \
    jni/test.cpp \
    jni/encrypt/aes_engine.cpp \
    jni/utils/jni_utils.cpp \
    jni/weimi/weimi_write.cpp \
    jni/weimi/weimi.cpp \
    jni/encrypt/encrypt.cpp \
    jni/weimi/weimi_read.cpp \
    jni/weimi/weimi_jni.cpp
