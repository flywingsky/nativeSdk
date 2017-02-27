/**
 * Project : weimi-native-sdk
 * FileName: file.h
 * date    : 2017-02-25
 * time    : 01:25:46
 * author  : genglei.cuan@godinsec.com
 */
#ifndef FILE_H
#define FILE_H
#include <base/macros.h>
#include <string>


NS_GODIN_BEGIN

class File {
  public:
    File();
    File(const std::string &name);

  public:
    enum OpenMode {
          NotOpen = 0x0000,
          ReadOnly = 0x0001,
          WriteOnly = 0x0002,
          ReadWrite = ReadOnly | WriteOnly,
          Append = 0x0004,
          Truncate = 0x0008,
          Text = 0x0010,
      };
    /**
     * @brief exists
     * 检查文件是否存在
     * @return
     */
    bool exists() const;

    /**
     * @brief suffix
     * 获取文件后缀
     * @return
     */
    std::string suffix() const;

    /**
     * @brief fileName
     * 获取文件名(不包括路径)
     * @return
     */
    std::string fileName() const;

    /**
     * @brief pathName
     * 获取文件的全路径名(path + fileName)
     * @return
     */
    std::string pathName() const;

    /**
     * @brief fileSize
     * 获取文件大小
     * @return
     */
    size_t fileSize() const;

    /**
     * @brief remove
     * 删除文件；
     * 删除文件前，会先关闭文件
     * @return
     */
    bool remove();

    /**
     * @brief rename
     * 文件重命名；
     * 如果newName文件已经存在，则返回false,即不重命名不会覆盖已有文件.
     * 重命名前，会先关闭文件
     * @param newName
     * @return
     */
    bool rename(const std::string newName);

    /**
     * @brief resize
     * 修改文件的大小
     * @param newSize
     * @return
     */
    bool resize(size_t newSize);

    bool isOpen() const;
    bool isReadable() const;
    bool isWritable() const;
    bool open(OpenMode mode);
    void close();


};


NS_GODIN_END

#endif // FILE_H
