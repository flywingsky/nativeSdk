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



godin::File::File(const std::string &name) {
  path_name.clear();
  file_name.clear();
  data = NULL;
  open_mode = godin::File::NotOpen;
  read_only = false;
  fd = -1;

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


  std::string  current_file_dir_path = fileDir();

  if(current_file_dir_path.empty())
    return false;

  if(isOpen())
    close();

  std::string suffix = (this->suffix().empty() == false) ? (std::string(".")+this->suffix()) : "";
  std::string new_name = current_file_dir_path + newName + suffix;
  if(::rename(path_name.c_str(),new_name.c_str()) == 0){
      path_name = new_name;
      file_name = newName + suffix;
      return true;
  }else{
      return false;
    }
}

bool godin::File::resize(size_t newSize) {

  if(isOpen())
    close();


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
      ReadOnly = ReadWrite;
      break;
    case godin::File::Append:
      fd = ::open(path_name.c_str(),O_RDWR | O_APPEND);
      ReadOnly = Append;
      break;
    case godin::File::Truncate:
      fd = ::open(path_name.c_str(),O_RDWR | O_TRUNC);
      ReadOnly = Truncate;
      break;
    default:
      fd = -1;
      ReadOnly = NotOpen;
      godin::Log::e("open file mode is not support. please check it before.");
      break;
    }
  if(fd > 0)
    return true;
  else
    return false;
}

void godin::File::close() {
  if(fd > 0){
    ::close(fd);
  }
}
