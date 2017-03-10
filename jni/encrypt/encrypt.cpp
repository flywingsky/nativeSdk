/**
 * Project : weimi-native-sdk
 * FileName: encrypt.cpp
 * date    : 2017-03-08
 * time    : 11:26:AES_BLOCK_SIZE
 * author  : genglei.cuan@godinsec.com
 */

#include "encrypt.h"
#include "aes_engine.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utils/file_utils.h>
#include "encrypt_macros.h"


bool godin::Encrypt::encryptData(const std::string &inputfile, const std::string &output) {

  if(!godin::FileUtils::isFileExists(inputfile))
    return false;
  if(!godin::FileUtils::isFileExists(output))
    return false;
  if(godin::FileUtils::getFileSize(inputfile) == 0)
    return false;

  int fd_in = ::open(inputfile.c_str(),O_RDONLY);

  int fd_out = ::open(output.c_str(),O_RDWR | O_APPEND);

  if(fd_in < 0 || fd_out < 0)
    return false;

  AesEngine *aes = new AesEngine();
  aes->setDefaultKey();

  struct EncInfo info;

  uint8_t* buf = (uint8_t*)malloc(DATA_BLOCK_SIZE);

  uint8_t* tobuf = (uint8_t*)malloc(DATA_BLOCK_SIZE+AES_BLOCK_SIZE);

  size_t inputSize = godin::FileUtils::getFileSize(inputfile);
  int tmp = inputSize % DATA_BLOCK_SIZE;
  int r = ((tmp % AES_BLOCK_SIZE)==0) ? AES_BLOCK_SIZE : AES_BLOCK_SIZE - (tmp % AES_BLOCK_SIZE);

  info.block_num = inputSize / DATA_BLOCK_SIZE;
  info.block_size = DATA_BLOCK_SIZE;
  info.encryption_type = ENCRYPTION_ALGORITHM_AES;
  info.encryption_mode = AES_CBC;
  info.hash_type = 0;/// 暂时无用
  info.encryption_blcok_size = AES_BLOCK_SIZE;
  info.cipher_len = info.block_num * (DATA_BLOCK_SIZE + AES_BLOCK_SIZE)
                    +  tmp
                    + (((tmp % AES_BLOCK_SIZE)==0) ? AES_BLOCK_SIZE : AES_BLOCK_SIZE - (tmp % AES_BLOCK_SIZE));

  info.header.type = ENCRYPTION_INFO_TYPE;
  info.header.headerSize = sizeof(struct EncInfo);
  info.header.size = info.header.headerSize + info.cipher_len;

  if(::write(fd_out,&info,info.header.headerSize)==-1){
      goto err;
  }
  for(int i=0;i<info.block_num;i++){
    ::read(fd_in,buf,DATA_BLOCK_SIZE);
    if(!aes->encryBlock(buf,DATA_BLOCK_SIZE,tobuf,DATA_BLOCK_SIZE+AES_BLOCK_SIZE)){
        goto err;
    }
    if(::write(fd_out,tobuf,DATA_BLOCK_SIZE+AES_BLOCK_SIZE)==-1){
        goto err;
    }
  }
  ::read(fd_in,buf,tmp);
  if(!aes->encryBlock(buf,tmp,tobuf,DATA_BLOCK_SIZE+AES_BLOCK_SIZE)){
      goto err;
  }

  if(::write(fd_out,tobuf,tmp+r)==-1){
      goto err;
  }

  close(fd_in);
  close(fd_out);
  free(buf);
  free(tobuf);
  delete aes;
  return true;
err:
  close(fd_in);
  close(fd_out);
  free(buf);
  free(tobuf);
  delete aes;
  return false;
}

bool godin::Encrypt::decryptData(const std::string &inputfile,
                                 off_t offset,
                                 const std::string &output,
                                 size_t plaintText_len){

  if(!godin::FileUtils::isFileExists(inputfile))
    return false;

  if(godin::FileUtils::getFileSize(inputfile) == 0)
    return false;

  if(godin::FileUtils::isFileExists(output)){
      godin::FileUtils::removeFile(output);
  }

  int fd_in = ::open(inputfile.c_str(),O_RDONLY);

  int fd_out = ::open(output.c_str(),O_RDWR | O_CREAT,S_IRWXU | S_IRWXG | S_IRWXO);

  if(fd_in < 0 || fd_out < 0)
    return false;

  AesEngine *aes = new AesEngine();
  aes->setDefaultKey();

  uint8_t* buf = NULL;
  uint8_t* tobuf = NULL;
  int tmp = 0;
  int r_tmp = 0;
  ::lseek(fd_in,offset,SEEK_SET);

  struct godin::Encrypt::Chunk_header header;

  ::read(fd_in,(void*)&header,sizeof(struct godin::Encrypt::Chunk_header));
  if(header.type != ENCRYPTION_INFO_TYPE){
      goto err;
  }

  ::lseek(fd_in,offset,SEEK_SET);

  struct godin::Encrypt::EncInfo info;
  ::read(fd_in,(void*)&info,sizeof(struct godin::Encrypt::EncInfo));
  ::lseek(fd_in,offset+header.headerSize,SEEK_SET);

  buf = (uint8_t*)malloc(info.block_size+info.encryption_blcok_size);
  tobuf = (uint8_t*)malloc(info.block_size);

  for(int i=0;i<info.block_num;i++){
      ::read(fd_in,buf,info.block_size+info.encryption_blcok_size);
     if(!aes->decryptBlock(buf,info.block_size+info.encryption_blcok_size,
                        tobuf,info.block_size))
       goto err;
     if(::write(fd_out,tobuf,info.block_size)==-1){
          goto err;
     }
  }
  tmp = info.cipher_len - info.block_num * (info.block_size+info.encryption_blcok_size);
  r_tmp = plaintText_len - info.block_num * (info.block_size);
  if(tmp > info.block_size || r_tmp < 0){
    goto err;
  }
  ::read(fd_in,buf,tmp);
  if(!aes->decryptBlock(buf,tmp,tobuf,r_tmp))
    goto err;
  if(::write(fd_out,tobuf,r_tmp)==-1){
     goto err;
  }
  close(fd_in);
  close(fd_out);
  delete aes;
  free(buf);
  free(tobuf);
  return true;
err:
  close(fd_in);
  close(fd_out);
  free(buf);
  free(tobuf);
  delete aes;
  return false;
}


