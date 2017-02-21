#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <base/macros.h>
#include <string>
#include <sstream>

/**
 * Project : weimi-native-sdk
 * FileName: string_utils.h
 * date    : 2017-02-20
 * time    : 13:10:02
 * author  : genglei.cuan@godinsec.com
 */


NS_GODIN_BEGIN

class StringUtils {

  private:
    StringUtils(){}

  public:
  /**
   * @brief ltrim
   * 清除字符串起始位置处的所有空格
   * @param s
   * 要被处理的字符串
   * 由于是引用传递,所以处理后,原始字符串也发生了改变.
   * @return
   */
  static std::string& ltrim(std::string& s);

  /**
   * @brief rtrim
   * 清除字符串结束位置处的所有空格
   * @param s
   * 要被处理的字符串
   * 由于是引用传递,所以处理后,原始字符串也发生了改变.
   * @return
   */
  static std::string& rtrim(std::string& s);

  /**
   * @brief trim
   * 清除字符串起始和结束位置处的所有空格
   * @param s
   * 要被处理的字符串
   * 由于是引用传递,所以处理后,原始字符串也发生了改变.
   * @return
   */
  static std::string& trim(std::string& s);

  template<typename T>
  static std::string toString(T value){
    std::ostringstream os;
    os << value;
    return os.str();
  }
};

NS_GODIN_END

#endif // STRINGUTILS_H
