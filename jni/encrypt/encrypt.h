/**
 * Project : weimi-native-sdk
 * FileName: encrypt.h
 * date    : 2017-03-08
 * time    : 11:26:09
 * author  : genglei.cuan@godinsec.com
 */


#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <base/macros.h>
#include <string>

NS_GODIN_BEGIN


class Encrypt {
  public:
    Encrypt(){}
    struct Chunk_header {

        uint16_t type;

        uint16_t headerSize;

        uint32_t size;
    };

    /**
     * 描述 chunk
     */
    enum{

      ENCRYPTION_INFO_TYPE            = 0x1234,
    };

    /**
     * 描述加密算法
     */
    enum{

      ENCRYPTION_ALGORITHM_AES       = 0x0011,

    };
    enum{
      AES_CBC       = 0x0101,
    };

    struct EncInfo{
      struct Chunk_header header;
      /// 加密算法
      int encryption_type;

      /// 加密算法模式
      int encryption_mode;

      /// hash 算法,该字段暂时无用
      int hash_type;

      /// 加密算法分组大小
      int encryption_blcok_size;

      /// 加密数据分块加密的块大小
      /// 例如把一个文件分成几块加密,每块大小.
      int block_size;

      /// 块数量
      int block_num;

      /// 密文数据长度
      uint32_t cipher_len;
    };
    static bool encryptData(const std::string &inputfile, const std::string &output);
    static bool decryptData(const std::string &inputfile,
                            off_t offset,
                            const std::string &output,
                            size_t plaintText_len);
};

NS_GODIN_END
#endif // ENCRYPT_H
