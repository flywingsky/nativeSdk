/**
 * Project : weimi-native-sdk
 * FileName: time.h
 * date    : 2017-03-01
 * time    : 00:35:31
 * author  : genglei.cuan@godinsec.com
 */
#ifndef TIME_H
#define TIME_H

#include <base/macros.h>
#include <stdio.h>

NS_GODIN_BEGIN

class Time {
  private:
    Time();
  public:

    /**
     * @brief currentTimeMillis
     * 返回系统当前的时间，毫秒计算；
     * 可用来计算某代码块运行时间时使用
     * @return
     */
    static int64_t currentTimeMillis();
};

NS_GODIN_END

#endif // TIME_H
