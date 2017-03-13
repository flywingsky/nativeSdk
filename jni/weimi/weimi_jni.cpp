/**
 * Project : weimi-native-sdk
 * FileName: weimi_jni.cpp
 * date    : 2017-03-10
 * time    : 15:24:16
 * author  : genglei.cuan@godinsec.com
 */

#include <jni.h>
#include <string>
#include <utils/log.h>
#include <assert.h>
#include <utils/jni_utils.h>
#include <utils/file_utils.h>
#include <utils/hash_utils.h>
#include <date/date.h>
#include <vector>
#include "weimi.h"
#include <encrypt/encrypt.h>
#include "weimi_jni.h"



#define FILE_NOT_EXISTS                  1
#define FILE_CAN_NOT_OPEN                2
#define FILE_IS_ILLEGAL                  3
#define COMMODITY_ID_ILLEGAL             4
#define COMMODITY_ID_CRC_ILLEGAL         5
#define CREATE_FILE_FAILED               6
#define FILE_SUFFIX_ILLEGAL              7
#define WEME_FILE_CREATE_FAILD           8
#define ENCRYPT_DATA_FAILED              9
#define DECRYPT_DATA_FAILED              10

static std::string appFilesPath = "";

static jstring native_getCommodityID(JNIEnv* env,jclass clazz,jstring WeiMiFilepath){

  std::string filePath = godin::JniUtils::jstringToStdString(WeiMiFilepath);

  /// 检查文件是否存在,不存在则抛异常 FILE_NOT_EXISTS
  if(!godin::FileUtils::isFileExists(filePath))
    return godin::JniUtils::stringToJString("");

  godin::WeiMi * weimi = new godin::WeiMi();

  /// 初始化解析模块,失败则抛出异常 FILE_CAN_NOT_OPEN
  if(!weimi->readInit(filePath)){
      delete weimi;
      return godin::JniUtils::stringToJString("");
  }


  /// 解析weimi文件,失败则抛出异常 FILE_IS_ILLEGAL
  if(!weimi->parseWeimiFile()){
      delete weimi;
      return godin::JniUtils::stringToJString("");
  }

  std::string id =  weimi->getCommodityNum();

  /// 秘钥商品 id 非法,抛出异常 COMMODITY_ID_ILLEGAL
  if(id.length() != COMMODITY_NUM_LEN){
      delete weimi;
      return godin::JniUtils::stringToJString("");
  }

  /// 计算 id 的 crc 值
  uint8_t id_crc[CRC_32_BYTES];

  godin::HashUtils::getBlockCrc32((uint8_t*)id.c_str(),COMMODITY_NUM_LEN,
                                  id_crc,CRC_32_BYTES);

  /// 获取 weimi 文件中记录的id 的 crc 值
  std::vector<uint8_t> crc = weimi->getCommodityNumCrc();

  /// 获取 crc 失败, 抛出异常 COMMODITY_ID_CRC_ILLEGAL
  if(crc.size() != CRC_32_BYTES){
      delete weimi;
      return godin::JniUtils::stringToJString("");
  }

  /// 比较 crc 失败,抛出异常 COMMODITY_ID_CRC_ILLEGAL
  for(int i=0;i<CRC_32_BYTES;i++){
      if(id_crc[i] != crc[i]){
          delete weimi;
          return godin::JniUtils::stringToJString("");
      }
  }

  delete weimi;

  return godin::JniUtils::stringToJString(id);
}

/// 其中一个参数尚未指定,即秘钥id:fek_id.现在默认为1234.
static bool native_encryptWeiMiFile(JNIEnv* env,jclass clazz, jstring jplaintext_file_path, jstring jgodin_id, jstring jcommodity_id,jstring out_file_path){

  std::string plaintextFile = godin::JniUtils::jstringToStdString(jplaintext_file_path);
  std::string outFile = godin::JniUtils::jstringToStdString(out_file_path);
  std::string commodityId = godin::JniUtils::jstringToStdString(jcommodity_id);

  /// 明文文件不存在,抛出异常 FILE_NOT_EXISTS
  if(!godin::FileUtils::isFileExists(plaintextFile))
    return false;

  /// 明文文件的后缀,如果其后缀长度超过预设值,则抛出 FILE_SUFFIX_ILLEGAL
  std::string fileSuffix = godin::FileUtils::getSuffix(plaintextFile);
  if(fileSuffix.length() > FILE_SUFFIX_LEN)
    return false;

  /// 秘钥商品 id 非法,抛出异常 COMMODITY_ID_ILLEGAL
  if(commodityId.length() != COMMODITY_NUM_LEN)
    return false;

  /// 输出文件已经存在,则删除.
  /// java 层调用前,确保文件检查,以及重命名工作
  if(godin::FileUtils::isFileExists(outFile))
    godin::FileUtils::removeFile(outFile);

  godin::WeiMi *weimi = new godin::WeiMi();

  /// 创建文件失败,抛出异常 CREATE_FILE_FAILED
  if(!godin::FileUtils::createFile(outFile) || !weimi->initWrite(outFile)){
      delete weimi;
      return false;
  }


  bool ret = weimi->writeFileInfo() &&
             weimi->writeWelcomeSection() &&
             weimi->writeCommoditySection(commodityId) &&
             weimi->writePlaintextSection(fileSuffix,1234,godin::FileUtils::getFileSize(plaintextFile)) &&
             weimi->writeCipherSection();

  /// 写入weme文件信息时失败,抛出 WEME_FILE_CREATE_FAILD
  if(!ret){
      godin::Log::e("create weme file failed.");
      delete weimi;
      /// 删除文件
      godin::FileUtils::removeFile(outFile);
      return false;
  }

  delete weimi;

  godin::Encrypt *encrypt = new godin::Encrypt();

  /// 加密明文失败,抛出 ENCRYPT_DATA_FAILED
  if(!encrypt->encryptData(plaintextFile,outFile)){
      godin::Log::e("encrypt data to the weme file failed.");
      delete encrypt;
      return false;
  }
  delete encrypt;
  return true;
}


