/**
 * Project : weimi-native-sdk
 * FileName: aes_engine.h
 * date    : 2017-03-02
 * time    : 19:28:44
 * author  : genglei.cuan@godinsec.com
 */

#ifndef AESENGINE_H
#define AESENGINE_H

#include <base/macros.h>
#include <mutex>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>


NS_GODIN_BEGIN

/**
 * @brief The AesEngine class
 * CBC 模式下 AES 算法封装
 */
class AesEngine {

  public:
    AesEngine():is_set_en_key_(false),is_set_de_key_(false){}

    /**
     * @brief setEncryptorKey
     * 设置加密模块0的 key 和 iv.
     * @param key
     * @param length
     * @param iv
     * @param iv_length
     * @return
     */
    bool setEncryptorKey(uint8_t *key, int length, uint8_t *iv, int iv_length);

    /**
     * @brief setDecryptorKey
     * 设置解密模块的 key 和 iv.
     * @param key
     * @param length
     * @param iv
     * @param iv_length
     * @return
     */
    bool setDecryptorKey(uint8_t *key, int length, uint8_t *iv, int iv_length);

    /**
     * @brief setDefaultKey
     * 用默认的 key 和 iv 初始化加密和解密模块
     */
    void setDefaultKey();




  private:
    DISALLOW_COPY_AND_ASSIGN(AesEngine);
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption aesEncryptor_;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption aesDecryptor_;
    std::mutex encryptor_mutex_;
    std::mutex decryptor_mutex_;
    bool is_set_en_key_;
    bool is_set_de_key_;
};

NS_GODIN_END

#endif // AESENGINE_H
