/**
 * Project : weimi-native-sdk
 * FileName: log.cpp
 * date    : 2017-02-17
 * time    : 00:42:19
 * author  : genglei.cuan@godinsec.com
 */

#include "log.h"
#include <android/log.h>
#include <stdarg.h>

bool godin::Log::is_debug = true;
std::string godin::Log::default_tag = "cgl";



void godin::Log::i_with_tag(const char* tag, const char* log, ...){
  if(is_debug){
      va_list arg;
      va_start(arg, log);
      __android_log_vprint(ANDROID_LOG_INFO, tag, log, arg);
      va_end(arg);
  }
}

void godin::Log::i(const char* log, ...){
  if(is_debug){
      va_list arg;
      va_start(arg, log);
      __android_log_vprint(ANDROID_LOG_INFO, default_tag.c_str(), log, arg);
      va_end(arg);
  }
}

void godin::Log::e_with_tag(const char* tag, const char* log, ...){
  va_list arg;
  va_start(arg, log);
  __android_log_vprint(ANDROID_LOG_ERROR, tag, log, arg);
  va_end(arg);
}

void godin::Log::e(const char* log, ...){
  va_list arg;
  va_start(arg, log);
  __android_log_vprint(ANDROID_LOG_ERROR, default_tag.c_str(), log, arg);
  va_end(arg);
}

void godin::Log::setDebugFlag(bool isEnable){
  is_debug = isEnable;
}

bool godin::Log::DebugFlag(){
  return is_debug;
}

