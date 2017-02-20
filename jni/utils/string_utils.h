#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <base/macros.h>
#include <string>

NS_GODIN_BEGIN

class StringUtils {

  private:
    StringUtils(){}

  /**
   * @brief ltrim
   * 清除字符串起始位置处的所有空格
   * @param s
   * @return
   */
  static std::string& ltrim(std::string& s);

  /**
   * @brief rtrim
   * 清除字符串结束位置处的所有空格
   * @param s
   * @return
   */
  static std::string& rtrim(std::string& s);

  /**
   * @brief trim
   * 清除字符串起始和结束位置处的所有空格
   * @param s
   * @return
   */
  static std::string& trim(std::string& s);
};

NS_GODIN_END

#endif // STRINGUTILS_H
