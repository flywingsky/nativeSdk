/**
 * Project : weimi-native-sdk
 * FileName: curl_wrapper.cpp
 * date    : 2017-02-16
 * time    : 19:45:36
 * author  : genglei.cuan@godinsec.com
 */


#include "curl_wrapper.h"

godin::CurlWrapper::CurlWrapper() {
  libCurl_ = curl_easy_init();
}

godin::CurlWrapper::~CurlWrapper() {
  if(libCurl_ != NULL)
    curl_easy_cleanup(libCurl_);
}


template<class T>
bool godin::CurlWrapper::setCurlOption(CURLoption option, T data) {
  return CURLE_OK == curl_easy_setopt(libCurl_, option, data);
}

template<class T>
bool godin::CurlWrapper::getCurlInfo(CURLINFO option, T data) {
  return CURLE_OK == curl_easy_getinfo(libCurl_, option, data);
}

bool godin::CurlWrapper::configureLibCurl(CURL *curl, char *error_buffer, bool isHttps) {

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
  code = curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

  if (code != CURLE_OK) {
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
  if(isHttps){
   code =  curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
   if (code != CURLE_OK) {
     return false;
   }
   code =  curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
   if (code != CURLE_OK) {
     return false;
   }
  }
  return true;
}

bool godin::CurlWrapper::performLibCurl(int64_t *response_code) {

  /// 最长等待30s时间
  if (CURLE_OK != curl_easy_perform(libCurl_)) {
    return false;
  }

  long http_response_code;/// 如200,404等等
  CURLcode code = curl_easy_getinfo(libCurl_, CURLINFO_RESPONSE_CODE, &http_response_code);
  if (response_code) {
    *response_code = static_cast<int64_t>(http_response_code);
  }
}
