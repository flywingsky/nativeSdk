/**
 * Project : weimi-native-sdk
 * FileName: file.h
 * date    : 2017-02-25
 * time    : 01:25:50
 * author  : genglei.cuan@godinsec.com
 */
#include "file.h"
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <utils/log.h>
#include <utils/file_utils.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>


godin::File::File(const std::string &name) {
  path_name.clear();
  file_name.clear();
  data = NULL;
  open_mode = godin::File::NotOpen;
  fd = -1;
  mmap_size = 0 ;
  char *temp_path = realpath(name.c_str(),NULL);

  if(temp_path == NULL){
   godin::Log::e(" file path :%s ,is invaild. please check it berfore.",name.c_str());
  }else{
   path_name = temp_path;
   free(temp_path);
   temp_path = NULL;
   file_name = basename((char *)path_name.c_str());
  }
}

godin::File::~File(){
  close();
}

bool godin::File::exists() const {
  if(path_name.empty())
    return false;
  return godin::FileUtils::isFileExists(path_name);
}

std::string godin::File::suffix() const {
  return godin::FileUtils::getSuffix(file_name);
}

size_t godin::File::fileSize() const {
  return godin::FileUtils::getFileSize(path_name);
}

std::string godin::File::fileDir() const {

  if(path_name.find_last_of("/") != std::string::npos){
      return path_name.substr(0,path_name.find_last_of("/")+1);
  }else{
      return "";
  }
}

bool godin::File::remove() {

  if(isOpen())
    close();

  return godin::FileUtils::removeFile(path_name);
}

bool godin::File::rename(const std::string &newName) {


  if(!exists())
    return false;

  std::string  current_file_dir_path = fileDir();

  if(current_file_dir_path.empty())
    return false;

  if(isOpen())
    close();

  std::string suffix = (this->suffix().empty() == false) ? (std::string(".")+this->suffix()) : "";
  std::string new_name = current_file_dir_path + newName + suffix;

  if(godin::FileUtils::isFileExists(new_name))
    return false;

  if(::rename(path_name.c_str(),new_name.c_str()) == 0){
      path_name = new_name;
      file_name = newName + suffix;
      return true;
  }else{
      return false;
    }
}

bool godin::File::resize(size_t newSize) {

  if(!exists())
    return false;

  if(isOpen())
    close();

  if(::truncate(path_name.c_str(),newSize) == 0)
    return true;
  else
    return false;

}

bool godin::File::isOpen() const {
  return fd > 0 ? true : false;
}

bool godin::File::isReadable() const {

  return fd >0 ? true : false;

}

bool godin::File::isWritable() const {
  if(fd>0 && open_mode != ReadOnly)
    return true;
  else {
      return false;
  }
}

bool godin::File::open(godin::File::OpenMode mode) {

  switch (mode) {
    case godin::File::ReadOnly:
      fd = ::open(path_name.c_str(),O_RDONLY);
      open_mode = ReadOnly;
      break;
    case godin::File::ReadWrite:
      fd = ::open(path_name.c_str(),O_RDWR);
      open_mode = ReadWrite;
      break;
    case godin::File::Append:
      fd = ::open(path_name.c_str(),O_RDWR | O_APPEND);
      open_mode = Append;
      break;
    case godin::File::Truncate:
      fd = ::open(path_name.c_str(),O_RDWR | O_TRUNC);
      open_mode = Truncate;
      break;
    default:
      fd = -1;
      open_mode = NotOpen;
      godin::Log::e("open file mode is not support. please check it before.");
      break;
    }
  if(fd > 0)
    return true;
  else
    return false;
}

bool godin::File::close() {
  bool ret = false;
  if(fd > 0){
      if(::close(fd) != 0)
        return false;
    }

   return munmap();
}

uint8_t *godin::File::mmap(size_t size, size_t offset, bool readOnly) {

  if(!isOpen()){
      if(!open(readOnly == true ? ReadOnly : ReadWrite))
        return NULL;
  }

  if(fileSize() < size + offset)
    return NULL;

  if(offset % ::sysconf(_SC_PAGESIZE) != 0)
    return NULL;

  ///已经mmap过了，再次映射前，需要先释放上次映射的mmap
  if(data !=NULL )
    return NULL;

  if(!readOnly){
     if(open_mode == ReadOnly)
       return NULL;
  }


  data = (uint8_t *) ::mmap( NULL, size, readOnly ? PROT_READ : PROT_READ|PROT_WRITE, MAP_SHARED, fd, offset);

  if(data != MAP_FAILED){
    mmap_size = size;
    return data;
  }


  return NULL;
}

bool godin::File::munmap() {

  if(data == NULL || ::munmap(data, mmap_size) == 0) {
      data = NULL;
      mmap_size = 0;
      return true;
  }
  return false;
}
