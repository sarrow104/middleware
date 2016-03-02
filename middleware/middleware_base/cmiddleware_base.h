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
    const char* ainame,
    uint32_t apbuffersize,
    uint32_t aieverymaxsize,
    callbackfun aireadfun,
    bool apstartthread);

  void* init_middleware_la_client(
    const char* ainame,
    uint32_t apbuffersize,
    uint32_t aieverymaxsize,
    callbackfun aireadfun,
    bool apstartthread);

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

  bool send_middleware_soio(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen);

  bool close_middleware_soio(void* ap, uint32_t aikey);

#ifdef __cplusplus
}//extern "C"
#endif //__cplusplus


 /**
 *  asio相关
 */
typedef bool(*sendfailure_fun)(const char*, uint32_t);   /** 发送失败回调 */
typedef bool(*multi_recv_fun)(uint32_t, const char*, uint32_t);  /** 接收回调 */
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
	typedef bool(*asio_recv_fun)(uint32_t, const char*, uint32_t);  /** 接收回调 */
	typedef struct 
	{
		uint32_t m_threadmaxsize;                                   /** 线程数 */
		asio_recv_fun* m_callbackfun;																/** 回调 */
		uint32_t m_timeout;                                         /** 连接无活动的存活时间 单位秒 */
		uint16_t m_port;                                            /** 端口号 */
		uint32_t m_recvpack_maxsize;                                /** 最大单个包大小 */
		uint32_t m_session_num;                                     /** 也就是最大连接数 */
		uint32_t m_loopbuffermaxsize;                               /** 环形数组缓冲大小 */
		uint32_t m_everyoncemaxsize;                                /** 单条数据最大大小 */
		uint32_t m_extern_loopbuffermaxsize;                        /** 回传的外部中间件的环形数组缓冲大小 */
		uint32_t m_extern_everyoncemaxsize;                         /** 回传的外部中间件的单条数据最大大小 */
		bool m_activ;                                               /** 是否自我保证数据活性 */
		bool m_extern_activ;
		uint32_t m_persecond_recvdatatbyte_num;                     /** 每秒钟接收的字节数 */
		bool m_s2s;                                                 /** 服务器与服务器的连接断开是否通知上层(此处两个服务器分别是:1.自身,2.集群中的某个其他节点(在1中的体现就是不受time out限制)) */
		bool m_s2c;                                                 /** 服务器与客户端的连接断开是否通知上层 */
		uint32_t m_heartbeat_num;                                   /** 心跳协议号,收到后重置time out时间，然后丢弃 */
	}casio_arg;

	void* init_middleware_asio_server(
		casio_arg* aparg
		);

	void* init_middleware_asio_client(
		multi_recv_fun logic_recv_callback,
		uint32_t aimaxsize,
		uint32_t aievery_once_max_size
		);

	bool send_middleware_asio(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen);

	bool close_middleware_asio(void* ap, uint32_t aikey);

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