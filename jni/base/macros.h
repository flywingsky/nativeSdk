/**
 * Project : weimi-native-sdk
 * FileName: macros.h
 * date    : 2017-02-15
 * time    : 15:37:29
 * author  : genglei.cuan@godinsec.com
 */


#ifndef MACROS_H
#define MACROS_H


/**
 * 将命名空间利用宏定义隔离出来,
 * 方便书写
 */
#ifdef __cplusplus
#define NS_GODIN_BEGIN                  namespace godin {
#define NS_GODIN_END                    }
#define USING_NS_GODIN                  using namespace godin
#else
#define NS_GODIN_BEGIN
#define NS_GODIN_END
#define USING_NS_GODIN
#endif



/**
 * DISALLOW_COPY_AND_ASSIGN
 * 参考google的c++规范,可以提高性能.
 * 参考:http://cs2.swfc.edu.cn/~zyl/?p=405
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&); \
void operator=(const TypeName&)




#endif // MACROS_H
