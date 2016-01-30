///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SHARED_MEMORY_MIDDLEARE_H
#define SHARED_MEMORY_MIDDLEARE_H

#include "mgt_shared_memory.h"

namespace middleware {

#ifndef MIDDLEWARE_BASE
#define MIDDLEWARE_BASE

  class middleware_base
  {
  public:
    virtual bool send(char* apdata, uint32_t aiwlen) = 0;
    virtual bool close() = 0;
  };

#endif //MIDDLEWARE_BASE


  /** 
   ** 共享内存服务器
   ** shared memory server 
   **/
  class middleware_sms :
    public middleware_base
  {
    communicate_module m_sms;
  public:
    middleware_sms(const char* aismname, uint64_t  ai_client_byte_sum, uint64_t ai_server_byte_sum, uint32_t aieveryonemaxsize, boost::function<bool(char*, uint32_t&)> logic_fun) :
      m_sms(aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun, true)
    {}

    virtual bool send(char* apdata, uint32_t aiwlen)
    {
      memcpy(m_sms.wget_start(), apdata, aiwlen);
      m_sms.wget_finish(aiwlen);
      return true;
    }
    virtual bool close()
    {
      /*未实现*/
      return true;
    }
  };


  /** 
   **  共享内存客户端
   **  shared memory client 
   **/
  class middleware_smc :
    public middleware_base
  {
    communicate_module m_smc;
  public:
    middleware_smc(const char* aismname, uint64_t  ai_client_byte_sum, uint64_t ai_server_byte_sum, uint32_t aieveryonemaxsize, boost::function<bool(char*, uint32_t&)> logic_fun) :
      m_smc(aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun, false)
    {
    }

    virtual bool send(char* apdata, uint32_t aiwlen)
    {
      memcpy(m_smc.wget_start(), apdata, aiwlen);
      m_smc.wget_finish(aiwlen);
      return true;
    }
    virtual bool close()
    {
      /*未实现*/
      return true;
    }
  };

} //namespace middleware{

#endif //SHARED_MEMORY_MIDDLEARE_H