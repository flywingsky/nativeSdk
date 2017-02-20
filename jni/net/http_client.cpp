/**
 * Project : weimi-native-sdk
 * FileName: http_client.cpp
 * date    : 2017-02-17
 * time    : 15:28:56
 * author  : genglei.cuan@godinsec.com
 */


#include "http_client.h"
#include <utils/log.h>
#include "curl_wrapper.h"
#include <string.h>
#include <utils/string_utils.h>

#define ERRCODR_BUF_SIZE 256

godin::HttpClient* shared_client_ = nullptr;



godin::HttpClient *godin::HttpClient::getInstanceClient() {
  if (shared_client_ == nullptr) {
    shared_client_ = new HttpClient();
  }
  return shared_client_;

}

void godin::HttpClient::freeInstanceClient() {
  if(shared_client_ != nullptr){
      delete shared_client_;
      shared_client_ = nullptr;
    }
}

bool godin::HttpClient::removeRequest(const std::string &identifer) {
  std::lock_guard<std::mutex> lg(request_queue_mutex_);
  for (auto it = request_queue_.begin(); it != request_queue_.end(); ++it) {
    if (identifer == *it) {
      request_queue_.erase(it);
      return true;
    }
  }
  return false;
}

bool godin::HttpClient::isCancelledRequest(const std::string &identifer) const {
  for (auto it = request_queue_.begin(); it != request_queue_.end(); ++it) {
    if (identifer == *it) {
      return false;
    }
  }
  return true;
}

static size_t onHttpWriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
  std::vector<char> *recv_buffer = (std::vector<char>*)stream;
  size_t sizes = size * nmemb;

  recv_buffer->insert(recv_buffer->end(), (char*)ptr, (char*)ptr+sizes);

  return sizes;
}


static size_t onHttpWriteHeader(void *ptr, size_t size, size_t nmemb, void *stream)
{
  std::vector<char> *recv_buffer = (std::vector<char>*)stream;
  size_t sizes = size * nmemb;

  recv_buffer->insert(recv_buffer->end(), (char*)ptr, (char*)ptr+sizes);

  return sizes;
}

void godin::HttpClient::performHttpRequest(std::unique_ptr<godin::HttpRequest> request,
                                           std::function<void (std::unique_ptr<godin::HttpResponse>)> callback) {

  godin::Log::i("URL is %s,request type is %s",
                request->url().c_str(),
                HttpRequest::stringFromHttpRequestType(request->requestType()).c_str());

  /// 检查请求是否已经被取消
   if(isCancelledRequest(request->identifier())){
      godin::Log::e(" request %s is camcelled.",request->url().c_str());
      return false;
   }

   std::vector<char> response_data;       // 存储服务器返回的 http body 数据
   std::vector<char> response_header;     // 存储服务器返回的 http header 数据
   int64_t response_code = -1;            // http服务器返回的状态码
   char error_buffer[ERRCODR_BUF_SIZE];   // 存储请求出错时的详细错误信息
   bool success = false;

   /// 提取 body 数据
   if (!request->requestData().empty()) {
     std::string request_data = request->requestData();
     for (size_t i = 0; i < request_data.size(); ++i) {
       request->raw_data_.push_back(request_data[i]);
     }
   }

   /// 开始执行请求
   bool isHttps = strncmp(request->url().c_str(),"https",5) == 0 ? true : false;
   switch (request->requestType()) {
     case HttpRequest::Type::GET:
       {
         godin::CurlWrapper curl;
         success = curl.initCurl(request.get(),
                                 onHttpWriteData,
                                 &response_data,
                                 onHttpWriteHeader,
                                 &response_header,
                                 error_buffer,
                                 isHttps)
                   && curl.setCurlOption(CURLOPT_FOLLOWLOCATION, true)/* 该参数设置为非零值表示follow服务器返回的重定向信息 */
                   && curl.performLibCurl(&response_code);

       }
       break;
       case HttpRequest::Type::POST:
       {
         godin::CurlWrapper curl;
         success = curl.initCurl(request.get(),
                                 onHttpWriteData,
                                 &response_data,
                                 onHttpWriteHeader,
                                 &response_header,
                                 error_buffer,
                                 isHttps)
             && curl.setCurlOption(CURLOPT_POST, 1)
             && curl.setCurlOption(CURLOPT_POSTFIELDS, &request->raw_data_[0])
             && curl.setCurlOption(CURLOPT_POSTFIELDSIZE, request->raw_data_.size())
             && curl.performLibCurl(&response_code);
       }
       break;
       case HttpRequest::Type::PUT:
       {
         godin::CurlWrapper curl;
         success = curl.initCurl(request.get(),
                                 onHttpWriteData,
                                 &response_data,
                                 onHttpWriteHeader,
                                 &response_header,
                                 error_buffer,
                                 isHttps)
             && curl.setCurlOption(CURLOPT_CUSTOMREQUEST, "PUT")
             && curl.setCurlOption(CURLOPT_POSTFIELDS, &request->raw_data_[0])
             && curl.setCurlOption(CURLOPT_POSTFIELDSIZE, request->raw_data_.size())
             && curl.performLibCurl(&response_code);
       }
       break;
       case HttpRequest::Type::DELETE:
       {
         godin::CurlWrapper curl;
         success = curl.initCurl(request.get(),
                                 onHttpWriteData,
                                 &response_data,
                                 onHttpWriteHeader,
                                 &response_header,
                                 error_buffer,
                                 isHttps)
             && curl.setCurlOption(CURLOPT_CUSTOMREQUEST, "DELETE")
             && curl.setCurlOption(CURLOPT_FOLLOWLOCATION, true)/* 该参数设置为非零值表示follow服务器返回的重定向信息 */
             && curl.performLibCurl(&response_code);
       }
       break;
     default:
       godin::Log::e("HttpClient: unkown http request type. ");
       break;
     }

   /// 再次检查请求是否被取消了
   /// 被取消的话,也就没接收 response 消息的必要了
   if(isCancelledRequest(request->identifier())){
      godin::Log::e(" request %s is camcelled.",request->url().c_str());
      return false;
   }

   /// 开始处理 response 消息
   /// 即创建和设置 HttpResponse
   std::unique_ptr<HttpResponse> http_response(new HttpResponse(std::move(request)));
   http_response->setResponseCode(response_code);/// 提取状态码
   http_response->setErrorBuffer(error_buffer);  /// 提取错误信息
   http_response->setSucceed(success); /// 请求是否执行成功

   if(success){
       std::string response_header_string(response_header.begin(), response_header.end());
       http_response->setResponseData(response_data);/// 提取返回的数据

       std::istringstream resp(response_header_string);
       std::string header;
       std::string::size_type index;
       while (std::getline(resp, header) && header != "\r") {
         index = header.find(':', 0);
         if (index != std::string::npos) {
           std::string key_include_trim = header.substr(0, index);
           std::string value_include_trim = header.substr(index + 1);
           std::string key = godin::StringUtils::trim(key_include_trim);
           std::string value = godin::StringUtils::trim(value_include_trim);
           http_response->response_header_.insert(std::make_pair(key, value));
         }
       }
   }

}
