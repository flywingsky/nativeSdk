/**
 * Project : weimi-native-sdk
 * FileName: date.h
 * date    : 2017-02-28
 * time    : 15:22:28
 * author  : genglei.cuan@godinsec.com
 */


#ifndef DATE_H
#define DATE_H

#include <base/macros.h>
#include <time.h>
#include <string>

NS_GODIN_BEGIN

/**
 * @brief The Date class
 *
 * UTC:
 * Coordinated Universal Time 协调世界时,即世界标准时间
 *
 * 注意该类没有提供设置 date 的功能
 *
 */

class Date {
  public:
    /**
     * @brief Date
     * 以当前时间戳构造本地时间详情
     */
    Date();

    /**
     * @brief Date
     * 以时间戳(秒)构造
     *
     * @param stamp
     * 时间戳
     *
     * @param utc
     * 是否为UTC世界标准时间，
     * 为false则按本地日历时间构造
     */
    Date(time_t stamp, bool utc = false);


  public:
    /**
     * @brief localTimeZone
     * 返回当前系统的时区
     * @return
     */
    static int localTimeZone();


    /**
     * @brief isLeapYear
     * 判断是否为闰年
     * @param year
     * @return
     */
    static bool isLeapYear(int year);

    /**
     * @brief yearMonthDays
     * 某年的某月有多少天
     * @param year
     * @param month
     * @return
     */
    static int yearMonthDays(int year, int month);

    /**
     * @brief year
     * 返回时间戳对应的 年
     * 其值为1900年至今年数,所以 +1900
     * @return
     */
    int year() const { return tm_.tm_year + 1900;}

    /**
     * @brief month
     * 返回时间戳对用的 月
     * 取值区间为[0, 11],所以+1
     * @return
     */
    int month() const {return tm_.tm_mon + 1; }

    /**
     * @brief day
     * 返回时间戳对用的 日
     * 取值区间为[1, 31]
     * @return
     */
    int day() const {return tm_.tm_mday ;}

    /**
     * @brief hour
     * 返回时间戳对应的 时
     * 取值区间为[0, 23]
     * @return
     */
    int hour() const { return tm_.tm_hour;}

    /**
     * @brief minute
     * 返回时间戳对应的 分钟
     * 取值区间为[0, 59]
     * @return
     */
    int minute() const { return tm_.tm_min;}

    /**
     * @brief second
     * 返回时间戳对应的 秒
     * 取值区间为[0, 59]
     * @return
     */
    int second() const { return tm_.tm_sec;}

    /**
     * @brief week
     * 返回时间戳对应的 星期几
     * 取值区间[0, 6]，0代表星期天，1代表星期1，以此类推
     * @return
     */
    int week() const { return (tm_.tm_wday > 0) ? tm_.tm_wday : 7;}

    /**
     * @brief isUTC
     * 是世界标准时间还是本地时间
     * @return
     * true: 世界标准时间
     * false: 本地时间
     */
    bool isUTC() const { return is_utc_;}

    /**
     * @brief toString
     * 转换为字符串，格式为1970-01-01 00:00:00
     * @return
     */
    std::string toString() const;

    /**
     * @brief format
     * 转化为字符串,格式为1970-01-01 00:00:00
     * 不穿参数时,和toString()结果一直
     * @param fmt
     *     %Y 用CCYY表示的年（如：2017）
     *     %m 月份 (01-12)
     *     %d 月中的第几天(01-31)
     *     %H 小时, 24小时格式 (00-23)
     *     %M 分钟(00-59)
     *     %S 秒钟(00-59)
     * @return
     * 如果发生错误返回空字符串
     */
    std::string format(const char * fmt = "%Y-%m-%d %H:%M:%S") const;

    /**
     * @brief stamp
     * 返回 tm_ 对应的本地时间戳
     * @return
     */
    time_t stamp() const;

    /**
     * @brief utcStamp
     * 返回 tm_ 对应的utc时间戳
     * @return
     */
    time_t utcStamp() const;

    /**
     * @brief timeZone
     * 返回时区
     *
     * @return
     * 返回值大于0,比如为8时,为东部时区 8
     * 返回值小于0,比如为-3时,为西部时区 3
     */
    int timeZone() const;

    /**
     * @brief operator <
     * 比较两个date大小
     * @param other
     * @return
     *
     */
    bool operator < (const Date & other);

    /**
     * @brief operator =
     * 比较两个date是否相等
     * @param other
     * @return
     */
    bool operator = (const Date & other);

  private:
    /**
     * @brief getTm
     * 时间值转换为时间详情,即初始化 struct tm
     * gmtime_r( )和localtime_r( )可将time_t时间类型转换为tm结构体
     * @param stamp
     */
    void getTm(time_t stamp);

  private:

    bool is_utc_;
    struct tm tm_;
};

NS_GODIN_END

#endif // DATE_H
