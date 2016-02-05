///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

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

typedef bool(*callbackfun)(const char*, uint32_t);

/** 初始化服务器 */
#ifndef cpp
/**
*  共享内存相关
*/
extern "C"
{
  
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
  
}//extern "C"


/**
 *  循环数组相关
 */
extern "C"
{
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
}


/**
 *  socket io相关
 */
typedef bool (*sendfailure_fun)(const char*, uint32_t);   /** 发送失败回调 */
typedef bool(*multi_recv_fun)(uint32_t, const char*, uint32_t);  /** 接收回调 */
extern "C"
{
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

  bool send_middleware_soio_client(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen);
  bool send_middleware_soio_server(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen);

  bool close_middleware_soio_client(void* ap, uint32_t aikey);
  bool close_middleware_soio_server(void* ap, uint32_t aikey);
}




#endif //CMIDLEWARE_BASE_H


 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */