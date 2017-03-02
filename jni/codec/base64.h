/**
 * Project : weimi-native-sdk
 * FileName: base64.h
 * date    : 2017-03-01
 * time    : 14:23:48
 * author  : genglei.cuan@godinsec.com
 */

#ifndef BASE64_H
#define BASE64_H

#include <base/macros.h>
#include <string>
#include <cryptopp/base64.h>

NS_GODIN_BEGIN

/**
 * @brief The Base64 class
 * 标准的 Base64 编解码.
 *
 * 因为编码后的+/=字符，标准的Base64并不适合直接放在URL里传输,以及作为文件名字和文件夹名字等.
 * 如有上述特殊需求,可以使用CryptoPP::Base64URLEncoder相关类;
 * 当然也可以通过修改Base64Encoder的alphabets来达到类似目的.具体api,可以参考CryptoPP官方文档.
 *
 * 该类主要是为了满足KMS设计文档中对Https中的数据进行编码保护的需求.
 */

class Base64 {

  public:

    /**
     * @brief Base64
     * 初始化Base64Encoder,使其不在结果中插入"空行"
     */
    Base64();

    /**
     * @brief encode
     * 将输入的数据进行 base64 编码
     * @param input
     * @param size
     * @return
     */
    std::string encode(const uint8_t * input,size_t size);

    /**
     * @brief encode
     * 将输入的字符串进行 base64 编码
     * @param input
     * @return
     */
    std::string encode(const std::string &input);

    /**
     * @brief encodeFile
     * 使用标准的 base64 编码一个文件
     * @param inputFile
     * @param outFile
     * @return
     */
    bool encodeFile(const std::string &inputFile,const std::string &outFile);

    /**
     * @brief encode_vector
     * 将输入的 vector 数组进行 base64 编码
     * @param input
     * @return
     */
    template<typename T>
    inline std::string encodeVector(const std::vector<T>& input){
        return encode((uint8_t*)input.data(),input.size()*sizeof(T));
    }


    /**
     * @brief decodeToVectorUint8
     * 对base64编码的字符串进行解码，
     * 返回解码后的二进制数组
     * @param input
     * @return
     */
    std::vector<uint8_t> decodeToVectorUint8(const std::string &input);

    /**
     * @brief decodeToString
     * 对base64编码的字符串进行解码，
     * 返回解码后的字符串;
     *
     * 一定要输入的64编码字符串是有字符串编码生成的,
     * 否则输出的字符串可能是乱码.
     * @param input
     * @return
     */
    std::string decodeToString(const std::string &input);

    /**
     * @brief decodeVector
     * 对base64编码的字符串进行解码，
     * 返回编码前数据的类型数组.
     * @param input
     * @return
     */
    template<typename T>
    std::vector<T> decodeVector(const std::string&input){
      std::lock_guard<std::mutex> lg(decoder_mutex_);
      decoder_.Put((byte*)input.data(), input.size() );
      decoder_.MessageEnd();

      int64_t size = decoder_.MaxRetrievable();
      if(size && size <= SIZE_MAX && (size % sizeof(T)==0))
      {
          std::vector<T>buffer(size / sizeof(T));
          decoder_.Get((uint8_t*)buffer.data(), size);
          return buffer;
      }
      std::vector<T> ve;
      ve.resize(0);
      return ve;
    }

    bool decodeFile(const std::string &inputFile,const std::string &outFile);

  private:
    DISALLOW_COPY_AND_ASSIGN(Base64);
    CryptoPP::Base64Encoder encoder_;
    CryptoPP::Base64Decoder decoder_;
    std::mutex encoder_mutex_;
    std::mutex decoder_mutex_;
};

NS_GODIN_END

#endif // BASE64_H
