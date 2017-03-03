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
    bool setEncryptorKey(const uint8_t *key, int length,const uint8_t *iv, int iv_length);

    /**
     * @brief setDecryptorKey
     * 设置解密模块的 key 和 iv.
     * @param key
     * @param length
     * @param iv
     * @param iv_length
     * @return
     */
    bool setDecryptorKey(const uint8_t *key, int length, const uint8_t *iv, int iv_length);

    /**
     * @brief setDefaultKey
     * 用默认的 key 和 iv 初始化加密和解密模块
     */
    void setDefaultKey();


    /**
     * @brief encryptStr
     * 加密传入的字符串
     * @param plainText
     * @return
     * 失败时,返回空字符串
     */
    std::string encryptStr(const std::string &plainText);

    /**
     * @brief decryptStr
     * 解密传入的字符串
     * @param cipher
     * @return
     * 失败时,返回空字符串
     */
    std::string decryptStr(const std::string &cipher);

    /**
     * @brief encryBlock
     * 加密一个数据块
     *
     * @param plain
     * 明文数据块起始地址
     * @param size
     * 明文数据块字节数
     *
     * @param buf
     * 存放密文的起始地址
     * @param len
     * 密文的字节数
     *
     * @return
     * true:成功
     * false:失败
     */
    bool encryBlock(uint8_t* plain,size_t size,uint8_t * buf,size_t len);

    /**
     * @brief decryptBlock
     * 解密一个数据块
     *
     * @param cipher
     * 密文数据块起始地址
     * @param size
     * 密文数据块字节数
     *
     * @param buf
     * 存放解密后的数据的起始地址
     * @param len
     * 解密后的数据的字节数
     *
     * @return
     * true:成功
     * false:失败
     */
    bool decryptBlock(uint8_t* cipher,size_t size,uint8_t * buf,size_t len);

    /**
     * @brief encryptFile
     * 加密一个文件
     *
     * @param inFilename
     * @param outFilename
     *
     * @return
     * 待加密文件如果不存在,返回false.
     * 生成的文件已经存在的话,返回false.即不会覆盖文件.
     * 加密成功返回 true;
     */
    bool encryptFile(const std::string & inFilename, const std::string & outFilename);

    /**
     * @brief decryptFile
     * 解密一个文件
     * @param decFilename
     * @param recoverFilename
     * @return
     * 待解密文件如果不存在,返回false.
     * 生成的文件已经存在的话,返回false.即不会覆盖文件.
     * 解密成功返回 true;
     */
    bool decryptFile(const std::string & decFilename,
                    const std::string & recoverFilename);

    /**
     * @brief encryptFileToBuf
     * 加密一个文件,将生成的密文存放到内存buffer中
     * @param inFilename
     * @param buf
     * @param len
     * @return
     * 待解密文件如果不存在,返回false.
     * 待解密文件大小比len大时,也返回false.
     */
    bool encryptFileToBuf(const std::string & inFilename,uint8_t * buf,size_t len);

    /**
     * @brief decryptBufToFile
     * 将内存中的密文解密到文件中
     * @param cipher
     * @param len
     * @param outFilename
     * @return
     * 生成的文件已经存在,返回false,即不会覆盖文件.
     */
    bool decryptBufToFile(uint8_t * cipher, size_t len, std::string outFilename);

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
