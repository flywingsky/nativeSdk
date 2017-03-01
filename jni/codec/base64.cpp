/**
 * Project : weimi-native-sdk
 * FileName: base64.cpp
 * date    : 2017-03-01
 * time    : 14:28:38
 * author  : genglei.cuan@godinsec.com
 */


#include "base64.h"
#include <utils/file_utils.h>
#include <utils/log.h>
#include <cryptopp/files.h>

std::string godin::Base64::encode(const uint8_t *input, size_t size) {

  std::string encoded;

  CryptoPP::StringSink *newAttachment = new CryptoPP::StringSink(encoded);

  std::lock_guard<std::mutex> lg(encoder_mutex_);

  encoder_.Attach(newAttachment);
  encoder_.Put(input, size);
  if(!encoder_.MessageEnd())
    encoded = "";

  encoder_.Detach(newAttachment);
  free(newAttachment);
  newAttachment = NULL;

  return encoded;
}

std::string godin::Base64::encode(const std::string &input) {
  return encode(input.c_str(),input.length());
}

bool godin::Base64::encodeFile(const std::string &inputFile, const std::string &outFile) {

  if(!godin::FileUtils::isFileExists(inputFile)){
      godin::Log::e(" base64 err: file input  %s not exists.",inputFile.c_str());
      return false;
  }

  if(godin::FileUtils::isFileExists(outFile)){
      godin::Log::e(" base64 err: file output %s is already exists.",outFile.c_str());
      return false;
  }

  std::lock_guard<std::mutex> lg(encoder_mutex_);
  CryptoPP::FileSink* out = new CryptoPP::FileSink(outFile.c_str());
  encoder_.Attach(out);
  try{
    CryptoPP::FileSource(inputFile.c_str(), true,
                    &encoder_);
    encoder_.Detach(out);
    free(out);
    out = NULL;
  }catch(const CryptoPP::Exception& e){
    godin::Log::e("base64 file err:%s",e.GetWhat().c_str());
    return false;
  }

  return true;
}
