/**
 * Project : weimi-native-sdk
 * FileName: hash_utils.h
 * date    : 2017-02-23
 * time    : 19:23:36
 * author  : genglei.cuan@godinsec.com
 */


#ifndef HASHUTILS_H
#define HASHUTILS_H

#include <base/macros.h>
#include <string>

NS_GODIN_BEGIN

/**
 * @brief The HashUtils class
 *
 * 使用 cryptopp 计算 hash
 */

class HashUtils {

  public:

    static bool getFileSha1HexStr(const std::string &path,std::string &shavale);

    static bool getFileSha1(const std::string &path,uint8_t *buf,int len);

    static bool getBlockSha1HexStr(uint8_t* data,size_t size,std::string &shavale);

    static bool getBlockSha1(uint8_t* data,size_t size,uint8_t* buf,int len);

    static bool getBlockCrc32HexStr(uint8_t* data,size_t size,std::string &crcvalue);

    static bool getBlockCrc32(uint8_t* data,size_t size,uint8_t* buf,int len);

  private:
    HashUtils(){}
};

NS_GODIN_END

#endif // HASHUTILS_H
