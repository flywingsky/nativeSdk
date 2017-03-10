/**
 * Project : weimi-native-sdk
 * FileName: weimi_read.cpp
 * date    : 2017-03-09
 * time    : 11:13:10
 * author  : genglei.cuan@godinsec.com
 */


#include "weimi_read.h"
#include <utils/file_utils.h>
#include "weimi_macros.h"
#include <unistd.h>
#include <string.h>
#include <utils/log.h>

godin::WeimiRead::~WeimiRead() {
  if(file_ != NULL)
    delete file_;

  if(file_info_ != NULL)
    free(file_info_);
  if(welcome_header_ != NULL)
    free(welcome_header_);
  if(commodity_header_ != NULL)
    free(commodity_header_);
  if(plaintext_header_ != NULL)
    free(plaintext_header_);
  if(cipher_header_ != NULL)
    free(cipher_header_);

  file_info_ = NULL;
  welcome_header_ = NULL;
  commodity_header_ = NULL;
  plaintext_header_ = NULL;
  cipher_header_ = NULL;
}

bool godin::WeimiRead::init(const std::string &path) {
  if(!is_init_){
      file_ = new godin::File(path);
      if(file_->open(godin::File::ReadOnly)){
        is_init_ = true;
        return true;
      }else{
        is_init_ = false;
        file_->close();
        return false;
      }
  }
  return true;
}

bool godin::WeimiRead::isWeimiFile() {
  if(is_init_){
    char buf[MAGIC_STR_LEN]={0};
    off_t offset = file_->getCurrentOffset();
    file_->lseekOfStart(0);
    if(::read(file_->getFd(),buf,MAGIC_STR_LEN) == -1)
      return false;
    file_->lseekOfStart(offset);
    if(::strncmp(buf,MAGIC_STR,MAGIC_STR_LEN) == 0)
      return true;
    else
      return false;
  }
  return false;
}

bool godin::WeimiRead::parseWeimiFile() {
  if(is_init_ && isWeimiFile()){

      size_t fileSize = file_->fileSize();
      int min_len = sizeof(struct WeiMiInfo)
                    + sizeof(struct WeiMiWelcomeInfo_header)
                    + sizeof(struct WeiMiCommodityInfo_header)
                    + sizeof(struct WeiMiPlaintextInfo_header)
                    + sizeof(struct WeiMiCipherInfo_header);

      if(fileSize < min_len){
         godin::Log::e("parse weme file failed,this is a damaged file.");
         return false;
      }
      uint8_t* data = file_->mmap((fileSize < 4096 ? fileSize : 4096),0,true);
      if(data == NULL)
        return false;

      if(file_info_!=NULL)
        free(file_info_);
      file_info_ = (struct WeiMiInfo *)::malloc(sizeof(struct WeiMiInfo));
      ::memcpy(file_info_,(void*)data,sizeof(struct WeiMiInfo));
      data = (uint8_t*)data + file_info_->info_len;

      bool parse_complete = false;
      while(!parse_complete){
        WeiMiChunk_header * chunk = (WeiMiChunk_header*)data;
        switch(chunk->type){
          case WEI_MI_FILE_WELCOME_SECTION:
            if(welcome_header_ != NULL)
              free(welcome_header_);
            welcome_header_ = (struct WeiMiWelcomeInfo_header* )::malloc(sizeof(struct WeiMiWelcomeInfo_header));
            ::memcpy(welcome_header_,(void*)chunk,sizeof(struct WeiMiWelcomeInfo_header));
            data = (uint8_t*)chunk + welcome_header_->header.size;
            break;
          case WEI_MI_FILE_COMMODITY_INFO_SECTION:
            if(commodity_header_ != NULL)
              free(commodity_header_);
            commodity_header_ = (struct WeiMiCommodityInfo_header* )::malloc(sizeof(struct WeiMiCommodityInfo_header));
            ::memcpy(commodity_header_,(void*)chunk,sizeof(struct WeiMiCommodityInfo_header));
            data = (uint8_t*)chunk + commodity_header_->header.size;
            break;
          case WEI_MI_FILE_PLAINTEXT_SECTION:
            if(plaintext_header_ != NULL)
              free(plaintext_header_);
            plaintext_header_ = (struct WeiMiPlaintextInfo_header*)::malloc(sizeof(struct WeiMiPlaintextInfo_header));
            ::memcpy(plaintext_header_,(void*)chunk,sizeof(struct WeiMiPlaintextInfo_header));
            data = (uint8_t*)chunk + plaintext_header_->header.size;
            break;
          case WEI_MI_FILE_ENC_SECTION:
            if(cipher_header_ != NULL)
              free(cipher_header_);
            cipher_header_ = (struct WeiMiCipherInfo_header*)::malloc(sizeof(struct WeiMiCipherInfo_header));
            ::memcpy(cipher_header_,(void*)chunk,sizeof(struct WeiMiCipherInfo_header));
            parse_complete = true;
            break;
          default:
            godin::Log::e("find unsupport section in file or this is a damaged file.");
            file_->munmap();
            return false;
           }
        }
      file_->munmap();
      if(parse_complete)
        return true;
      else
        return false;
  }
  return false;
}


