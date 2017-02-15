unix{
INCLUDEPATH +=/home/genglei-cuan/ssd/tools/Android/android-sdk-linux/ndk-bundle/platforms/android-23/arch-arm/usr/include
INCLUDEPATH +=/home/genglei-cuan/ssd/work/weimi/weimi-native-sdk/jni/third/cryptopp565/android-build/include
INCLUDEPATH +=/home/genglei-cuan/ssd/work/weimi/weimi-native-sdk/jni/third/easysqlite/android-build/include
INCLUDEPATH +=/home/genglei-cuan/ssd/work/weimi/weimi-native-sdk/jni/third/json11
INCLUDEPATH +=/home/genglei-cuan/ssd/work/weimi/weimi-native-sdk/jni/third/libcurl/android-build/include
}

INCLUDEPATH += jni

HEADERS += \
    jni/net/http_client.h \
    jni/base/macros.h \
    jni/net/http_request.h

SOURCES += \
    jni/net/http_client.cpp \
    jni/net/http_request.cpp
