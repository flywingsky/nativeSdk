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
#include <stdio.h>

NS_GODIN_BEGIN

class File {
  public:
    File(const std::string &name);
    virtual ~File();

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
     * 文件存在的情况下，对文件重命名；
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
     * 在文件存在的情况下，修改文件的大小;
     * 如果文件已经打开，会先关闭；
     * 如果原来的文件大小比参数length大，则超过的部分会被删除；
     * 如果原来的文件大小比参数length小，则新增部分的内容被0填充(非字符0)
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
    bool close();

    /**
     * @brief fd
     * 返回fd.
     * 最好不要直接操作fd
     * @return
     */
    int fd() const {return fd;}

    /**
     * @brief openMode
     * 返回文件的打开模式
     * @return
     */
    OpenMode openMode() const { return open_mode;}

    /**
     * @brief mmap
     * 映射文件到内存；
     * 不能重复映射；
     * 在没有执行ummap操作时，再次执行mmap操作，返回false.
     *
     * @param size
     * 映射的大小，size + offset 必须小于文件本身的大小
     *
     * @param offset
     * offset 必须页对其(例如32位下，是4K对其)
     * @param readOnly
     * 是否只读映射；
     * 当为可写映射时，需要保证文件打开的模式必须是可写的,否则返回false.
     *
     * @return
     */
    uint8_t * mmap(size_t size,size_t offset,bool readOnly);

    /**
     * @brief munmap
     * 释放mmap
     * @return
     */
    bool munmap();

    uint8_t* getMmapAddr() const{ return data;}

    private:

      DISALLOW_COPY_AND_ASSIGN(File);

      int fd; ///< 文件句柄

      std::string path_name; ///<  路径 + 文件名

      std::string file_name; ///< 文件名

      uint8_t *data; ///< 文件数据指针

      size_t mmap_size; ///< 映射大小

      OpenMode open_mode; ///< 打开模式
    };


NS_GODIN_END

#endif // FILE_H
