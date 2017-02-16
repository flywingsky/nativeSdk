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
 * 对 libCurl 库的简单封装(不能单独工作，需要结合HttpClient).
 *
 * 采用 RAII 思想封装,避免忘记资源释放.
 *
 * 关于 libcurl 的使用,可参考其官方文档:
 * https://curl.haxx.se/libcurl/c/
 *
 * libcurl 总的来说是基于回调的编程。
 * 在启动http请求前，设置好各类参数和回调方法，
 * 当满足条件时 libcurl 将回调设置的回调方法，以实现特定功能。
 */

class HttpRequest;
class HttpClient;

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

    typedef size_t (*writeCallback)(void *ptr, size_t size, size_t nmemb, void *stream);

    /**
     * @brief initCurl
     * libCurl 的简单初始化
     * @param request
     * @param callback
     * @param stream
     * @param header_callback
     * @param header_stream
     * @param error_buffer
     * @param is_https
     * @return
     */
    bool initCurl(const HttpRequest* request,
                  writeCallback callback,
                  void *stream,
                  writeCallback header_callback,
                  void *header_stream,
                  char *error_buffer,
                  bool is_https);

    /**
     * 此友元类访问下面的私有方法
     */
    friend class HttpClient;

  private:
    template<class T>
    bool setCurlOption(CURLoption option, T data);

    template<class T>
    bool getCurlInfo(CURLINFO option, T data);

    /**
     * @brief configureLibCurl
     * 配置 libcurl 链接和请求超时时间(30s)
     * 以及用于存储请求出错时的错误信息的缓冲区
     * @param curl
     * @param error_buffer
     * @return
     */
    bool configureLibCurl(CURL *curl, char *error_buffer);
    /**
     * @brief performLibCurl
     * 开始执行 http 请求
     * 注意，不要单独运行该方法，
     * 需结合 HttpClient中的具体请求，再辅以其他配置项才能运行
     * @param response_code
     * @return
     */
    bool performLibCurl(int64_t *response_code);

  private:
    DISALLOW_COPY_AND_ASSIGN(CurlWrapper);

    CURL* libCurl_;
    curl_slist* headers_;

};

NS_GODIN_END

#endif // CURLWRAPPER_H
