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
    File(const std::string &name);

  public:
    enum OpenMode {
          NotOpen    = 0x0000, ///< 还没打开
          ReadOnly   = 0x0001, ///< 以只读方式打开
          ReadWrite  = 0x0002, ///< 以可读可写方式打开
          Append     = 0x0003, ///< 以可读可写追加方式
          Truncate   = 0x0004, ///< 以可读可写方式打开,并清空文件内容
          NoSupport  = 0x0005,
      };
    /**
     * @brief exists
     * 检查文件是否存在
     * @return
     * false: 不存在,或者path_name 为空
     * true: 存在
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
    std::string fileName() const { return file_name;}

    /**
     * @brief pathName
     * 获取文件的绝对路径名(path + fileName)
     * @return
     */
    std::string pathName() const { return path_name;}

    /**
     * @brief fileSize
     * 获取文件大小
     * @return
     */
    size_t fileSize() const;

    /**
     * @brief fileDir
     * 返回当前文件所在的文件夹绝对路径(结尾包括"/")
     * @return
     */
    std::string fileDir()  const;

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
     *
     * @param newName
     * 特别要注意的是,传入的是文件的新名字,不包括路径和后缀;
     * 成功改名后,文件会被关闭,内部path_name和file_name均会更新.
     * 另外该方法不能修改文件的后缀名.
     * @return
     */
    bool rename(const std::string &newName);

    /**
     * @brief resize
     * 修改文件的大小
     * @param newSize
     * @return
     */
    bool resize(size_t newSize);

    /**
     * @brief isOpen
     * 文件是否已经打开
     * @return
     */
    bool isOpen() const;

    /**
     * @brief isReadable
     * 文件已经打开,并且可读
     * @return
     */
    bool isReadable() const;

    /**
     * @brief isWritable
     * 文件已经打开,并且可写
     * @return
     */
    bool isWritable() const;

    /**
     * @brief open
     * 打开文件
     * @param mode
     * @return
     */
    bool open(OpenMode mode);

    /**
     * @brief close
     * 关闭文件,并且释放资源
     */
    void close();

    /**
     * @brief fd
     * 返回fd.
     * @return
     */
    int fd() const {return fd;}

    /**
     * @brief openMode
     * 返回文件的打开模式
     * @return
     */
    OpenMode openMode() const { return open_mode;}

    private:

      DISALLOW_COPY_AND_ASSIGN(File);
      int fd; ///< 文件句柄

      std::string path_name; ///<  路径 + 文件名

      std::string file_name; ///< 文件名

      char *data; ///< 文件数据指针

      bool read_only; ///< flag. 只读标志

      OpenMode open_mode;
    };


NS_GODIN_END

#endif // FILE_H
