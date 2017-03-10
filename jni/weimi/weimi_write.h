/**
 * Project : weimi-native-sdk
 * FileName: weimi_write.h
 * date    : 2017-03-06
 * time    : 16:44:05
 * author  : genglei.cuan@godinsec.com
 */


#ifndef WEIMIWRITE_H
#define WEIMIWRITE_H

#include <base/macros.h>
#include "wei_mi_struct.h"
#include <file/file.h>
#include <string>

NS_GODIN_BEGIN

class WeimiWrite {
  public:

    WeimiWrite():is_init_(false),
      welcome_offset_(0),
    commodity_offset_(0),
    plainttext_offset_(0),
    cipher_offset_(0){}

    virtual ~WeimiWrite();

    /**
     * @brief init
     * 初始化;
     * 传入的路径代表的文件不存在时,自动创建.
     * 已经存在时,清空文件的内容.
     * @param path
     * @return
     */
    bool init(const std::string & path);

    bool writeFileInfo(struct WeiMiInfo* info);

    bool writeWelcomeSection(struct WeiMiWelcomeInfo_header* info,char* body,int len);

    bool writeCommoditySection(struct WeiMiCommodityInfo_header* info);

    bool writePlaintextSection(struct WeiMiPlaintextInfo_header* info);

    bool writeCipherSection(struct WeiMiCipherInfo_header* info);


  private:
    DISALLOW_COPY_AND_ASSIGN(WeimiWrite);
    godin::File *file_;
    bool is_init_;

    off_t welcome_offset_;
    off_t commodity_offset_;
    off_t plainttext_offset_;
    off_t cipher_offset_;


};


NS_GODIN_END

#endif // WEIMIWRITE_H
