/**
 * Project : weimi-native-sdk
 * FileName: curl_wrapper.h
 * date    : 2017-02-16
 * time    : 19:45:26
 * author  : genglei.cuan@godinsec.com
 */



#ifndef CURLWRAPPER_H
#define CURLWRAPPER_H

#include <base/macros.h>
#include <curl/curl.h>
#include <curl/easy.h>

NS_GODIN_BEGIN

/**
 * @brief The CurlWrapper class
 *
 * 对 libCurl 库的简单封装.
 *
 * 采用 RAII 思想封装,避免忘记资源释放.
 *
 * 关于 libcurl 的使用,可参考其官方文档:
 * https://curl.haxx.se/libcurl/c/
 */

class CurlWrapper {

  public:
    /**
     * @brief CurlWrapper
     * 初始化 libCurl 资源
     */
    CurlWrapper();

    /**
     * @brief ~CurlWrapper
     * 释放资源
     */
    virtual ~CurlWrapper();


  private:

    template<class T>
    bool setCurlOption(CURLoption option, T data);

    template<class T>
    bool getCurlInfo(CURLINFO option, T data);

    bool configureLibCurl(CURL *curl, char *error_buffer, bool isHttps);

    bool performLibCurl(int64_t *response_code);

  private:
    DISALLOW_COPY_AND_ASSIGN(CurlWrapper);

    CURL* libCurl_;

};

NS_GODIN_END

#endif // CURLWRAPPER_H
