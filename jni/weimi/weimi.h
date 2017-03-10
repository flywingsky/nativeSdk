/**
 * Project : weimi-native-sdk
 * FileName: weimi.h
 * date    : 2017-03-07
 * time    : 20:08:41
 * author  : genglei.cuan@godinsec.com
 */


#ifndef WEIMI_H
#define WEIMI_H

#include <base/macros.h>
#include "weimi_write.h"
#include "weimi_read.h"
#include <string>
#include <vector>

NS_GODIN_BEGIN

class WeiMi {
  public:
    WeiMi():write_(nullptr),read_(nullptr),write_init_(false),read_init_(false),parse_(false){}
    virtual ~WeiMi();

    /**
     * @brief initWrite
     * 初始化写模块;
     * 传入的文件不存在时,则创建;
     * 存在时,清空文件内容.
     *
     * 注意: 重复调用该方法,会先回收上次创建的写模块资源.
     * 重新打开并分配资源.
     * @param path
     * @return
     */
    bool initWrite(const std::string &path);

    /**
     * @brief writeFileInfo
     * 写入weimi文件的fileinfo信息
     * @return
     */
    bool writeFileInfo();

    /**
     * @brief writeWelcomeSection
     * 写入weimi文件的welcome信息.
     * 欢迎信息采用utf8编码.
     * 注意:此处并没进行编码转换,
     * 务必保证将存储欢迎信息的源文件,
     * 存储为utf-8格式的.
     * TODO: 采用 iconv 库,进行字符集转换.
     * @return
     */
    bool writeWelcomeSection();

    /**
     * @brief writeCommoditySection
     * 写入weimi文件的秘钥商品信息
     *
     * @param commodity_number
     * 商品编号,长度必须是 COMMODITY_NUM_LEN.
     * 该值目前为12.
     *
     * 长度不符合的,该方法直接返回false.不会写入任何信息到文件中.
     * @return
     */
    bool writeCommoditySection(const std::string &commodity_number);

    /**
     * @brief writePlaintextSection
     * 写入weimi文件的明文信息
     *
     * @param file_suffix
     * 原文件后缀,最长为 FILE_SUFFIX_LEN.
     * 该值目前为8.
     * 超过8时,返回false.不会写入任何信息到文件中.
     *
     * @param fek_id
     * 秘钥ID
     *
     * @param plaintText_size
     * 待加密的原文件的长度.
     * @return
     */
    bool writePlaintextSection(const std::string &file_suffix,uint32_t fek_id,uint32_t plaintText_size);

    /**
     * @brief writeCipherSection
     * 写入weimi文件中的密文信息.
     * 注意: 该区预示着其后面跟着的是密文数据,
     * 该数据由加密模块填充.密文数据结构对weimi文件透明.
     * @return
     */
    bool writeCipherSection();

    /**
     * @brief readInit
     * 初始化读模块;
     * 以只读方式打开传入的文件.
     *
     * 重复初始化,会先释放先前的资源.并重新打开文件.
     * @param path
     * @return
     */
    bool readInit(const std::string &path);

    /**
     * @brief isWeimiFile
     * 检查一个文件是否是weimi文件.
     * 注意:这里仅仅是检查一个文件的前八字节内容是否是"WeimeEnc"
     * @return
     */
    bool isWeimiFile();

    /**
     * @brief parseWeimiFile
     * 解析weimi文件
     *
     * @return
     * 不是weimi文件,或者解析过程中出错,返回false.
     * weimi文件版本不匹配,也返回false.
     */
    bool parseWeimiFile();

    /**
     * @brief getFileMajorVersionNum
     * 获取weimi文件的主版本号
     * @return
     * 失败返回 -1;
     */
    int getFileMajorVersionNum();

    /**
     * @brief getFileSecondVerionNum
     * 获取weimi文件的次版本号
     *
     * @return
     * 失败返回 -1;
     */
    int getFileSecondVerionNum();

    /**
     * @brief getCommodityNum
     * 返回秘钥商品编号
     * @return
     * 失败返回空字符串.
     */
    std::string getCommodityNum();

    /**
     * @brief getCommodityNumCrc
     * 获取商品编号的crc校验值
     * @return
     * 成功返回4个uint8_t数据;
     * 失败返回0个数据.
     */
    std::vector<uint8_t> getCommodityNumCrc();

    /**
     * @brief getPlaintFileSuffix
     * 返回明文文件名的后缀
     * @return
     */
    std::string getPlaintFileSuffix();

    /**
     * @brief getFekId
     * 获取秘钥ID
     * @return
     */
    uint32_t getFekId();

    /**
     * @brief getCipherOffset
     * 获取密文数据在weimi文件中相对起始位置的偏移量
     * @return
     */
    uint32_t getCipherOffset();

    uint32_t getPlaintextSize();


  private:

    DISALLOW_COPY_AND_ASSIGN(WeiMi);

    WeimiWrite *write_;
    WeimiRead *read_;

    bool write_init_;
    bool read_init_;
    bool parse_;


};

NS_GODIN_END

#endif // WEIMI_H
