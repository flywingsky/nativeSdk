/**
 * Project : weimi-native-sdk
 * FileName: aes_engine.cpp
 * date    : 2017-03-02
 * time    : 19:28:57
 * author  : genglei.cuan@godinsec.com
 */


#include "aes_engine.h"
#include "default_key.h"
#include <cryptopp/osrng.h>
#include <cryptopp/ccm.h>
#include <cryptopp/files.h>
#include <utils/log.h>
#include <utils/file_utils.h>



bool godin::AesEngine::setEncryptorKey(const uint8_t *key, int length,const uint8_t *iv, int iv_length) {

  if(key == NULL || iv == NULL)
    return false;

  if(length != CryptoPP::AES::DEFAULT_KEYLENGTH)
    return false;
  if(iv_length != CryptoPP::AES::BLOCKSIZE)
    return false;

  std::lock_guard<std::mutex> lg(encryptor_mutex_);
  aesEncryptor_.SetKeyWithIV(key,length,iv,iv_length);

  is_set_en_key_ = true;

  return true;
}

bool godin::AesEngine::setDecryptorKey(const uint8_t *key, int length,const uint8_t *iv, int iv_length) {

  if(key == NULL || iv == NULL)
    return false;

  if(length != CryptoPP::AES::DEFAULT_KEYLENGTH)
    return false;
  if(iv_length != CryptoPP::AES::BLOCKSIZE)
    return false;

  std::lock_guard<std::mutex> lg(decryptor_mutex_);
  aesDecryptor_.SetKeyWithIV(key,length,iv,iv_length);

  is_set_de_key_ = true;

  return true;
}


void godin::AesEngine::setDefaultKey() {

  setEncryptorKey(deafault_aesKey,CryptoPP::AES::DEFAULT_KEYLENGTH,deafault_aesIv,CryptoPP::AES::BLOCKSIZE);
  setDecryptorKey(deafault_aesKey,CryptoPP::AES::DEFAULT_KEYLENGTH,deafault_aesIv,CryptoPP::AES::BLOCKSIZE);

}

std::string godin::AesEngine::encryptStr(const std::string &plainText) {

  std::string cipher;

  if(!is_set_en_key_)
    return "";

  std::lock_guard<std::mutex> lg(encryptor_mutex_);
  try{
    CryptoPP::StringSource(plainText, true,
                    new CryptoPP::StreamTransformationFilter(aesEncryptor_,
                                    new CryptoPP::StringSink(cipher)));
  }catch(const CryptoPP::Exception& e){
    godin::Log::e("encrypt str err:%s",e.GetWhat().c_str());
    return "";
  }

  return cipher;
}

std::string godin::AesEngine::decryptStr(const std::string &cipher) {

  std::string recover;

  if(!is_set_de_key_)
    return "";

  std::lock_guard<std::mutex> lg(decryptor_mutex_);
  try{
    CryptoPP::StringSource(cipher, true,
                    new CryptoPP::StreamTransformationFilter(aesDecryptor_,
                                    new CryptoPP::StringSink(recover)));
  }
  catch(const CryptoPP::Exception& e){
    godin::Log::e("decrypt str err:%s",e.GetWhat().c_str());
    return "";
  }
  return recover;
}

bool godin::AesEngine::encryBlock(uint8_t *plain, size_t size, uint8_t *buf, size_t len) {

  if(!is_set_en_key_)
    return false;

  if(plain == NULL || buf == NULL || size == 0 || len == 0)
    return false;

  std::lock_guard<std::mutex> lg(encryptor_mutex_);
  try{
    CryptoPP::StringSource(plain,size, true,
                    new CryptoPP::StreamTransformationFilter(aesEncryptor_,
                                    new CryptoPP::ArraySink(buf,len)));
  }catch(const CryptoPP::Exception& e){
    godin::Log::e("encrypt block err:%s",e.GetWhat().c_str());
    return false;
  }
  return true;
}

