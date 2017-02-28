/**
 * Project : weimi-native-sdk
 * FileName: time.cpp
 * date    : 2017-03-01
 * time    : 00:35:40
 * author  : genglei.cuan@godinsec.com
 */
#include "time.h"
#include <sys/time.h>
#include <time.h>

int64_t godin::Time::currentTimeMillis() {
  timeval now;

  ::gettimeofday(&now, NULL);

  int64_t when = now.tv_sec * 1000LL + now.tv_usec / 1000;

  return when;
}
