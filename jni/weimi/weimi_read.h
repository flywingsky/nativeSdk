/**
 * Project : weimi-native-sdk
 * FileName: weimi_read.h
 * date    : 2017-03-09
 * time    : 11:11:15
 * author  : genglei.cuan@godinsec.com
 */

#ifndef WEIMIREAD_H
#define WEIMIREAD_H

#include <base/macros.h>
#include <file/file.h>
#include "wei_mi_struct.h"

NS_GODIN_BEGIN

class WeimiRead {

  public:
    WeimiRead():is_init_(false),file_(NULL),file_info_(NULL),welcome_header_(NULL),
                commodity_header_(NULL),plaintext_header_(NULL),cipher_header_(NULL){}

    virtual ~WeimiRead();

    bool init(const std::string & path);

    /**
     * @brief isWeimiFile
     * 检查一个文件是否是weimi文件.
     * weimi 文件前8个字节是"WeimeEnc".
     *
     * 大小写敏感.
     *
     * @return
     */
    bool isWeimiFile();

    bool parseWeimiFile();

    WeiMiInfo *getFileInfo() const { return file_info_;}

    WeiMiWelcomeInfo_header *getWelcomeHeader() const{return welcome_header_;}

    WeiMiCommodityInfo_header *getCommodityHeader() const{return commodity_header_;}

    WeiMiPlaintextInfo_header *getPlaintextHeader() const{return plaintext_header_;}

    WeiMiCipherInfo_header *getCipherHeader() const{return cipher_header_;}


private:
    DISALLOW_COPY_AND_ASSIGN(WeimiRead);
    godin::File *file_;
    bool is_init_;
    struct WeiMiInfo * file_info_;
    struct WeiMiWelcomeInfo_header* welcome_header_;
    struct WeiMiCommodityInfo_header* commodity_header_;
    struct WeiMiPlaintextInfo_header* plaintext_header_;
    struct WeiMiCipherInfo_header* cipher_header_;
};

NS_GODIN_END
#endif // WEIMIREAD_H
