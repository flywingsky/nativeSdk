/**
 * Project : weimi-native-sdk
 * FileName: curl_wrapper.cpp
 * date    : 2017-02-16
 * time    : 19:45:36
 * author  : genglei.cuan@godinsec.com
 */


#include "curl_wrapper.h"
#include <utils/log.h>
#include <map>
#include "http_request.h"

godin::CurlWrapper::CurlWrapper() {
  libCurl_ = curl_easy_init();
}

godin::CurlWrapper::~CurlWrapper() {
  if(libCurl_ != NULL)
    curl_easy_cleanup(libCurl_);
}



bool godin::CurlWrapper::configureLibCurl(CURL *curl, char *error_buffer) {

  if(curl == NULL)
    return false;

  int32_t code;

  /**
   * 当数据传输完成，curl_easy_perform返回一个代码来告诉调用者否成功。
   * 使用CURLOPT_ERRORBUFFER ,让libcurl缓存更多可读的错误信息,
   * 便于后续分析错误原因
   */
  code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
  if (code != CURLE_OK) {
    return false;
  }
  /**
   * 设置连接超时为30s,超过30s,还没连接成功的话,终止连接
   */
  code = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L);
  if (code != CURLE_OK) {
    return false;
  }
  /**
   * 连接成功之后,设置一次请求的最大时间为30s,
   * 即超过30s,中断请求.
   */
  code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
  if (code != CURLE_OK) {
    return false;
  }


  if (code != CURLE_OK) {
    return false;
  }
  return true;
}

bool godin::CurlWrapper::performLibCurl(int64_t *response_code) {

  /// 执行 http 请求
  /// 最长等待30s时间
  if (CURLE_OK != curl_easy_perform(libCurl_)) {
    return false;
  }

  /// 获取libcurl执行结果
  /// 获取 http 返回的 status code 如200,404等等
  long http_response_code;
  CURLcode code = curl_easy_getinfo(libCurl_, CURLINFO_RESPONSE_CODE, &http_response_code);
  if (response_code) {
    *response_code = static_cast<int64_t>(http_response_code);
  }

  /// 检查执行结果
  if ((code != CURLE_OK) || (*response_code != 200)) {
    godin::Log::e("libcurl curl_easy_getinfo failed: %s", curl_easy_strerror(code));
    return false;
  }
  return true;
}


bool godin::CurlWrapper::initCurl(const godin::HttpRequest *request,
                                  writeCallback callback,
                                  void *stream,
                                  writeCallback header_callback,
                                  void *header_stream,
                                  char *error_buffer,
                                  bool is_https) {
  if(libCurl_ == NULL)
    return false;

  if(!configureLibCurl(libCurl_,error_buffer)){
    godin::Log::e(" configure libcurl error_buffer failed.");
    return false;
  }

  /**
   * 如果是 https 连接的话,
   * 取消 SSL 非法认证错误.
   *
   * 因为我们的 server 中没有证书.(证书一般要付费)
   *
   * TODO: 后续根据实际情况,决定是否打开该选项
   */
  bool code = false;
  if(is_https){
   code =  setCurlOption(CURLOPT_SSL_VERIFYPEER, 0);
   if (!code) {
     godin::Log::e(" configure CURLOPT_SSL_VERIFYPEER failed.");
     return false;
   }
   code =  setCurlOption(CURLOPT_SSL_VERIFYHOST, 0);
   if (!code) {
     godin::Log::e(" configure CURLOPT_SSL_VERIFYHOST failed.");
     return false;
   }
   #ifdef HTTPS_DEBUG
   setCurlOption(CURLOPT_VERBOSE, 1L);
   #endif
  }
  code = setCurlOption(CURLOPT_NOSIGNAL, 1L);
  if (!code) {
    godin::Log::e(" configure CURLOPT_NOSIGNAL failed.");
    return false;
  }
  /// 构建 http 请求的 header
  std::map<std::string, std::string> request_header = request->requestHeader();
  if (!request_header.empty()) {
    for (auto it = request_header.begin(); it != request_header.end(); ++it) {
      std::string header_string = it->first + ":" + it->second;
      headers_ = curl_slist_append(headers_, header_string.c_str());
      if (!setCurlOption(CURLOPT_HTTPHEADER, headers_)) {
        godin::Log::e(" configure CURLOPT_HTTPHEADER failed.");
        return false;
      }
    }
  }
  bool success = setCurlOption(CURLOPT_URL, request->url().c_str())/* 设置 url*/
  && setCurlOption(CURLOPT_HEADERFUNCTION, header_callback) /* libcurl 收到 http 头部数据时的回调方法*/
  && setCurlOption(CURLOPT_HEADERDATA, header_stream) /* libcurl 收到的 http 头部数据的地址*/
  && setCurlOption(CURLOPT_WRITEFUNCTION, callback) /* libcurl 收到数据后的回调方法，即可以用来保存数据*/
  && setCurlOption(CURLOPT_WRITEDATA, stream); /* libcurl 收到的数据的地址*/
  return success;

}
