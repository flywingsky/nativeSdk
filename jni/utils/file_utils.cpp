#include "file_utils.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <vector>
#include <errno.h>
#include <stdlib.h>
#include <algorithm>


bool godin::FileUtils::isFileExists(const std::string &path) {
  if(access(path.c_str(),F_OK) != -1)
    return true;
  return false;
}

bool godin::FileUtils::isDirExists(const std::string &path) {
  struct stat file_stat;
  if(stat(path.c_str(), &file_stat)==0){
      if(S_ISDIR(file_stat.st_mode)){
          return true;
      }
  }
  return false;
}

bool godin::FileUtils::isRegularFile(const std::string &path) {
  struct stat file_stat;
  if(stat(path.c_str(), &file_stat)==0){
      if(S_ISREG(file_stat.st_mode)){
          return true;
      }
  }
  return false;
}

bool godin::FileUtils::isLinkFile(const std::string &path) {
  struct stat file_stat;
  if(stat(path.c_str(), &file_stat)==0){
      if(S_ISLNK(file_stat.st_mode)){
          return true;
      }
  }
  return false;
}

size_t godin::FileUtils::getFileSize(const std::string &path) {
  struct stat file_stat;
  if(stat(path.c_str(), &file_stat)==0){
    return file_stat.st_size;
  }
  return 0;
}

bool godin::FileUtils::createFile(const std::string &path) {

  /// 可读可写打开
  int open_flags = O_RDWR | O_CREAT;

  if(isFileExists(path))
    return true;

  int fd = open(path.c_str(), open_flags,S_IRWXU | S_IRWXG | S_IRWXO);

  if(fd == -1)
    return false;
  else
    close(fd);

  return true;
}

bool godin::FileUtils::createFileBySize(const std::string &path, size_t len) {
  int open_flags = O_RDWR | O_CREAT;

  if(isFileExists(path) && getFileSize(path) != len){
      if(!removeFile(path))
        return false;
  }
  int fd = open(path.c_str(), open_flags,S_IRWXU | S_IRWXG | S_IRWXO);
   if(fd == -1)
     return false;
   else{
     /// 取巧办法创建一个指定大小的文件
     /// 文件末尾是字符"1"．
     lseek(fd, len - 1, SEEK_SET);
     write(fd, "1", 1);
     close(fd);
   }
   return true;
}

bool godin::FileUtils::removeFile(const std::string &path) {
  if (remove(path.c_str()) == 0) {
    return true;
  } else {
    return false;
    }
}

bool godin::FileUtils::createDir(const std::string &path) {

  if(isDirExists(path))
    return true;

  size_t start = 0;
  size_t found = path.find_first_of("/\\", start);
  std::string subpath;
  std::vector<std::string> dirs;

  if (found != std::string::npos)
  {
    while (true)
    {
      subpath = path.substr(start, found - start + 1);
      if (!subpath.empty())
        dirs.push_back(subpath);
      start = found+1;
      found = path.find_first_of("/\\", start);
      if (found == std::string::npos)
      {
        if (start < path.length())
        {
          dirs.push_back(path.substr(start));
        }
        break;
      }
    }
  }

  DIR *dir = NULL;

  subpath = "";
  for (int i = 0; i < dirs.size(); ++i) {
    subpath += dirs[i];
    dir = opendir(subpath.c_str());
    if (!dir)
    {
      int ret = mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
      if (ret != 0 && (errno != EEXIST))
      {
        return false;
      }
    }
  }
  return true;
}

bool godin::FileUtils::removeDir(const std::string &path) {

  if(!isDirExists(path))
    return true;

  std::string command = "rm -r ";

  // path 中可能包含空格,所有添加""
  command += "\"" + path + "\"";

  if (system(command.c_str()) >= 0)
    return true;
  else
    return false;
}

bool godin::FileUtils::removeAllFilesFromDir(const std::string &path) {

  if(!isDirExists(path))
    return false;

  DIR *dir;
  struct dirent *ent;
  bool success = false;
  if ((dir = opendir (path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      if (ent->d_type == DT_DIR) {
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) { // ignore "." & ".." under unix
          std::string dir_path = path + ent->d_name + "/";
          success = removeDir(dir_path);
          if(!success)
            return false;
        }
      }
      else if (ent->d_type == DT_REG) {
        std::string file_path = path + ent->d_name;
        success = removeFile(file_path);
        if(!success)
          return false;
      }
    }
    closedir(dir);
    return true;
  } else {
    return false;
  }
  return false;
}

bool godin::FileUtils::removeAllFilesFromDir(const std::string &path, const std::vector<std::string> &exclude_sub_dirs) {

  if(!isDirExists(path))
    return false;

  DIR *dir;
  struct dirent *ent;
  bool success = false;
  if ((dir = opendir (path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {

      /// 排除例外的
      std::string ent_name = ent->d_name;
      if (std::find(exclude_sub_dirs.begin(), exclude_sub_dirs.end(), ent_name) != exclude_sub_dirs.end()) {
        continue;
      }

      if (ent->d_type == DT_DIR) {
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) { // ignore "." & ".." under unix
          std::string dir_path = path + ent->d_name + "/";
          success = removeDir(dir_path);
          if(!success)
            return false;
        }
      }
      else if (ent->d_type == DT_REG) {
        std::string file_path = path + ent->d_name;
        success = removeFile(file_path);
        if(!success)
          return false;
      }
    }
    closedir(dir);
    return true;
  } else {
    return false;
  }
  return false;
}
