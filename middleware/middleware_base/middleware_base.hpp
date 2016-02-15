///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef MIDDLEWARE_BASE_HPP
#define MIDDLEWARE_BASE_HPP

#include "middleware/middleware_base/shared_memory/mgt_shared_memory.hpp"
#include "middleware/middleware_base/loop_array/loop_array_middleware.hpp"
#include "middleware/middleware_base/socket_io/gateway_client.hpp"
#include "middleware/middleware_base/socket_io/gateway_server.hpp"

#include <cstdint>

namespace middleware {
  
  enum E_MW_TYPE
  {
    E_SM_SERVER,
    E_SM_CLIENT,
    E_LA_SERVER,
    E_LA_CLIENT,

    E_MIDDLE_RANGE = 64,

    E_SOIO_SERVER,
    E_SOIO_CLIENT,
  };

  class middleware_type
  {
  public:
    virtual uint8_t type() = 0;

  };

  /**
  *  中间件
  *  用构造函数
  *  区分
  *  循环数组,
  *  共享内存,
  *  socket
  */
  class middleware_base:
    public middleware_type
  {
  public:
    virtual bool send( const char* apdata, uint32_t aiwlen) = 0;
    virtual bool close() = 0;
  };

 /**
  *  共享内存客户端
  */
  class middleware_sm_server :
    public middleware_base
  {
    communicate_module m_sms;
  public:
    middleware_sm_server(const char* aismname, uint64_t  ai_client_byte_sum, uint64_t ai_server_byte_sum, uint32_t aieveryonemaxsize, boost::function<bool(const char*, uint32_t)> logic_fun) :
      m_sms(aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun, true)
    {}

    virtual bool send(const char* apdata, uint32_t aiwlen)
    {
      memcpy(m_sms.wget_start(), apdata, aiwlen);
      m_sms.wget_finish(aiwlen);
      return true;
    }

    virtual bool close()
    {
      /** 未实现*/
      return true;
    }

  virtual uint8_t type()
  {
    return E_MW_TYPE::E_SM_SERVER;
  }
  };

  /**
   *  共享内存客户端
   *  shared memory client
   */
  class middleware_sm_client :
    public middleware_base
  {
    communicate_module m_smc;
  public:
    middleware_sm_client(const char* aismname, uint64_t  ai_client_byte_sum, uint64_t ai_server_byte_sum, uint32_t aieveryonemaxsize, boost::function<bool(const char*, uint32_t)> logic_fun) :
      m_smc(aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun, false)
    {
    }

    virtual bool send(const char* apdata, uint32_t aiwlen)
    {
      memcpy(m_smc.wget_start(), apdata, aiwlen);
      m_smc.wget_finish(aiwlen);
      return true;
    }

    virtual bool close()
    {
      /** 未实现 */
      return true;
    }

  virtual uint8_t type()
  {
    return E_MW_TYPE::E_SM_CLIENT;
  }
  };


  /**
   *  循环数组服务器
   */
  class middleware_la_server :
    public middleware_base
  {
    middleware_looparray m_las;
  public:
    middleware_la_server( const char* ainame,
      uint32_t apbuffersize,
      uint32_t aieverymaxsize,
      FUN_READ_CALLBACK aireadfun,
      bool apstartthread) :
    m_las(ainame, apbuffersize, aieverymaxsize, aireadfun, apstartthread, true)
    {}

    virtual bool send(const char* apdata, uint32_t aiwlen)
    {
       return m_las.send(apdata, aiwlen);
    }

    virtual bool close()
    {
      return m_las.close();
    }

  virtual uint8_t type()
  {
    return E_MW_TYPE::E_LA_SERVER;
  }

  };

  /**
   *  循环数组客户端
   */
  class middleware_la_client :
    public middleware_base
  {
    middleware_looparray m_las;
  public:
    middleware_la_client( const char* ainame,
      uint32_t apbuffersize,
      uint32_t aieverymaxsize,
      FUN_READ_CALLBACK aireadfun,
      bool apstartthread) :
    m_las(ainame, apbuffersize, aieverymaxsize, aireadfun, apstartthread, false)
    {}

    virtual bool send(const char* apdata, uint32_t aiwlen)
    {
      return m_las.send(apdata, aiwlen);
    }

    virtual bool close()
    {
      return m_las.close();
    }

  virtual uint8_t type()
  {
    return E_MW_TYPE::E_LA_CLIENT;
  }
  };



  /**
  *  多对多中间件
  *  服务器通过key与客户端联系  
  *  用构造函数
  *  区分
  *  socket io
  *  socket asio
  */
  class socket_middleware_base:
    public middleware_type
  {
  public:
    virtual bool send(uint32_t aikey, const char* apdata, uint32_t aiwlen) = 0;
    virtual bool close(uint32_t aikey) = 0;
  /** client 需要实现*/
  bool create_connect(
      uint32_t aikey, 
      std::string aiserverip, 
      uint32_t aiserverport, 
      boost::function<bool(const char*, uint32_t)> aisendfailure
      )
    {
      return false;
    }
  };


  /**
   *  socket io 服务器
   */
  class middleware_soio_server :
    public socket_middleware_base
  {
    gateway_socket_server_con m_asi;
  public:
    middleware_soio_server( uint32_t aiport,
      boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback,
      uint32_t aimaxsize,
      uint32_t aievery_once_max_size,
      boost::function<bool(const char*, uint32_t)> aisendfailure
      ):
      m_asi(aiport, logic_recv_callback, aimaxsize, aievery_once_max_size, aisendfailure)
    {}

    virtual bool send(uint32_t aikey, const char* apdata, uint32_t aiwlen)
    {
       return m_asi.send( aikey, apdata, aiwlen);
    }

    virtual bool close(uint32_t aikey)
    {
      return m_asi.close( aikey);
    }

  virtual uint8_t type()
  {
    return E_MW_TYPE::E_SOIO_SERVER;
  }
  };

  /**
   *  socket io 客户端
   */
  class middleware_soio_client :
    public socket_middleware_base
  {
    gateway_socket_client_con m_asi;
  public:
    middleware_soio_client(  
      boost::function<bool(uint32_t ,const char*, uint32_t)> logic_recv_callback,
    uint32_t aimaxsize,
      uint32_t aievery_once_max_size
    ) :
    m_asi(logic_recv_callback, aimaxsize, aievery_once_max_size)
    {}

    void create_connect()
    {}
    virtual bool send(uint32_t aikey, const  char* apdata, uint32_t aiwlen)
    {
      return m_asi.send(aikey, apdata, aiwlen);
    }

    virtual bool close(uint32_t aikey)
    {
      return m_asi.close(aikey);
    }

    bool create_connect(
      uint32_t aikey,
      std::string aiserverip,
      uint32_t aiserverport,
      boost::function<bool(const char*, uint32_t)> aisendfailure
      )
    {
      return m_asi.create_conkey(aikey, aiserverip, aiserverport, aisendfailure);
    }
  virtual uint8_t type()
  {
    return E_MW_TYPE::E_SOIO_CLIENT;
  }
  };

}


#endif /* MIDDLEWARE_BASE_HPP */


/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */