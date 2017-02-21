/**
 * Project : weimi-native-sdk
 * FileName: http_response.h
 * date    : 2017-02-16
 * time    : 01:09:39
 * author  : genglei.cuan@godinsec.com
 */

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <base/macros.h>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "http_request.h"
NS_GODIN_BEGIN



class HttpResponse {

  public:
    /**
     * 利用 explicit 显示声明该构造函数，避免隐式转换
     */
    explicit HttpResponse(std::unique_ptr<HttpRequest> request);

    virtual ~HttpResponse() {}

    /**
     * 友元类，使得 HttpClient 成员方法
     * 可以通过 HttpResponse 对象，
     * 访问 HttpResponse 的私有成员
     */
    friend class HttpClient;

    const HttpRequest* getHttpRequest() const;

    bool isSucceed() const { return succeed_; }
    void setSucceed(bool succeed) { succeed_ = succeed; }


    std::vector<char> responseData() const { return response_data_; }
    std::string responseDataAsString() const;
    void setResponseData(std::vector<char> buffer) { response_data_ = buffer; }


    std::string errorBuffer() const { return error_buffer_; }
    void setErrorBuffer(const std::string& error) { error_buffer_ = error; }


    int64_t responseCode() const { return response_code_; }
    void setResponseCode(int64_t code) { response_code_ = code; }


    std::map<std::string, std::string> responseHeader() const { return response_header_;  }
    void addValueToHttpHeaderField(const std::string& key, const std::string& value) {
      response_header_.insert(std::pair<std::string, std::string>(key, value));
    }


  private:
    DISALLOW_COPY_AND_ASSIGN(HttpResponse);

    std::unique_ptr<HttpRequest> http_request_;
    bool succeed_;
    int64_t response_code_;         ///< 返回的状态码
    std::map<std::string, std::string> response_header_; ///< 返回的 header
    std::vector<char> response_data_; ///< 返回的 data 部分
    std::string error_buffer_; ///< 错误信息
};

NS_GODIN_END

#endif // HTTPRESPONSE_H

