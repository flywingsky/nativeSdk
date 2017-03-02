/**
 * Project : weimi-native-sdk
 * FileName: aes_engine.cpp
 * date    : 2017-03-02
 * time    : 19:28:57
 * author  : genglei.cuan@godinsec.com
 */


#include "aes_engine.h"
#include "default_key.h"


bool godin::AesEngine::setEncryptorKey(uint8_t *key, int length, uint8_t *iv, int iv_length) {

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

bool godin::AesEngine::setDecryptorKey(uint8_t *key, int length, uint8_t *iv, int iv_length) {

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
