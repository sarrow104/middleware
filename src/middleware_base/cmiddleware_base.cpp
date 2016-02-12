///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/cmiddleware_base.h"
#include "middleware/middleware_base/middleware_base.hpp"

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
