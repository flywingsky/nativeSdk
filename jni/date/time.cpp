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

double godin::Time::currentTimeMillis() {
  struct timespec res;
  clock_gettime(CLOCK_REALTIME, &res);
  return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}
