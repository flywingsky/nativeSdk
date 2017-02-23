/**
 * Project : weimi-native-sdk
 * FileName: authsms_json.cpp
 * date    : 2017-02-22
 * time    : 16:19:10
 * author  : genglei.cuan@godinsec.com
 */


#include "authsms_json.h"



bool godin::AuthSmsJson::parseFromJson(const std::string &json) {
    std::string error;
    json11::Json json_object = json11::Json::parse(json, error);

    if (!error.empty()) {
      return false;
    }
    /// 按照通信协议中定义的json数据格式进行提取
    statuscode = json_object["head"]["statuscode"].string_value().c_str();
    statusmsg  = json_object["head"]["statusmsg"].string_value().c_str();
    return true;
}

std::unique_ptr<godin::AuthSmsJson> godin::AuthSmsJson::clone() const {
   std::unique_ptr<AuthSmsJson> authSms(new AuthSmsJson());
   authSms->init(statuscode,statusmsg);
   return authSms;
}

void godin::AuthSmsJson::init(std::string statuscode, std::string statusmsg){
  this->statuscode = statuscode;
  this->statusmsg = statusmsg;
  return ;
}
