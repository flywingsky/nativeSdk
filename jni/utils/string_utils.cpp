/**
 * Project : weimi-native-sdk
 * FileName: string_utils.cpp
 * date    : 2017-02-20
 * time    : 13:14:47
 * author  : genglei.cuan@godinsec.com
 */

#include "string_utils.h"
#include <cctype> //isspace
#include <string>
#include <algorithm> // find_if



std::string &godin::StringUtils::ltrim(std::string &s) {
  std::string::iterator it = std::find_if_not(s.begin(), s.end(), [](int c){return isspace(c);});
  s.erase(s.begin(),it);
  return s;
}

std::string &godin::StringUtils::rtrim(std::string &s) {
  std::string::iterator it = std::find_if_not(s.rbegin(), s.rend(), [](int c){return isspace(c);}).base();
  s.erase(it,s.end());
  return s;
}

std::string &godin::StringUtils::trim(std::string &s) {
  return ltrim(rtrim(s));
}

