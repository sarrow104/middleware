///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)
#ifndef CMIDDLEWARE_BASE_H
#define CMIDDLEWARE_BASE_H
#include <stdint.h>

/**
 *  C 接口
 */

/** #define _DLL_SAMPLE */
#ifdef _MSC_VER
#   define _DLL_SAMPLE
#   ifdef _DLL_SAMPLE
#       define DLL_SAMPLE_API __declspec(dllexport)
#   else
#       define DLL_SAMPLE_API __declspec(dllimport)
#   endif //_DLL_SAMPLE
#else
#   define DLL_SAMPLE_API
#endif  //_MSC_VER

/**    共享内存相关   */
#ifndef __cplusplus
typedef uint32_t bool;
#define true 1
#define false 0
#endif //__cplusplus


typedef bool (*callbackfun)(const char*, uint32_t);

/**
* 协议类型
*/
#ifndef __cplusplus
enum
{
  SERIALIZE_TYPE_BINARY,                              // 二进制 
  SERIALIZE_TYPE_JSON,                                //  JSON 
  SERIALIZE_TYPE_XML,                                 //  XML 
};
#endif //__cplusplus

/** 初始化服务器 */
/**
*  共享内存相关
*/
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
  
  /**  初始化共享内存通信组件服务器 */
  void* init_middleware_sm_server(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    callbackfun logic_fun
    );
  void* init_middleware_sm_server2(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    callbackfun logic_fun
    );
  
  /**  初始化共享内存通信组件客户端 */
  void* init_middleware_sm_client(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    callbackfun logic_fun);
  void* init_middleware_sm_client2(
    int32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    callbackfun logic_fun);

  /** 发送数据 */
  bool send_middleware(void* ap, char* apdata, uint32_t aiwlen);

  /** 关闭 */
  bool close_middleware_sm(void* ap);
  
#ifdef __cplusplus
}//extern "C"
#endif //__cplusplus


/**
 *  循环数组相关
 */
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

  void* init_middleware_la_server(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    callbackfun logic_fun);
  void* init_middleware_la_server2(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    callbackfun logic_fun);

  void* init_middleware_la_client(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    callbackfun logic_fun);
  void* init_middleware_la_client2(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    callbackfun logic_fun);

#ifdef __cplusplus
}//extern "C"
#endif //__cplusplus


/**
 *  socket io相关
 */
typedef bool (*sendfailure_fun)(const char*, uint32_t);   /** 发送失败回调 */
typedef bool(*multi_recv_fun)(uint32_t, const char*, uint32_t);  /** 接收回调 */
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

  void* init_middleware_soio_server(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    multi_recv_fun logic_recv_callback,
    sendfailure_fun aisendfailure
    );
  void* init_middleware_soio_server2(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    multi_recv_fun logic_recv_callback,
    sendfailure_fun aisendfailure
    );

  void* init_middleware_soio_client(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    multi_recv_fun logic_recv_callback
    );
  void* init_middleware_soio_client2(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    multi_recv_fun logic_recv_callback
    );

  bool create_connkey_soio(
    void* ap,
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    sendfailure_fun aisendfailure
    );
  bool create_connkey_soio2(
    void* ap,
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    sendfailure_fun aisendfailure
    );

  bool create_connect_soio(
    void* ap,
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    sendfailure_fun aisendfailure
    );
  bool create_connect_soio2(
    void* ap,
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    sendfailure_fun aisendfailure
    );

  bool send_middleware_soio(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen);

  bool close_middleware_soio(void* ap, uint32_t aikey);

#ifdef __cplusplus
}//extern "C"
#endif //__cplusplus

 /**
 *  asio相关
 */
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
  /** 初始化第一步 */
  void init_middleware_asio_server_1part(
    uint32_t aiconfigtype,
    const char* aiconfigpath
    );
  void init_middleware_asio_server_1part2(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen
    );

  /** 初始化第二步,返回需要提供的回调数量 */
  uint32_t init_middleware_asio_server_2part(
    );

  /** 初始化第三步,提供回调函数组 */
  void* init_middleware_asio_server_3part(
    callbackfun* apcallackfunarr,
    uint32_t aicallbackfunsize
    );

#define init_middleware_asio_client   init_middleware_soio_client
#define init_middleware_asio_client2  init_middleware_soio_client2
#define create_connkey_asio           create_connkey_soio
/**
 *asio 不需要以下两个函数
 *#define create_connkey_asio2          create_connkey_soio2
 *#define create_connect_asio           create_connect_soio
 */
#define create_connect_asio2          create_connect_soio2 
#ifdef __cplusplus
}//extern "C"
#endif //__cplusplus



#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
  typedef void (*test_fun)();
  void test_thread( test_fun fun);
  void boost_sleep(uint32_t ainum);

#ifdef __cplusplus
}//extern "C"
#endif //__cplusplus

#endif //CMIDDLEWARE_BASE_H


 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
