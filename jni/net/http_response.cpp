/**
 * Project : weimi-native-sdk
 * FileName: http_response.cpp
 * date    : 2017-02-16
 * time    : 01:10:36
 * author  : genglei.cuan@godinsec.com
 */

#include "http_response.h"



godin::HttpResponse::HttpResponse(std::unique_ptr<godin::HttpRequest> request) {

  http_request_ = std::move(request);
  succeed_ = false;

  response_header_.clear();
  response_data_.clear();
  error_buffer_.clear();
}

const godin::HttpRequest *godin::HttpResponse::getHttpRequest() const{
  return http_request_.get();
}

std::string godin::HttpResponse::responseDataAsString() const {
  std::string responseDataString(response_data_.begin(), response_data_.end());
  return responseDataString;
}
