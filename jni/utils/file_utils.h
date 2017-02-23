#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <base/macros.h>

#include <string>
#include <vector>

NS_GODIN_BEGIN

/**
 * @brief The FileUtils class
 *
 * 针对 Linux (含Android) 平台
 *
 */

class FileUtils {

  private:
    FileUtils(){}

  public:

    /****************   检查类操作    *******************/

    /**
     * @brief isExists
     * 检查文件是否存在
     * @param path
     * @return
     */
    static bool isFileExists(const std::string &path);

    /**
     * @brief isDirExists
     * 判断是否是文件夹
     * @param path
     * @return
     */
    static bool isDirExists(const std::string &path);

    /**
     * @brief isRegularFile
     * 判断传入的文件是否是一个普通文件,而非软连接等.
     * @param path
     */
    static bool isRegularFile(const std::string &path);

    /**
     * @brief isLinkFile
     * 判断传入的文件是否是一个非软连接.
     * @param path
     * @return
     */
    static bool isLinkFile(const std::string &path);

    /**
     * @brief getFileSize
     * 获取文件的大小
     * @param path
     * @return
     */
    static size_t getFileSize(const std::string &path);



    /****************   文件操作    *******************/


    /**
     * @brief createFile
     * 创建一个文件,
     * 不存在时,才创建
     * @param path
     * @param mode
     * @return
     */
    static bool createFile(const std::string &path);

    /**
     * @brief createFileBySize
     * 创建指定大小的文件,
     * 使用字符"1"填充文件最后一字节,
     * 不存在时,创建,
     * 存在但大小和指定的len不一致时,删除原文件,重新创建
     * @param path
     * @param mode
     * @param len
     * @return
     */
    static bool createFileBySize(const std::string &path,size_t len);

    /**
     * @brief removeFile
     * 删除文件
     * @param path
     * @return
     * true:删除成功
     * false:删除失败
     */
    static bool removeFile(const std::string &path);


    /**
     * @brief createDir
     * 创建文件夹,
     * 可以递归创建.
     * @param path
     * @param mode
     * @return
     */
    static bool createDir(const std::string& path);

    /**
     * @brief removeDir
     * 删除一个文件夹里所有的文件以及文件夹自身
     * @param path
     * @return
     */
    static bool removeDir(const std::string& path);

    /**
     * @brief removeAllFilesFromDir
     * 删除文件夹内的所有文件(连接文件除外),但不删除文件夹自身
     * @param path
     * @return
     */
    static bool removeAllFilesFromDir(const std::string& path);

    /**
     * @brief removeAllFilesFromDir
     * 删除文件夹内的所有文件(连接文件除外),但不删除文件夹自身.
     * 可以额外指定哪些文件不删除
     * @param path
     * @param exclude_sub_dirs
     * @return
     */
    static bool removeAllFilesFromDir(const std::string& path, const std::vector<std::string>& exclude_sub_dirs);


};

NS_GODIN_END

#endif // FILEUTILS_H
