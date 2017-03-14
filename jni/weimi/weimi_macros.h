/**
 * Project : weimi-native-sdk
 * FileName: weimi_macros.h
 * date    : 2017-03-06
 * time    : 15:25:43
 * author  : genglei.cuan@godinsec.com
 */


#ifndef WEIMI_MACROS_H
#define WEIMI_MACROS_H

#define MAGIC_STR "WeimeEnc"

#define MAGIC_STR_LEN 8

#define MAJOR_VERSION 1

#define SECOND_VERSION 0

#define DES_CONTENT "此文件由weimi加密.\n"

#define COMMODITY_NUM_LEN 32

/// 保证是4的倍数
#define MAGIC_LEN 8

/// 必须是20
#define SHA1_HEX_LEN  20

/// 必须是4
#define CRC_32_BYTES 4



/// 文件后缀最大长度
#define FILE_SUFFIX_LEN 8

#endif // WEIMI_MACROS_H
