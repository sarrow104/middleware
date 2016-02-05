///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/cmiddleware_base.h"
#include "middleware/middleware_base/shared_memory/mgt_shared_memory.hpp"
#include "middleware/middleware_base/loop_array/loop_array_middleware.hpp"
#include "middleware/middleware_base/socket_io/gateway_client.hpp"
#include "middleware/middleware_base/socket_io/gateway_server.hpp"

extern "C" {

void* init_middleware_sm_server(
  const char* aismname,
  uint64_t  ai_client_byte_sum,
  uint64_t ai_server_byte_sum,
  uint32_t aieveryonemaxsize,
  callbackfun logic_fun)
{
  middleware::communicate_module* lpsms = new middleware::communicate_module(
    aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun, true
    );
  return lpsms;
}

void* init_middleware_sm_client(
  const char* aismname,
  uint64_t  ai_client_byte_sum,
  uint64_t ai_server_byte_sum,
  uint32_t aieveryonemaxsize,
  callbackfun logic_fun)
{
  return new middleware::communicate_module(
    aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun, true
    );
}


bool send_middleware_sm(void* ap, char* apdata, uint32_t aiwlen)
{
  middleware::communicate_module* lp = static_cast<middleware::communicate_module*>(ap);
  memcpy(lp->wget_start(), apdata, aiwlen);
  lp->wget_finish(aiwlen);
  return true;
}

bool close_middleware_sm(void* ap)
{
  return true;
}



void* init_middleware_la_server(
  const char* ainame,
  uint32_t apbuffersize,
  uint32_t aieverymaxsize,
  callbackfun aireadfun,
  bool apstartthread,
  bool apisclient)
{
  return new middleware::middleware_looparray(
    ainame, apbuffersize, aieverymaxsize, aireadfun, apstartthread, apisclient);
}


void* init_middleware_la_client(
  const char* ainame,
  uint32_t apbuffersize,
  uint32_t aieverymaxsize,
  callbackfun aireadfun,
  bool apstartthread,
  bool apisclient)
{
  return new middleware::middleware_looparray(
    ainame, apbuffersize, aieverymaxsize, aireadfun, apstartthread, apisclient);
}


bool send_middleware_la(void* ap, char* apdata, uint32_t aiwlen)
{
  return static_cast<middleware::middleware_looparray*>(ap)->send(apdata, aiwlen);
}

bool close_middleware_la(void* ap)
{
  return static_cast<middleware::middleware_looparray*>(ap)->close();
}




void* init_middleware_soio_server(
  uint32_t aiport,
  multi_recv_fun logic_recv_callback,
  uint32_t aimaxsize,
  uint32_t aievery_once_max_size,
  sendfailure_fun aisendfailure
  )
{
  return new middleware::gateway_socket_server_con(
    aiport, logic_recv_callback, aimaxsize, aievery_once_max_size, aisendfailure);
}

void* init_middleware_soio_client(
  multi_recv_fun logic_recv_callback,
  uint32_t aimaxsize,
  uint32_t aievery_once_max_size
  )
{
  return new middleware::gateway_socket_client_con(
    logic_recv_callback, aimaxsize, aievery_once_max_size);
}

bool send_middleware_soio_client(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen)
{
  return static_cast<middleware::gateway_socket_client_con*>(ap)->send(aikey, apdata, aiwlen);
}

bool send_middleware_soio_server(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen)
{
  return static_cast<middleware::gateway_socket_server_con*>(ap)->send(aikey, apdata, aiwlen);
}

bool close_middleware_soio_client(void* ap, uint32_t aikey)
{
  return static_cast<middleware::gateway_socket_client_con*>(ap)->close(aikey);
}

bool close_middleware_soio_server(void* ap, uint32_t aikey)
{
  return static_cast<middleware::gateway_socket_server_con*>(ap)->close(aikey);
}

}
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
