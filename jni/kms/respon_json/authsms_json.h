#ifndef AUTHSMSJSON_H
#define AUTHSMSJSON_H

#include <base/macros.h>
#include <json11.hpp>
#include <string>

NS_GODIN_BEGIN

class AuthSmsJson {
  public:
    AuthSmsJson(){statuscode.clear();statusmsg.clear();}

    /**
     * @brief parseFromJson
     * 从json数据中提取相关数据初始化该类中的相关成员.
     * @param json
     * @return
     */
    bool parseFromJson(const std::string& json);

    /**
     * @brief clone
     * 创建一个该对象的副本的唯一方法.
     * @return
     */
    std::unique_ptr<AuthSmsJson> clone() const;

    std::string statusCode(){return statuscode;}

    std::string statusMsg(){return statusmsg;}

  private:
    DISALLOW_COPY_AND_ASSIGN(AuthSmsJson);
    void init(std::string statuscode,std::string statusmsg);
    std::string statuscode;
    std::string statusmsg;
};

NS_GODIN_END

#endif // AUTHSMSJSON_H
