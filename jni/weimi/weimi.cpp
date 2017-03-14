/**
 * Project : weimi-native-sdk
 * FileName: weimi.cpp
 * date    : 2017-03-07
 * time    : 20:15:03
 * author  : genglei.cuan@godinsec.com
 */

#include "weimi.h"
#include <utils/hash_utils.h>
#include <utils/log.h>


godin::WeiMi::~WeiMi() {
  if(write_)
    delete write_;
  if(read_)
    delete read_;
}

bool godin::WeiMi::initWrite(const std::string &path) {

  if(write_init_){
      delete write_;
      write_ = nullptr;
  }

  if(write_ == nullptr){
      write_ = new WeimiWrite();
  }
  if(write_->init(path)){
      write_init_ = true;
      return true;
  }else{
      write_init_ = false;
      return false;
  }
}

bool godin::WeiMi::writeFileInfo() {

  if(!write_init_)
    return false;

  struct godin::WeiMiInfo fileinfo;
  fileinfo.big_or_little = godin::LITTLE_END;
  fileinfo.info_len = sizeof(struct godin::WeiMiInfo);
  ::memcpy(fileinfo.magic_num,MAGIC_STR,strlen(MAGIC_STR));
  fileinfo.major_num = MAJOR_VERSION;
  fileinfo.second_num = SECOND_VERSION;

  return write_->writeFileInfo(&fileinfo);
}

bool godin::WeiMi::writeWelcomeSection() {

  struct godin::WeiMiWelcomeInfo_header info;
  ::memset(&info,0,sizeof(struct godin::WeiMiWelcomeInfo_header));
  info.encode_type = UTF_8;
  info.fill_lenth  = ((strlen(DES_CONTENT) % 4)==0) ? 0 : 4 - (strlen(DES_CONTENT) % 4);

  info.header.type = WEI_MI_FILE_WELCOME_SECTION;
  info.header.headerSize = sizeof(struct godin::WeiMiWelcomeInfo_header);
  info.header.size = info.header.headerSize + strlen(DES_CONTENT) + info.fill_lenth;

  return write_->writeWelcomeSection(&info,(char*)DES_CONTENT,strlen(DES_CONTENT));
}

bool godin::WeiMi::writeCommoditySection(const std::string &commodity_number) {

  struct godin::WeiMiCommodityInfo_header info;
  ::memset(&info,0,sizeof(struct godin::WeiMiCommodityInfo_header));
  if(commodity_number.length() != COMMODITY_NUM_LEN){
      godin::Log::e("commodity number length is invaild.");
      return false;
  }

  ::memcpy(info.serial_num,commodity_number.c_str(),COMMODITY_NUM_LEN);

  godin::HashUtils::getBlockCrc32((uint8_t*)commodity_number.c_str(),COMMODITY_NUM_LEN,
                                  info.serial_num_crc,CRC_32_BYTES);

  info.header.type = WEI_MI_FILE_COMMODITY_INFO_SECTION;
  info.header.headerSize = sizeof(struct godin::WeiMiCommodityInfo_header);
  info.header.size = info.header.headerSize;

  return write_->writeCommoditySection(&info);
}

bool godin::WeiMi::writePlaintextSection(const std::string &file_suffix, uint32_t fek_id, uint32_t plaintText_size) {

  if(file_suffix.length() > FILE_SUFFIX_LEN){
      godin::Log::e("file suffix length more than the default value.");
      return false;
  }
  struct godin::WeiMiPlaintextInfo_header info;
  ::memset(&info,0,sizeof(struct godin::WeiMiPlaintextInfo_header));
  ::memcpy(info.file_suffix,file_suffix.c_str(),file_suffix.length());
  info.fek_id = fek_id;
  info.plaintext_len = plaintText_size;

  info.header.type = WEI_MI_FILE_PLAINTEXT_SECTION;
  info.header.headerSize = sizeof(struct godin::WeiMiPlaintextInfo_header);
  info.header.size = info.header.headerSize;

  return write_->writePlaintextSection(&info);
}

bool godin::WeiMi::writeCipherSection() {

  struct godin::WeiMiCipherInfo_header info;
  ::memset(&info,0,sizeof(struct godin::WeiMiCipherInfo_header));
  info.header.type = WEI_MI_FILE_ENC_SECTION;
  info.header.headerSize = sizeof(struct godin::WeiMiCipherInfo_header);
  info.header.size = info.header.headerSize;
  /// info.offset 在下面的方法中被初始化.
  return write_->writeCipherSection(&info);

}

bool godin::WeiMi::readInit(const std::string &path) {

  if(read_init_){
      delete read_;
      read_ = nullptr;
  }

  if(read_ == nullptr)
    read_ = new WeimiRead();

  if(read_->init(path)){
      read_init_ = true;
      return true;
  }else{
      read_init_ = false;
      return false;
    }
}

bool godin::WeiMi::isWeimiFile() {
  if(read_init_)
    return read_->isWeimiFile();
  return false;
}

bool godin::WeiMi::parseWeimiFile(){
  if(read_init_ && read_->parseWeimiFile()){
      parse_ = true;
      return true;
  }else
    parse_ = false;
  return false;
}

int godin::WeiMi::getFileMajorVersionNum() {
  if(parse_ && read_->getFileInfo())
    return read_->getFileInfo()->major_num;
  return -1;
}

int godin::WeiMi::getFileSecondVerionNum() {
  if(parse_ && read_->getFileInfo())
    return read_->getFileInfo()->second_num;
  return -1;
}

std::string godin::WeiMi::getCommodityNum() {
  std::string serialNum;
  if(parse_ && read_->getCommodityHeader()){
     char  serial_num[COMMODITY_NUM_LEN+1]= {0};
     ::memcpy(serial_num,read_->getCommodityHeader()->serial_num,COMMODITY_NUM_LEN);
     serialNum = serial_num;
     return serialNum;
  }
  return "";
}

std::vector<uint8_t> godin::WeiMi::getCommodityNumCrc() {
  std::vector<uint8_t> crc;
  crc.resize(4);
  if(parse_ && read_->getCommodityHeader()){
      uint8_t * p = read_->getCommodityHeader()->serial_num_crc;
      crc[0] = p[0];
      crc[1] = p[1];
      crc[2] = p[2];
      crc[3] = p[3];
      return crc;
  }
  crc.resize(0);
  return crc;

}

std::string godin::WeiMi::getPlaintFileSuffix() {
  std::string suffix;
  if(parse_ && read_->getPlaintextHeader()){
      char  file_suffix[FILE_SUFFIX_LEN+1]= {0};
      ::memcpy(file_suffix,read_->getPlaintextHeader()->file_suffix,FILE_SUFFIX_LEN);
      suffix = file_suffix;
      return suffix;
  }
  return "";
}

uint32_t godin::WeiMi::getFekId() {
  if(parse_ && read_->getPlaintextHeader()){
      return read_->getPlaintextHeader()->fek_id;
  }
  return 0;
}

uint32_t godin::WeiMi::getCipherOffset() {
  if(parse_ && read_->getCipherHeader()){
      return read_->getCipherHeader()->offset;
  }
  return 0;
}

uint32_t godin::WeiMi::getPlaintextSize() {
  if(parse_ && read_->getPlaintextHeader()){
      return read_->getPlaintextHeader()->plaintext_len;
  }
  return 0;
}

