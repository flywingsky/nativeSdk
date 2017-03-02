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


godin::Base64::Base64() {
  CryptoPP::AlgorithmParameters params = CryptoPP::MakeParameters(CryptoPP::Name::InsertLineBreaks(), false);
  encoder_.IsolatedInitialize(params);
}

std::string godin::Base64::encode(const uint8_t *input, size_t size) {

  std::string encoded;
  std::lock_guard<std::mutex> lg(encoder_mutex_);
  encoder_.Detach(new CryptoPP::StringSink(encoded));
  try{
    CryptoPP::StringSource(input, size, true, new CryptoPP::Redirector(encoder_));
    encoder_.Detach();
  }catch(const CryptoPP::Exception& e){
    encoder_.Detach();
    godin::Log::e(" base64 err:  %s",e.GetWhat().c_str());
    return "";
  }
  return encoded;
}

std::string godin::Base64::encode(const std::string &input) {
  return encode((uint8_t*)input.c_str(),input.length());
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
  encoder_.Detach(new CryptoPP::FileSink(outFile.c_str()));
  try{
    CryptoPP::FileSource(inputFile.c_str(), true, new CryptoPP::Redirector(encoder_));
    encoder_.Detach();
  }catch(const CryptoPP::Exception& e){
    godin::Log::e("base64 file err:%s",e.GetWhat().c_str());
    encoder_.Detach();
    return false;
  }
  return true;
}

std::vector<uint8_t> godin::Base64::decodeToVectorUint8(const std::string &input) {
  return decodeVector<uint8_t>(input);
}

std::string godin::Base64::decodeToString(const std::string &input) {
  std::string decoded;
  std::lock_guard<std::mutex> lg(decoder_mutex_);
  decoder_.Detach(new CryptoPP::StringSink(decoded));
  try{
    CryptoPP::StringSource (input, true,
        new CryptoPP::Redirector(decoder_)
    );
    decoder_.Detach();
  }catch(const CryptoPP::Exception& e){
    decoder_.Detach();
    godin::Log::e(" base64 err:  %s",e.GetWhat().c_str());
    return "";
  }
  return decoded;
}

bool godin::Base64::decodeFile(const std::string &inputFile, const std::string &outFile) {

  if(!godin::FileUtils::isFileExists(inputFile)){
      godin::Log::e(" base64 err: file input  %s not exists.",inputFile.c_str());
      return false;
  }

  if(godin::FileUtils::isFileExists(outFile)){
      godin::Log::e(" base64 err: file output %s is already exists.",outFile.c_str());
      return false;
  }

  std::lock_guard<std::mutex> lg(decoder_mutex_);
  decoder_.Detach(new CryptoPP::FileSink(outFile.c_str()));
  try{
    CryptoPP::FileSource(inputFile.c_str(), true, new CryptoPP::Redirector(decoder_));
    decoder_.Detach();
  }catch(const CryptoPP::Exception& e){
    godin::Log::e("base64 file err:%s",e.GetWhat().c_str());
    decoder_.Detach();
    return false;
  }
  return true;
}