static jstring native_decryptWeiMiFile(JNIEnv* env,jclass clazz,jstring WeiMiFilepath){

  std::string filePath = godin::JniUtils::jstringToStdString(WeiMiFilepath);

  /// 文件不存在,抛出 FILE_NOT_EXISTS
  if(!godin::FileUtils::isFileExists(filePath)){
      return godin::JniUtils::stringToJString("");
  }

  godin::WeiMi * weimi = new godin::WeiMi();

  /// 文件打开失败,抛出 FILE_CAN_NOT_OPEN
  if(!weimi->readInit(filePath)){
      return godin::JniUtils::stringToJString("");
  }

  /// 解析weimi文件,失败则抛出异常 FILE_IS_ILLEGAL
  if(!weimi->parseWeimiFile()){
      delete weimi;
      return godin::JniUtils::stringToJString("");
  }

  std::string weimiDir = appFilesPath + "/" + WEIMI_DIR;

  if(!godin::FileUtils::isDirExists(weimiDir))
    godin::FileUtils::createDir(weimiDir);

  std::string fileName = godin::File(filePath).fileName();

  std::string fileNameWithoutSuffix = fileName.substr(0,fileName.find_last_of("."));

  std::string outFileName = appFilesPath + "/" +
                          fileNameWithoutSuffix + "-" + godin::Date().toString() + "-" +
                          "." + godin::FileUtils::getSuffix(filePath);

  if(godin::FileUtils::isFileExists(outFileName))
    godin::FileUtils::removeFile(outFileName);

  godin::Encrypt *encrypt = new godin::Encrypt();

  bool ret = encrypt->decryptData(filePath,weimi->getCipherOffset(),outFileName,weimi->getPlaintextSize());

  /// 解密失败,抛出异常 DECRYPT_DATA_FAILED
  if(!ret){
    godin::Log::e("decrypt file : %s failed.",filePath.c_str());
    delete weimi;
    return NULL;
  }

  delete weimi;
  delete encrypt;
  return godin::JniUtils::stringToJString(outFileName);
}

static const JNINativeMethod nMethods[]={

  {"encryptWeiMiFile","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z",(void*)native_encryptWeiMiFile},

  {"decryptWeiMiFile","(Ljava/lang/String;)Ljava/lang/String;",(void*)native_decryptWeiMiFile},

  {"getCommodityID","(Ljava/lang/String;)Ljava/lang/String;",(void*)native_getCommodityID},

};


static jint registerNativeMethods(JNIEnv *env,const char * className,const JNINativeMethod nMethods[],int numMethods ){

    jclass clazz = env->FindClass(className);
    if(clazz == NULL){
        godin::Log::e("can not find class: %s",className);
        return JNI_ERR;
    }
    if(env->RegisterNatives(clazz,nMethods,numMethods)!=JNI_OK){
        return JNI_ERR;
    }
    return JNI_OK;
}



JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM*vm,void*reserved){

    JNIEnv *env = NULL;
    if(vm->GetEnv((void**)&env,JNI_VERSION_1_6)!=JNI_OK){
        godin::Log::e("get Env  error !!!");
        return JNI_ERR;
    }
    assert(env!=NULL);
    if(registerNativeMethods(env,CLASS_NAME,nMethods,sizeof(nMethods)/sizeof(nMethods[0]))!=JNI_OK){
        godin::Log::e("register native methods error!!!");
        return JNI_ERR;
    }
    /// 初始化 jni utils
    godin::JniUtils::setJavaVM(vm);
    appFilesPath = godin::JniUtils::getFilesDir();
    if(appFilesPath.empty()){
        godin::Log::e("failed to get the app files path.");
        return JNI_ERR;
    }
    godin::Log::i("app files path %s",appFilesPath.c_str());
    return JNI_VERSION_1_6;
}
