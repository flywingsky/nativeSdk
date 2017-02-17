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
  static HttpClient* getInstanceClient();
  static void freeInstanceClient();

  private:
    HttpClient(){}

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


    void performHttpRequest(std::unique_ptr<HttpRequest> request,
                            std::function<void(std::unique_ptr<HttpResponse>)> callback);

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
