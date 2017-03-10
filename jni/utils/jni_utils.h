/**
 * Project : weimi-native-sdk
 * FileName: jni_utils.h
 * date    : 2017-03-03
 * time    : 16:44:59
 * author  : genglei.cuan@godinsec.com
 */

#ifndef JNIUTILS_H
#define JNIUTILS_H

#include <base/macros.h>
#include <jni.h>
#include <string>
#include <vector>

NS_GODIN_BEGIN

typedef struct JavaMethodInfo_ {
  JNIEnv* env;
  jclass class_id;
  jmethodID method_id;
} JavaMethodInfo;

typedef struct JavaFieldInfo_ {
  JNIEnv* env;
  jclass class_id;
  jfieldID field_id;
} JavaFieldInfo;

/**
 * @brief The JniUtils class
 * jni 操作的简单封装.
 *
 * Android 系统必须支持 JNI_VERSION_1_6.
 */
class JniUtils {

  public:
    /**
     * @brief setJavaVM
     * 务必保证使用该类其他方法前,
     * 调用该方法设置 JavaVm.
     * @param vm
     */
    static void setJavaVM(JavaVM* vm);

    static JavaVM* getJavaVm(){ return java_vm_;}

    /**
     * @brief getJniEnv
     * 获取当前线程的 JniEnv.
     *
     * 注意:当该线程是在native中新创建,而且还没与JavaVM绑定时,
     * 该方法会将线程与JavaVM绑定.然后返回JNIEnv.
     *
     * @return
     */
    static JNIEnv* getJniEnv();


    /**
     * @brief getStaticMethodInfo
     * 获取某个java类的某个静态方法信息
     *
     * @param methodinfo
     * @param class_name
     * @param method_name
     * @param param_code
     * @return
     * 成功返回true;
     * 失败返回false;
     */
    static bool getStaticMethodInfo(JavaMethodInfo* methodinfo,
                                         const char* class_name,
                                         const char* method_name,
                                         const char* param_code);

    /**
     * @brief getMethodInfo
     * 获取某个java类的某个非静态方法信息
     *
     * @param methodinfo
     * @param class_name
     * @param method_name
     * @param param_code
     * @return
     */
    static bool getMethodInfo(JavaMethodInfo* methodinfo,
                                   const char* class_name,
                                   const char* method_name,
                                   const char* param_code);

    /**
     * @brief getStaticFieldInfo
     * 获取某个java类的某个静态属性成员信息
     *
     * @param fieldinfo
     * @param class_name
     * @param field_name
     * @param sig_code
     * @return
     */
    static bool getStaticFieldInfo(JavaFieldInfo* fieldinfo,
                                         const char* class_name,
                                         const char* field_name,
                                         const char* sig_code);


    /**
     * @brief getFieldInfo
     * 获取某个java类的某个非静态属性成员信息
     *
     * @param fieldinfo
     * @param class_name
     * @param field_name
     * @param sig_code
     * @return
     */
    static bool getFieldInfo(JavaFieldInfo* fieldinfo,
                                   const char* class_name,
                                   const char* field_name,
                                   const char* sig_code);

    /**
     * @brief stringFromJstring
     * 将 Jstring 转换为 std::string
     * @param jstr
     * @return
     */
    static std::string jstringToStdString(const jstring jstr);

    /**
     * @brief stringToJString
     * 将 std::string 转换为 Jstring
     * @param str
     * @return
     */
    static jstring stringToJString(const std::string& str);

    static int getSdkVersion();

  private:
    JniUtils(){}

    /**
     * @brief setJniEnv
     * 缓存当前线程的 JniEnv 到线程局部缓存中去.
     * @param vm
     * @return
     */
    static JNIEnv* setJniEnv(JavaVM* vm);
    static JavaVM* java_vm_;
};

NS_GODIN_END

#endif // JNIUTILS_H
