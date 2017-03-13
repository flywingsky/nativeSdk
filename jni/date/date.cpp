#include "date.h"
#include <limits.h>
#include <sstream>
#include <iomanip>


godin::Date::Date() {
  is_utc_ = false;

  getTm(::time(NULL));
}

godin::Date::Date(time_t stamp, bool utc) {
  is_utc_ = utc;

  getTm(stamp);
}

int godin::Date::localTimeZone() {
  return Date(0).hour();
}

bool godin::Date::isLeapYear(int year) {
  return (year % 4 == 0 && ((year % 400 == 0) || (year % 100 != 0)));
}

int godin::Date::yearMonthDays(int year, int month) {
  switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
       return 31;
    case 4:
    case 6:
    case 9:
    case 11:
       return 30;
    case 2:
       return isLeapYear(year) ? 29 : 28;
    default:
       return 0;
  }
}

std::string godin::Date::toString() const {
  std::string str =  format();
  return str.replace(10,1,"-");
}

std::string godin::Date::format(const char *fmt) const {
    char buf[256] = {0};
    if (0 == ::strftime(buf, 256, fmt, &tm_)) {
      buf[0] = '\0';
    }
    return std::string(buf);
}



time_t godin::Date::stamp() const {
  return mktime(const_cast<struct tm *>(&tm_));
}

time_t godin::Date::utcStamp() const {
  return stamp() - timeZone() * -3600;
}

int godin::Date::timeZone() const {

  /// tm_.tm_gmtoff 指定了日期变更线东面时区中
  /// UTC东部时区正秒数或UTC西部时区的负秒数
  return static_cast<int>(tm_.tm_gmtoff / 3600);
}

bool godin::Date::operator <(const godin::Date &other) {

  if (tm_.tm_year != other.tm_.tm_year) {
     return tm_.tm_year < other.tm_.tm_year;
  }

  if (tm_.tm_mon != other.tm_.tm_mon) {
     return tm_.tm_mon < other.tm_.tm_mon;
  }

  if (tm_.tm_mday != other.tm_.tm_mday) {
     return tm_.tm_mday < other.tm_.tm_mday;
  }

  if (tm_.tm_hour != other.tm_.tm_hour) {
     return tm_.tm_hour < other.tm_.tm_hour;
  }

  if (tm_.tm_min != other.tm_.tm_min) {
     return tm_.tm_min < other.tm_.tm_min;
  }

  return tm_.tm_sec < other.tm_.tm_sec;
}

bool godin::Date::operator =(const godin::Date &other) {
  return  (tm_.tm_year == other.tm_.tm_year) &&
          (tm_.tm_mon == other.tm_.tm_mon) &&
          (tm_.tm_mday == other.tm_.tm_mday) &&
          (tm_.tm_hour == other.tm_.tm_hour) &&
          (tm_.tm_min == other.tm_.tm_min) &&
          (tm_.tm_sec == other.tm_.tm_sec);
}

void godin::Date::getTm(time_t stamp) {
  is_utc_ ? ::gmtime_r(&stamp, &tm_) : ::localtime_r(&stamp, &tm_);
}
