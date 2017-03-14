/**
 * Project : weimi-native-sdk
 * FileName: wei_mi_struct.h
 * date    : 2017-03-06
 * time    : 15:21:43
 * author  : genglei.cuan@godinsec.com
 */


#ifndef WEI_MI_STRUCT_H
#define WEI_MI_STRUCT_H

#include <base/macros.h>
#include <stdio.h>
#include "weimi_macros.h"



NS_GODIN_BEGIN

enum{
  BIG_END     = 0,
  LITTLE_END  = 1,
};

enum{
  UTF_8    = 0,
  UTF_16   = 1,
};

struct WeiMiInfo {
  char magic_num[MAGIC_LEN];     ///< 魔数
  uint16_t major_num;            ///< 主版本号
  uint16_t second_num;           ///< 次版本号
  int  big_or_little;            ///< 大小端
  int  info_len;                 ///< info 大小
  //int  file_len;                 ///< 文件 大小
};

struct WeiMiChunk_header {

    uint16_t type;

    uint16_t headerSize;

    uint32_t size;
};

/**
 * 描述 chunk
 */
enum{

  /// weimi 加密文件的欢迎消息
  WEI_MI_FILE_WELCOME_SECTION            = 0x0000,

  /// weimi 秘钥商品信息
  WEI_MI_FILE_COMMODITY_INFO_SECTION     = 0x0001,

  /// weimi 加密文件的明文信息
  WEI_MI_FILE_PLAINTEXT_SECTION          = 0x0002,

  /// weimi 加密文件的密文信息
  WEI_MI_FILE_ENC_SECTION                = 0x0003,

};

/// weimi 欢迎消息,包括 header+body
struct WeiMiWelcomeInfo_header {
  struct WeiMiChunk_header header;
  int encode_type;        ///< 字符串编码格式
  int fill_lenth;         ///< 需要补齐的字节数
};

/// weimi 秘钥商品信息,没有body
struct WeiMiCommodityInfo_header {
  struct WeiMiChunk_header header;
  ///int encode_type;                         ///< 字符串编码格式
  char serial_num[COMMODITY_NUM_LEN];         ///< 商品编号
  uint8_t serial_num_crc[CRC_32_BYTES];    ///> 商品编号CRC校验
};

/// weimi 秘钥商品信息,没有body
struct WeiMiPlaintextInfo_header {
  struct WeiMiChunk_header header;
  ///int encode_type;                      ///< 字符串编码格式
  uint32_t plaintext_len;                  ///< 明文文件长度
  char file_suffix[FILE_SUFFIX_LEN];       ///< 明文文件后缀
  uint32_t fek_id;                         ///< 秘钥ID
};

/// weimi 秘钥加密信息,包括 header+body
struct WeiMiCipherInfo_header {
  struct WeiMiChunk_header header;
  uint32_t offset; ///密文数据相对文件头的偏移量
};


NS_GODIN_END

#endif // WEI_MI_STRUCT_H
