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
    const char* aismname,
    uint64_t  ai_client_byte_sum,
    uint64_t ai_server_byte_sum,
    uint32_t aieveryonemaxsize,
    callbackfun logic_fun);
  
  /**  初始化共享内存通信组件客户端 */
  void* init_middleware_sm_client(
    const char* aismname,
    uint64_t  ai_client_byte_sum,
    uint64_t ai_server_byte_sum,
    uint32_t aieveryonemaxsize,
    callbackfun logic_fun);

  /** 发送数据 */
  bool send_middleware_sm(void* ap, char* apdata, uint32_t aiwlen);

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
    const char* ainame,
    uint32_t apbuffersize,
    uint32_t aieverymaxsize,
    callbackfun aireadfun,
    bool apstartthread,
    bool apisclient);

  void* init_middleware_la_client(
    const char* ainame,
    uint32_t apbuffersize,
    uint32_t aieverymaxsize,
    callbackfun aireadfun,
    bool apstartthread,
    bool apisclient);

  /** 发送数据 */
  bool send_middleware_la(void* ap, char* apdata, uint32_t aiwlen);

  /** 关闭 */
  bool close_middleware_la(void* ap);

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
    uint32_t aiport,
    multi_recv_fun logic_recv_callback,
    uint32_t aimaxsize,
    uint32_t aievery_once_max_size,
    sendfailure_fun aisendfailure
    );

  void* init_middleware_soio_client(
    multi_recv_fun logic_recv_callback,
    uint32_t aimaxsize,
    uint32_t aievery_once_max_size
    );

  bool create_connect_client(void* ap,uint32_t aikey,const char* aiserverip,uint32_t aiserverport,sendfailure_fun aisendfailure);

  bool send_middleware_soio_client(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen);
  bool send_middleware_soio_server(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen);

  bool close_middleware_soio_client(void* ap, uint32_t aikey);
  bool close_middleware_soio_server(void* ap, uint32_t aikey);

#ifdef __cplusplus
}//extern "C"
#endif //__cplusplus



#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
	typedef void (*test_fun)();
	void test_thread( test_fun fun);

#ifdef __cplusplus
}//extern "C"
#endif //__cplusplus

#endif //CMIDDLEWARE_BASE_H


 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */