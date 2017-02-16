/**
 * Project : weimi-native-sdk
 * FileName: log.h
 * date    : 2017-02-17
 * time    : 00:40:56
 * author  : genglei.cuan@godinsec.com
 */

#ifndef LOG_H
#define LOG_H


#include <base/macros.h>
#include <string>

NS_GODIN_BEGIN

class Log{

  private:
    /**
     * 禁止创建该类实例
     */
    Log(){}

    static bool is_debug;
    static std::string default_tag;

  public:
    /**
     *  用于打印非错类型的调试信息，正式发布时，需关闭;
     *  可指定tag
     */
    static void i_with_tag(const char* tag, const char* log, ...);

    /**
     *  用于打印非错类型的调试信息，正式发布时，需关闭;
     *  使用默认tag:godinVm
     */
    static void i(const char* log, ...);

    /**
     * 用于打印错误类型的信息，正式发布时，不会关闭;
     * 可指定tag
     */
    static void e_with_tag(const char* tag, const char* log, ...);

    /**
     * 用于打印错误类型的信息，正式发布时，不会关闭;
     * 使用默认tag:godinVm
     */
    static void e(const char* log, ...);

    /**
     * 设置非错误类型调试信息的开关
     *
     * @brief setDebugFlag
     * @param isEnable
     */
    static void setDebugFlag(bool isEnable);

    /**
     * 获取非错误类型调试信息的开关
     *
     * @brief DebugFlag
     * @return
     */
    static bool DebugFlag();
};

NS_GODIN_END

#endif // LOG_H
