/**
 * Project : weimi-native-sdk
 * FileName: http_client.h
 * date    : 2017-02-15
 * time    : 15:05:07
 * author  : genglei.cuan@godinsec.com
 */


#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <base/macros.h>
#include "http_request.h"
#include "http_response.h"

#include <thread>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <mutex>


NS_GODIN_BEGIN

class HttpClient {


  public:

    HttpClient(){}
   /**
    * @brief getInstanceClient
    * 获取一个全局唯一的客户端实例,
    * 适合用于异步发送请求.
    * 最好不要将该实例用于同步发送请求.
    * @return
    */
    static HttpClient* getInstanceClient();
    /**
     * @brief freeInstanceClient
     * 释放全局唯一的客户端实例资源
     */
    static void freeInstanceClient();

    /**
     * @brief asyncSend
     * 异步发送请求,立即返回
     * 所谓异步请求,是指在一个
     * 新线程中执行请求.
     * @param request
     * @param callback
     */
    void asyncSend(std::unique_ptr<HttpRequest> request,
              std::function<void(std::unique_ptr<HttpResponse>)> callback);

    /**
     * @brief syncSend
     * 同步发送请求,阻塞直到请求执行完毕
     * @param request
     * @param callback
     */
    void syncSend(std::unique_ptr<HttpRequest> request,
              std::function<void(std::unique_ptr<HttpResponse>)> callback);


    /**
     * @brief cancelAllRequests
     * 取消所有请求
     */
    void cancelAllRequests();

  private:

    /**
     * @brief removeRequest
     * 从请求队列中删除一个请求
     *
     * @param identifer
     * 请求的标识,即 url+请求时的时间戳
     *
     * @return
     * true: 删除成功
     * false: 请求没在队列中
     */
    bool removeRequest(const std::string& identifer);

    /**
     * @brief isCancelledRequest
     * 该请求是否被取消,即从请求队列中移除了.
     *
     * @param identifer
     *
     * @return
     *
     * true: 已经取消
     * false: 没有取消
     */
    bool isCancelledRequest(const std::string& identifer) const;


    /**
     * @brief performHttpRequest
     * 执行 http 请求,
     * 内部会调用performHttpResponseCallback(),
     * 执行请求成功后对respon的处理回调
     * @param request
     * @param callback
     */
    void performHttpRequest(std::unique_ptr<HttpRequest> request,
                            std::function<void(std::unique_ptr<HttpResponse>)> callback);

    /**
     * @brief performHttpResponseCallback
     * 调用用来处理 http 服务器返回的 respon 消息的回调方法
     * @param response
     * @param callback
     */
    void performHttpResponseCallback(std::unique_ptr<HttpResponse> response,
                                    std::function<void(std::unique_ptr<HttpResponse>)> callback);

  private:
    DISALLOW_COPY_AND_ASSIGN(HttpClient);
    static HttpClient* shared_client_;
    std::mutex request_queue_mutex_;
    std::vector<std::string> request_queue_;


};

NS_GODIN_END

#endif // HTTPCLIENT_H
