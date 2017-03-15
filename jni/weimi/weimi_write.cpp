/**
 * Project : weimi-native-sdk
 * FileName: weimi_write.cpp
 * date    : 2017-03-07
 * time    : 20:08:22
 * author  : genglei.cuan@godinsec.com
 */


#include "weimi_write.h"
#include <utils/file_utils.h>
#include <utils/log.h>

godin::WeimiWrite::~WeimiWrite() {
  if(file_->isOpen())
    file_->close();
  if(file_ != nullptr)
    delete file_;
}

bool godin::WeimiWrite::init(const std::string &path) {

  if(!is_init_){
      if(!godin::FileUtils::isFileExists(path)){
          if(!godin::FileUtils::createFile(path)){
              return false;
            }
        }
      file_ = new godin::File(path);
      if(file_->open(godin::File::Truncate)){
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

bool godin::WeimiWrite::writeFileInfo(godin::WeiMiInfo *info) {

  if(!is_init_ || info == nullptr)
    return false;

  /// 将文件偏移设置到文件起始处
  if(file_->lseekOfStart(0) == -1){
    godin::Log::e(" writeFileInfo err: lseek failed.");
    return false;
  }

  if(!file_->write((void*)info,info->info_len)){
      welcome_offset_ = info->info_len;
      return true;
  }
  else{
      welcome_offset_ = 0;
      return false;
    }


}

bool godin::WeimiWrite::writeWelcomeSection(godin::WeiMiWelcomeInfo_header *info, char *body, int len) {

  if(!is_init_ || info == nullptr || body == nullptr)
    return false;

  if(welcome_offset_ == 0){
      godin::Log::e("write welcome section failed,please write file info before.");
      return false;
  }

  if(info->header.headerSize + info->fill_lenth + len != info->header.size){
      godin::Log::e("write welcome section failed,fill length err.");
      return false;
  }

  if(file_->lseekOfStart(welcome_offset_) == -1){
      godin::Log::e("writeWelcomeInfo err: lseek failed.");
      return false;
  }

  if(!file_->write((void*)info,info->header.headerSize))
      return false;

  if(!file_->write((void*)body,len)){
      commodity_offset_ = 0;
      return false;
  }
  else{
      commodity_offset_ = welcome_offset_ + info->header.size;
      return true;
  }


}

bool godin::WeimiWrite::writeCommoditySection(godin::WeiMiCommodityInfo_header *info) {

  if(!is_init_ || info == nullptr)
    return false;

  if(commodity_offset_ == 0){
    godin::Log::e(" write commodity section failed,please write welcome section first.");
    return false;
  }
  if(file_->lseekOfStart(commodity_offset_) == -1){
      godin::Log::e(" writeCommoditySection err: lseek failed.");
      return false;
  }

  if(!file_->write((void*)info,info->header.headerSize)){
      plainttext_offset_ = 0;
      return false;
  }else{
      plainttext_offset_ = commodity_offset_ + info->header.size;
      return true;
    }
}

bool godin::WeimiWrite::writePlaintextSection(godin::WeiMiPlaintextInfo_header *info) {

  if(!is_init_ || info == nullptr)
    return false;

  if(plainttext_offset_ == 0){
    godin::Log::e(" write plaint section failed,please write commodity section first.");
    return false;
  }

  if(file_->lseekOfStart(plainttext_offset_) == -1){
      godin::Log::e(" writePlaintextSection err: lseek failed.");
      return false;
  }
  if(!file_->write((void*)info,info->header.headerSize)){
      cipher_offset_ = 0;
      return false;
  }else{
      cipher_offset_ = plainttext_offset_ + info->header.size;
      return true;
    }

}

bool godin::WeimiWrite::writeCipherSection(godin::WeiMiCipherInfo_header *info) {

  if(!is_init_ || info == nullptr)
    return false;

  if(cipher_offset_ == 0){
    godin::Log::e(" write cipher section failed,please write plaintext section first.");
    return false;
  }

  if(file_->lseekOfStart(cipher_offset_) == -1){
      godin::Log::e(" writeCipherSection err: lseek failed.");
      return false;
  }
  info->offset = cipher_offset_ + info->header.headerSize;
  if(!file_->write((void*)info,info->header.headerSize)){
      return false;
  }else{
      return true;
    }
}
