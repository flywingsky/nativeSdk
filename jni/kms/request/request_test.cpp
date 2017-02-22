#include "request_test.h"
#include <string>
#include <json11.hpp>
#include <net/http_request.h>
#include <net/http_response.h>
#include <net/http_client.h>
#include "request_macros.h"
#include <utils/log.h>
#include <kms/respon_json/authsms_json.h>

void godin::RequestTest::testdemo() {
  std::unique_ptr<godin::HttpRequest> request(new godin::HttpRequest());


  json11::Json postData = json11::Json::object {
      { "phone_num", "15011329051" },
      { "msg_type", "1" },
      { "imei", "861110037303521" },
      { "app_version", "weimi1.0.1" },
  };

  request->setUrl(std::string(KmsServerAdderss) + std::string(KmsApiVersion) + "GetAuthSms");
  request->setRequestType(godin::HttpRequest::Type::POST);

  /// 必须在header中加这个字段,否则server端无法解析json数据,造成400错误
  request->addValueToHttpHeaderField("Content-Type", "application/json");

  request->setRequestData(postData.dump());

  godin::Log::i("post data: %s",request->requestData().c_str());

  godin::HttpClient::getInstanceClient()->syncSend(std::move(request), [](std::unique_ptr<godin::HttpResponse> response) {
    if (response->isSucceed()) {
      godin::Log::i("request http success.");
      /// 打印状态码,请务必使用"%lld"
      godin::Log::i("response code:%lld",response->responseCode());
      godin::Log::i("data:%s\n",response->responseDataAsString().c_str());
      /// 直接解析返回的json数据
      std::string err;
      json11::Json json = json11::Json::parse(response->responseDataAsString().c_str(),err);
      if (!err.empty()) {
          godin::Log::i("json parse failed.");
      }
      godin::Log::i("statuscode: %s",json["head"]["statuscode"].string_value().c_str());
      godin::Log::i("statusmsg: %s",json["head"]["statusmsg"].string_value().c_str());

      /// 也可先将其封装成一个类,然后将json数据打入类对象中去

      godin::AuthSmsJson authSms;

      authSms.parseFromJson(response->responseDataAsString().c_str());

      godin::Log::i("statuscode: %s",authSms.statusCode().c_str());
      godin::Log::i("statusmsg: %s",authSms.statusMsg().c_str());

    } else {
      godin::Log::i("request http err.");
      godin::Log::i("response code:%lld",response->responseCode());
      godin::Log::i("data:%s\n",response->responseDataAsString().c_str());
      godin::Log::i("errorBuffer:%s",response->errorBuffer().c_str());
    }
  });
  return ;
}
