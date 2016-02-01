///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef MGT_SOCKET_SHARED_MEMORY_H
#define MGT_SOCKET_SHARED_MEMORY_H

#include "shared_memory_client.h"
#include "shared_memory_server.h"

namespace middleware {

  /**
   *  双向通信共享内存
   */
  class communicate_module
  {
    gateway_shared_memory* m_client_gsm;
    gateway_shared_memory* m_server_gsm;
    uint64_t  m_client_byte_sum;
    uint64_t  m_server_byte_sum;
    shared_memory_base* m_client;
    shared_memory_base* m_server;
    std::string m_namestr;
    uint32_t m_everyone_maxsize;
    boost::function<bool(char*, uint32_t&)> m_recv_callbackfun;
    bool m_ok;

	/*
	 *  创建共享内存对象
	 */
    gateway_shared_memory* create_shared_memory_obj(const char* aismname, uint64_t  ai_client_byte_sum)
    {
      gateway_shared_memory* lret_gsm = new gateway_shared_memory();
      if (!lret_gsm->open_shared_memort_head(aismname))
      {
        lret_gsm->create_shared_memort_head(aismname, ai_client_byte_sum);
      }
      return lret_gsm;
    }

   /**
	  *  设置回调
	  */
    void set_callback()
    {
      if (m_recv_callbackfun != NULL)
      {
        get_client()->set_callback(m_recv_callbackfun);
      }
    }

	/**
	 *  获取服务器端
	 */
    shared_memory_server* get_server()
    {
      return (shared_memory_server*)m_server;
    }

	/**
	 *  获取客户端
	 */
    shared_memory_client* get_client()
    {
      return (shared_memory_client*)m_client;
    }

	/**
	 *  设置操纵共享内存的对象
	 */
    void set_operator_shared_memory()
    {
      if (m_ok)
      {
        m_client = new shared_memory_client(m_server_gsm->address(), m_client_byte_sum, m_everyone_maxsize, (m_namestr + "c").c_str());
        m_server = new shared_memory_server(m_client_gsm->address(), m_server_byte_sum, m_everyone_maxsize, (m_namestr + "s").c_str());
      }
      else
      {
        m_server = new shared_memory_server(m_server_gsm->address(), m_client_byte_sum, m_everyone_maxsize, (m_namestr + "c").c_str());
        m_client = new shared_memory_client(m_client_gsm->address(), m_server_byte_sum, m_everyone_maxsize, (m_namestr + "s").c_str());
      }
    }
  public:
    communicate_module(
      const char* aismname,
      uint64_t  ai_client_byte_sum,
      uint64_t ai_server_byte_sum,
      uint32_t aieveryonemaxsize,
      boost::function<bool(char*, uint32_t&)> aifun,
      bool aiok = true
      ) :
      m_client_byte_sum(ai_client_byte_sum),
      m_server_byte_sum(ai_server_byte_sum),
      m_everyone_maxsize(aieveryonemaxsize),
      m_recv_callbackfun(aifun),
      m_namestr(aismname),
      m_ok(aiok)
    {
      /** 定义共享内存对象 */
      m_client_gsm = create_shared_memory_obj((m_namestr + "_client").c_str(), m_client_byte_sum);
      m_server_gsm = create_shared_memory_obj((m_namestr + "_server").c_str(), m_server_byte_sum);

      set_operator_shared_memory();
      set_callback();
    }

    /** 读进程专用 */
    char* rget_strat(uint32_t& ailen)
    {
      return get_client()->rget_strat(ailen);
    }

    void rget_finish()
    {
      get_client()->rget_finish();
    }

    /** 写进程专用 */
    char* wget_start()
    {
      return get_server()->wget_start();
    }

    void wget_finish(uint32_t aiwlen)
    {
      get_server()->wget_finish(aiwlen);
    }

  };

} //namespace middleware

#endif
