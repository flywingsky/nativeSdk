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
    jni/date/time.h

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
    jni/date/time.cpp
