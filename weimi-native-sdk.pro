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

INCLUDEPATH += jni

HEADERS += \
    jni/net/http_client.h \
    jni/base/macros.h \
    jni/net/http_request.h \
    jni/net/http_response.h \
    jni/net/curl_wrapper.h

SOURCES += \
    jni/net/http_client.cpp \
    jni/net/http_request.cpp \
    jni/net/http_response.cpp \
    jni/net/curl_wrapper.cpp
