/**
 * Project : weimi-native-sdk
 * FileName: hash_utils.cpp
 * date    : 2017-02-23
 * time    : 19:23:52
 * author  : genglei.cuan@godinsec.com
 */


#include "hash_utils.h"
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <cryptopp/sha.h>
#include <cryptopp/crc.h>
#include "file_utils.h"

bool godin::HashUtils::getFileSha1HexStr(const std::string &path, std::string &shavale) {

  if(!godin::FileUtils::isFileExists(path))
    return false;

  CryptoPP::SHA1 sha1;
  CryptoPP::FileSource(path.c_str(),true,
                       new CryptoPP::HashFilter(sha1,
                          new CryptoPP::HexEncoder(
                          new CryptoPP::StringSink(shavale)))
                       );
  return true;
}

bool godin::HashUtils::getFileSha1(const std::string &path, uint8_t *buf, int len) {
  if(buf == NULL || len == 0)
    return false;

  if(!godin::FileUtils::isFileExists(path))
    return false;

  CryptoPP::SHA1 sha1;
  CryptoPP::FileSource(path.c_str(),true,
                       new CryptoPP::HashFilter(sha1,
                          new CryptoPP::ArraySink(buf,len))
                       );
  return true;
}

bool godin::HashUtils::getBlockSha1HexStr(uint8_t *data, size_t size, std::string &shavale) {

  if(data == NULL || size == 0)
    return false;

  CryptoPP::SHA1 sha1;
  CryptoPP::StringSource(data,size,true,
                         new CryptoPP::HashFilter(sha1,
                            new CryptoPP::HexEncoder(
                            new CryptoPP::StringSink(shavale))));
  return true;
}

bool godin::HashUtils::getBlockSha1(uint8_t *data, size_t size, uint8_t *buf, int len) {

  if(data == NULL || size == 0)
    return false;
  if(data == buf || len == 0)
    return false;

  CryptoPP::SHA1 sha1;
  CryptoPP::StringSource(data,size,true,
                         new CryptoPP::HashFilter(sha1,
                            new CryptoPP::ArraySink(buf,len)));
  return true;
}

bool godin::HashUtils::getBlockCrc32HexStr(uint8_t *data, size_t size, std::string &crcvalue) {

  if(data == NULL || size == 0)
    return false;

  CryptoPP::CRC32 crc;
  CryptoPP::StringSource(data,size,true,
                         new CryptoPP::HashFilter(crc,
                            new CryptoPP::HexEncoder(
                            new CryptoPP::StringSink(crcvalue))));
  return true;
}

bool godin::HashUtils::getBlockCrc32(uint8_t *data, size_t size, uint8_t *buf, int len) {

  CryptoPP::CRC32 crc;

  if(data == NULL || size == 0)
    return false;
  if(data == buf || len < crc.DigestSize())
    return false;

  crc.Update(data,size);
  crc.TruncatedFinal(buf,len);

  return true;
}
