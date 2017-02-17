/**
 * Project : weimi-native-sdk
 * FileName: http_request.h
 * date    : 2017-02-15
 * time    : 17:26:35
 * author  : genglei.cuan@godinsec.com
 */


#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

#include <base/macros.h>

NS_GODIN_BEGIN

class HttpRequest {

  friend class HttpClient;

  public:
    /**
     * @brief The Type enum
     * 请求的类型
     */
    enum Type {
      GET,
      POST,
      PUT,
      DELETE,
      UNKNOWN
    };

  public:
    HttpRequest();
    virtual ~HttpRequest() {}


    Type requestType() const{ return request_type_;}
    void setRequestType(const Type &request_type){ request_type_ = request_type;}

    std::string url() const{ return url_;}
    void setUrl(const std::string &url){ url_ = url;}

    std::map<std::string, std::string> requestHeader() const{ return request_header_;}
    void setRequestHeader(const std::map<std::string, std::string> &request_header){ request_header_ = request_header;}

    std::string requestData() const{ return request_data_;}
    void setRequestData(const std::string &request_data){ request_data_ = request_data;}

    std::string identifier() const{ return identifier_;}
    void setIdentifier(const std::string &identifier){ identifier_ = identifier;}


    /**
     * @brief Clone
     * 只能通过该方法获得该对象的新的拷贝实例
     *
     * @return
     */
    std::unique_ptr<HttpRequest> Clone() const;

    /**
     * @brief StringFromHttpRequestType
     * 将 Type 转换为对应的String,如"GET,POST"等
     *
     * @param type
     * @return
     */
    static std::string stringFromHttpRequestType(HttpRequest::Type type);

    /**
     * @brief addValueToHttpHeaderField
     * 向 http 请求的 header 添加属性
     * @param key
     * @param value
     */
    void addValueToHttpHeaderField(const std::string& key, const std::string& value);

    /**
     * @brief setValueForHttpHeaderField
     * 重新设置 http 请求中 header 中某个属性的值
     * @param key
     * @param value
     */
    void setValueForHttpHeaderField(const std::string& key, const std::string& value);

    /**
     * @brief getValueForHttpHeaderField
     * 获取 http 请求中 header 中某个属性的值
     * @param key
     * @return
     */
    std::string  getValueForHttpHeaderField(const std::string& key);

  private:

    /**
     * 禁止构造拷贝和赋值拷贝
     */
    DISALLOW_COPY_AND_ASSIGN(HttpRequest);

    Type request_type_;         ///< 请求类型
    std::string url_;           ///< 请求的url
    std::map<std::string, std::string> request_header_; ///< header
    std::string request_data_;  ///<  body
    std::string identifier_;    ///< 请求的标识符,url+请求时的时间戳
    /**
     * @brief raw_data_
     * 在真正进行 http 请求时,
     * 需将 string 类型的 request_data_ 转换为 char 数组.
     */
    std::vector<char> raw_data_;

    /**
     * @brief createIdentifier
     * 仅供友元类 HttpClient 调用
     * @return
     */
    std::string createIdentifier() const;


};

NS_GODIN_END

#endif // HTTPREQUEST_H