bool godin::AesEngine::decryptBlock(uint8_t *cipher, size_t size, uint8_t *buf, size_t len) {
  if(!is_set_de_key_)
    return false;

  if(cipher == NULL || buf == NULL || size == 0 || len == 0)
    return false;
  std::lock_guard<std::mutex> lg(decryptor_mutex_);
  try{
    CryptoPP::StringSource(cipher, size,true,
                    new CryptoPP::StreamTransformationFilter(aesDecryptor_,
                                    new CryptoPP::ArraySink(buf,len)));
  }
  catch(const CryptoPP::Exception& e){
    godin::Log::e("decrypt block err:%s",e.GetWhat().c_str());
    return false;
  }
  return true;
}

bool godin::AesEngine::encryptFile(const std::string &inFilename, const std::string &outFilename) {

  if(!is_set_en_key_)
    return false;

  if(!godin::FileUtils::isFileExists(inFilename)) {
     godin::Log::e("encrypt file: %s is not exists.",inFilename.c_str());
     return false;
  }

  if(godin::FileUtils::isFileExists(outFilename)) {
     godin::Log::e("cipher file: %s is already exists. please remove it before.",outFilename.c_str());
     return false;
  }

  std::lock_guard<std::mutex> lg(encryptor_mutex_);
  try{
    CryptoPP::FileSource(inFilename.c_str(), true,
                    new CryptoPP::StreamTransformationFilter(aesEncryptor_,
                                    new CryptoPP::FileSink(outFilename.c_str())));
  }
  catch(const CryptoPP::Exception& e){
    godin::Log::e("encrypt file err:%s",e.GetWhat().c_str());
    return false;
  }
  return true;

}

bool godin::AesEngine::decryptFile(const std::string &decFilename, const std::string &recoverFilename) {
  if(!is_set_de_key_)
    return false;

  if(!godin::FileUtils::isFileExists(decFilename)) {
     godin::Log::e("decrypt file: %s is not exists.",decFilename.c_str());
     return false;
  }

  if(godin::FileUtils::isFileExists(recoverFilename)) {
     godin::Log::e("recover file: %s is already exists. please remove it before.",recoverFilename.c_str());
     return false;
  }

  std::lock_guard<std::mutex> lg(decryptor_mutex_);
  try{
    CryptoPP::FileSource(decFilename.c_str(), true,
                    new CryptoPP::StreamTransformationFilter(aesDecryptor_,
                                    new CryptoPP::FileSink(recoverFilename.c_str())));
  }
  catch(const CryptoPP::Exception& e){
    godin::Log::e("decrypt file err:%s",e.GetWhat().c_str());
    return false;
  }
  return true;
}

bool godin::AesEngine::encryptFileToBuf(const std::string &inFilename, uint8_t *buf, size_t len) {

  if(!is_set_en_key_)
    return false;

  if(!godin::FileUtils::isFileExists(inFilename)) {
     godin::Log::e("encrypt file: %s is not exists.",inFilename.c_str());
     return false;
  }

  if(godin::FileUtils::getFileSize(inFilename) < len){
      godin::Log::e("encrypt file size is  larger than the buffer space to store cipher.");
      return false;
  }

  std::lock_guard<std::mutex> lg(encryptor_mutex_);
  try{
    CryptoPP::FileSource(inFilename.c_str(), true,
                    new CryptoPP::StreamTransformationFilter(aesEncryptor_,
                                    new CryptoPP::ArraySink(buf,len)));
  }catch(const CryptoPP::Exception& e){
    godin::Log::e("encrypt file to buffer err:%s",e.GetWhat().c_str());
    return false;
  }

  return true;
}

bool godin::AesEngine::decryptBufToFile(uint8_t *cipher, size_t len, std::string outFilename) {

  if(!is_set_de_key_)
    return false;

  if(godin::FileUtils::isFileExists(outFilename)) {
     godin::Log::e("decrypt buffer to the file: %s is already exists.",outFilename.c_str());
     return false;
  }

  if(cipher == NULL || len == 0)
    return false;

  std::lock_guard<std::mutex> lg(decryptor_mutex_);
  try{
    CryptoPP::StringSource(cipher, len,true,
                    new CryptoPP::StreamTransformationFilter(aesDecryptor_,
                                    new CryptoPP::FileSink(outFilename.c_str())));
  }
  catch(const CryptoPP::Exception& e){
    godin::Log::e("decrypt file to buffer err:%s",e.GetWhat().c_str());
    return false;
  }
  return true;

}


