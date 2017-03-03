#include "jni_utils.h"
#include <pthread.h>
#include <utils/log.h>

JavaVM* godin::JniUtils::java_vm_ = nullptr;

/******************************************/
static pthread_key_t k_JniEnv;
void detachCurrentThread(void* parameter) {
  godin::JniUtils::getJavaVm()->DetachCurrentThread();
}
/******************************************/

void godin::JniUtils::setJavaVM(JavaVM *vm) {

  java_vm_ = vm;
  pthread_key_create(&k_JniEnv, detachCurrentThread);
}

JNIEnv *godin::JniUtils::getJniEnv() {
  JNIEnv* env = reinterpret_cast<JNIEnv*>(pthread_getspecific(k_JniEnv));
  if (env == nullptr && java_vm_ != nullptr) {
    env = setJniEnv(java_vm_);
  }
  return env;
}

bool godin::JniUtils::getStaticMethodInfo(godin::JavaMethodInfo *methodinfo, const char *class_name, const char *method_name, const char *param_code) {

  if ((methodinfo == nullptr) || (class_name == nullptr) || (method_name == nullptr)
      || (param_code == nullptr)) {
    return false;
  }

  JNIEnv* env = getJniEnv();
  if (!env) {
    godin::Log::e("failed to get JNIEnv");
    return false;
  }

  jclass class_id = env->FindClass(class_name);
  if (!class_id) {
    godin::Log::e("failed to find class %s", class_name);
    return false;
  }

  jmethodID method_id = env->GetStaticMethodID(class_id, method_name,
                                               param_code);
  if (!method_id) {
    godin::Log::e("failed to find static method %s", method_name);
    return false;
  }


  methodinfo->class_id = class_id;
  methodinfo->env = env;
  methodinfo->method_id = method_id;


  return true;
}

bool godin::JniUtils::getMethodInfo(godin::JavaMethodInfo *methodinfo, const char *class_name, const char *method_name, const char *param_code) {

  if ((methodinfo == nullptr) || (class_name == nullptr) || (method_name == nullptr)
      || (param_code == nullptr)) {
    return false;
  }

  JNIEnv* env = getJniEnv();
  if (!env) {
    godin::Log::e("failed to get JNIEnv");
    return false;
  }

  jclass class_id = env->FindClass(class_name);
  if (!class_id) {
    godin::Log::e("failed to find class %s", class_name);
    return false;
  }
  jmethodID method_id = env->GetMethodID(class_id, method_name, param_code);
  if (!method_id) {
    godin::Log::e("failed to find method %s", method_name);
    return false;
  }


  methodinfo->class_id = class_id;
  methodinfo->env = env;
  methodinfo->method_id = method_id;


  return true;
}

bool godin::JniUtils::getStaticFieldInfo(godin::JavaFieldInfo *fieldinfo, const char *class_name, const char *field_name, const char *sig_code) {

  if ((fieldinfo == nullptr) || (class_name == nullptr) || (field_name == nullptr)
       || (sig_code == nullptr)) {
    return false;
  }

  JNIEnv* env = getJniEnv();
  if (!env) {
    godin::Log::e("failed to get JNIEnv");
    return false;
  }

  jclass class_id = env->FindClass(class_name);
  if (!class_id) {
    godin::Log::e("failed to find class %s", class_name);
    return false;
  }

  jfieldID field_id = env->GetStaticFieldID(class_id, field_name,sig_code);
  if (!field_id) {
    godin::Log::e("failed to find static method %s", field_name);
    return false;
  }


  fieldinfo->class_id = class_id;
  fieldinfo->env = env;
  fieldinfo->field_id = field_id;

  return true;
}

bool godin::JniUtils::getFieldInfo(godin::JavaFieldInfo *fieldinfo, const char *class_name, const char *field_name, const char *sig_code) {
  if ((fieldinfo == nullptr) || (class_name == nullptr) || (field_name == nullptr)
       || (sig_code == nullptr)) {
    return false;
  }

  JNIEnv* env = getJniEnv();
  if (!env) {
    godin::Log::e("failed to get JNIEnv");
    return false;
  }

  jclass class_id = env->FindClass(class_name);
  if (!class_id) {
    godin::Log::e("failed to find class %s", class_name);
    return false;
  }

  jfieldID field_id = env->GetFieldID(class_id, field_name,sig_code);
  if (!field_id) {
    godin::Log::e("failed to find static method %s", field_name);
    return false;
  }


  fieldinfo->class_id = class_id;
  fieldinfo->env = env;
  fieldinfo->field_id = field_id;

  return true;
}

std::string godin::JniUtils::jstringToStdString(const jstring jstr) {
  if (jstr == nullptr) {
    return "";
  }

  JNIEnv* env = getJniEnv();
  if (!env) {
    return "";
  }

  const char* chars = env->GetStringUTFChars(jstr, nullptr);
  std::string ret(chars);
  env->ReleaseStringUTFChars(jstr, chars);

  return ret;
}

jstring godin::JniUtils::stringToJString(const std::string &str) {
  JNIEnv* env = getJniEnv();
  if (!env) {
    return nullptr;
  }
  return env->NewStringUTF(str.c_str());
}

JNIEnv *godin::JniUtils::setJniEnv(JavaVM *vm) {
  JNIEnv* env = nullptr;
  jint ret = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
  switch (ret) {
    case JNI_OK:
      pthread_setspecific(k_JniEnv, env);
      return env;

    case JNI_EDETACHED: {
      if (vm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
        godin::Log::e("failed to call AttachCurrentThread()");
        return nullptr;
      } else {
        pthread_setspecific(k_JniEnv, env);
        return env;
      }
    }

    case JNI_EVERSION:
      godin::Log::e("JNI version 1.6 not supported");
      return nullptr;

    default:
      godin::Log::e("failed to get the environment using GetEnv()");
      return nullptr;
  }
}
