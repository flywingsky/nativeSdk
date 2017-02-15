/**
 * Project : weimi-native-sdk
 * FileName: http_request.cpp
 * date    : 2017-02-15
 * time    : 19:06:21
 * author  : genglei.cuan@godinsec.com
 */


#include "http_request.h"
#include <time.h>

godin::HttpRequest::HttpRequest() {
  request_type_ = Type::UNKNOWN;
  url_.clear();
  request_header_.clear();
  request_data_.clear();
  identifier_.clear();
}

std::unique_ptr<godin::HttpRequest> godin::HttpRequest::Clone() const {
  std::unique_ptr<HttpRequest> request(new HttpRequest());
  request->setRequestType(request_type_);
  request->setUrl(url_);
  request->setRequestHeader(request_header_);
  request->setRequestData(request_data_);
  request->setIdentifier(identifier_);
  return request;
}

std::string godin::HttpRequest::StringFromHttpRequestType(godin::HttpRequest::Type type) {
  if (type == HttpRequest::Type::GET) {
    return "GET";
  } else if (type == HttpRequest::Type::POST) {
    return "POST";
  } else if (type == HttpRequest::Type::DELETE) {
    return "DELETE";
  } else if (type == HttpRequest::Type::PUT) {
    return "PUT";
  } else {
    return "";
    }
}

void godin::HttpRequest::addValueToHttpHeaderField(const std::string &key, const std::string &value) {
  request_header_.insert(std::pair<std::string, std::string>(key, value));
}

void godin::HttpRequest::setValueForHttpHeaderField(const std::string &key, const std::string &value) {
  request_header_[key] = value;
}

std::string godin::HttpRequest::getValueForHttpHeaderField(const std::string &key) {
  return request_header_[key];
}

std::string godin::HttpRequest::createIdentifier() const {
  time_t now;
  time(&now);
  return url_ + std::to_string(now);
}

