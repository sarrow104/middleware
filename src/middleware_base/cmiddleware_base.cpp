///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/cmiddleware_base.h"
#include "middleware/middleware_base/middleware_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

extern "C" {


void* init_middleware_sm_server(
  const char* aismname,
  uint64_t  ai_client_byte_sum,
  uint64_t ai_server_byte_sum,
  uint32_t aieveryonemaxsize,
  callbackfun logic_fun)
{
  return new middleware::middleware_sm_server(
    aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun
    );
}

void* init_middleware_sm_client(
  const char* aismname,
  uint64_t  ai_client_byte_sum,
  uint64_t ai_server_byte_sum,
  uint32_t aieveryonemaxsize,
  callbackfun logic_fun)
{
  return new middleware::middleware_sm_client(
    aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun
    );
}



bool send_middleware(void* ap, char* apdata, uint32_t aiwlen)
{
  return static_cast<middleware::middleware_base*>(ap)->send( apdata, aiwlen );
}

bool close_middleware(void* ap)
{
  return static_cast<middleware::middleware_base*>(ap)->close( );
}



void* init_middleware_la_server(
  const char* ainame,
  uint32_t apbuffersize,
  uint32_t aieverymaxsize,
  callbackfun aireadfun,
  bool apstartthread)
{
  return new middleware::middleware_la_server(
    ainame, apbuffersize, aieverymaxsize, aireadfun, apstartthread);
}


void* init_middleware_la_client(
  const char* ainame,
  uint32_t apbuffersize,
  uint32_t aieverymaxsize,
  callbackfun aireadfun,
  bool apstartthread)
{
  return new middleware::middleware_la_client(
    ainame, apbuffersize, aieverymaxsize, aireadfun, apstartthread);
}





void* init_middleware_soio_server(
  uint32_t aiport,
  multi_recv_fun logic_recv_callback,
  uint32_t aimaxsize,
  uint32_t aievery_once_max_size,
  sendfailure_fun aisendfailure
  )
{
  return new middleware::middleware_soio_server(
    aiport, logic_recv_callback, aimaxsize, aievery_once_max_size, aisendfailure);
}

void* init_middleware_soio_client(
  multi_recv_fun logic_recv_callback,
  uint32_t aimaxsize,
  uint32_t aievery_once_max_size
  )
{
  return new middleware::middleware_soio_client(
    logic_recv_callback, aimaxsize, aievery_once_max_size);
}


bool create_connect_client(void* ap,uint32_t aikey,const char* aiserverip,uint32_t aiserverport,sendfailure_fun aisendfailure)
{
  return static_cast<middleware::middleware_soio_client*>(ap)->create_connect(aikey, aiserverip, aiserverport, aisendfailure);
}


bool send_middleware_soio(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen)
{
  return static_cast<middleware::socket_middleware_base*>(ap)->send(aikey, apdata, aiwlen);
}

bool close_middleware_soio(void* ap, uint32_t aikey)
{
  return static_cast<middleware::socket_middleware_base*>(ap)->close(aikey);
}


void* init_middleware_asio_server(
	casio_arg* aparg
	)
{
	std::vector<boost::function<bool(const char*, uint32_t)> > lrecv;
	lrecv.resize(aparg->m_threadmaxsize);
	for (uint32_t i = 0; i < aparg->m_threadmaxsize;++i)
	{
		lrecv[i] = aparg->m_callbackfun[i];
	}

	middleware::socket_asio_arg larg(aparg->m_threadmaxsize, lrecv);

	larg.m_timeout								= aparg->m_timeout;                                         /** 连接无活动的存活时间 单位秒 */
	larg.m_port										= aparg->m_port;                                            /** 端口号 */
	larg.m_recvpack_maxsize				= aparg->m_recvpack_maxsize;                                /** 最大单个包大小 */
	larg.m_session_num						= aparg->m_session_num;                                     /** 也就是最大连接数 */
	larg.m_loopbuffermaxsize			= aparg->m_loopbuffermaxsize;                               /** 环形数组缓冲大小 */
	larg.m_everyoncemaxsize				= aparg->m_everyoncemaxsize;                                /** 单条数据最大大小 */
	larg.m_extern_loopbuffermaxsize	= aparg->m_extern_loopbuffermaxsize;                      /** 回传的外部中间件的环形数组缓冲大小 */
	larg.m_extern_everyoncemaxsize	= aparg->m_extern_everyoncemaxsize;                       /** 回传的外部中间件的单条数据最大大小 */
	larg.m_activ									= aparg->m_activ;                                           /** 是否自我保证数据活性 */
	larg.m_extern_activ						= aparg->m_extern_activ;
	larg.m_persecond_recvdatatbyte_num = aparg->m_persecond_recvdatatbyte_num;                /** 每秒钟接收的字节数 */
	larg.m_s2s										= aparg->m_s2s;                                             /** 服务器与服务器的连接断开是否通知上层(此处两个服务器分别是:1.自身,2.集群中的某个其他节点(在1中的体现就是不受time out限制)) */
	larg.m_s2c										= aparg->m_s2c;                                             /** 服务器与客户端的连接断开是否通知上层 */
	larg.m_heartbeat_num					= aparg->m_heartbeat_num;                                   /** 心跳协议号,收到后重置time out时间，然后丢弃 */

	return new middleware::middleware_asio_server(larg);
}



void test_thread( test_fun fun)
{
  boost::thread( boost::function<void()>(fun) );
}

void boost_sleep(uint32_t ainum)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(ainum));
}


}
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
