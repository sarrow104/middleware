///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef MIDDLEWARE_BASE_HPP
#define MIDDLEWARE_BASE_HPP

#include "middleware/middleware_base/shared_memory/mgt_shared_memory.hpp"
#include "middleware/middleware_base/loop_array/loop_array_middleware.hpp"
#include "middleware/middleware_base/socket_io/gateway_client.hpp"
#include "middleware/middleware_base/socket_io/gateway_server.hpp"
#include "middleware/tools/incidental/pack_head.hpp"

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
    E_ASIO_SERVER,
    E_ASIO_CLIENT,
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
		tools::mgt_serializecpp* m_mser;
  public:
    middleware_sm_server(const char* aismname, uint64_t  ai_client_byte_sum, uint64_t ai_server_byte_sum, uint32_t aieveryonemaxsize, boost::function<bool(const char*, uint32_t)> logic_fun) :
			m_mser(nullptr),
      m_sms(aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun, true)
    {}
		/** 配置文件构造函数 */
		middleware_sm_server(
			uint32_t aiconfigtype, 
			const char* aiconfigpath, 
			boost::function<bool(const char*, uint32_t)> logic_fun
			);

		/** 配置内容构造函数 */
		middleware_sm_server(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen,
			boost::function<bool(const char*, uint32_t)> logic_fun
			);

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
		tools::mgt_serializecpp* m_mser;
  public:
    middleware_sm_client(
			const char* aismname, 
			uint64_t  ai_client_byte_sum, 
			uint64_t ai_server_byte_sum, 
			uint32_t aieveryonemaxsize, 
			boost::function<bool(const char*, uint32_t)> logic_fun) :
			m_mser(nullptr),
      m_smc(aismname, ai_client_byte_sum, ai_server_byte_sum, aieveryonemaxsize, logic_fun, false)
    {
    }

		/** 配置文件构造函数 */
		middleware_sm_client(
			uint32_t aiconfigtype,
			const char* aiconfigpath,
			boost::function<bool(const char*, uint32_t)> logic_fun
			);

		/** 配置内容构造函数 */
		middleware_sm_client(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen,
			boost::function<bool(const char*, uint32_t)> logic_fun
			);

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
		tools::mgt_serializecpp* m_mser;
  public:
    middleware_la_server( const char* ainame,
      uint32_t apbuffersize,
      uint32_t aieverymaxsize,
      FUN_READ_CALLBACK aireadfun,
      bool apstartthread) :
			m_mser(nullptr),
			m_las(ainame, apbuffersize, aieverymaxsize, aireadfun, apstartthread, true)
			{}

		/** 配置文件构造函数 */
		middleware_la_server(
			uint32_t aiconfigtype,
			const char* aiconfigpath,
			FUN_READ_CALLBACK aireadfun
			);

		/** 配置内容构造函数 */
		middleware_la_server::middleware_la_server(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen,
			FUN_READ_CALLBACK aireadfun
			);

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
		tools::mgt_serializecpp* m_mser;
  public:
    middleware_la_client( const char* ainame,
      uint32_t apbuffersize,
      uint32_t aieverymaxsize,
      FUN_READ_CALLBACK aireadfun,
      bool apstartthread) :
			m_las(ainame, apbuffersize, aieverymaxsize, aireadfun, apstartthread, false)
    {}

		/** 配置文件构造函数 */
		middleware_la_client(
			uint32_t aiconfigtype,
			const char* aiconfigpath,
			FUN_READ_CALLBACK aireadfun
			);

		/** 配置内容构造函数 */
		middleware_la_client(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen,
			FUN_READ_CALLBACK aireadfun
			);

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
  bool create_connkey(
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
		tools::mgt_serializecpp* m_mser;
  public:
    middleware_soio_server( uint32_t aiport,
      boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback,
      uint32_t aimaxsize,
      uint32_t aievery_once_max_size,
      boost::function<bool(const char*, uint32_t)> aisendfailure
      ):
			m_mser(nullptr),
      m_asi(aiport, logic_recv_callback, aimaxsize, aievery_once_max_size, aisendfailure)
    {}

		middleware_soio_server(
			uint32_t aiconfigtype, 
			const char* aiconfigpath, 
			boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback, 
			boost::function<bool(const char*, uint32_t)> aisendfailure
			);

		/** 配置内容构造函数 */
		middleware_soio_server(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen,
			boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback,
			boost::function<bool(const char*, uint32_t)> aisendfailure
			);

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
		tools::mgt_serializecpp* m_mser;
  public:
    middleware_soio_client(  
      boost::function<bool(uint32_t ,const char*, uint32_t)> logic_recv_callback,
			uint32_t aimaxsize,
      uint32_t aievery_once_max_size
    ) :
		m_mser(nullptr),
    m_asi(logic_recv_callback, aimaxsize, aievery_once_max_size)
    {}

		middleware_soio_client(
			uint32_t aiconfigtype,
			const char* aiconfigpath,
			boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback
			);

		/** 配置内容构造函数 */
		middleware_soio_client(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen,
			boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback
			);

    virtual bool send(uint32_t aikey, const  char* apdata, uint32_t aiwlen)
    {
      return m_asi.send(aikey, apdata, aiwlen);
    }

    virtual bool close(uint32_t aikey)
    {
      return m_asi.close(aikey);
    }

    bool create_connkey(
      uint32_t aikey,
      std::string aiserverip,
      uint32_t aiserverport,
      boost::function<bool(const char*, uint32_t)> aisendfailure
      )
    {
      return m_asi.create_conkey(aikey, aiserverip, aiserverport, aisendfailure);
    }

		bool create_connkey(
			uint32_t aiconfigtype,
			const char* aiconfigpath, 
			boost::function<bool(const char*, uint32_t)> aisendfailure
			)
		{
			m_mser = new tools::mgt_serializecpp(aiconfigtype, aiconfigpath);
			bool ret = m_asi.create_conkey(
				m_mser->pop<uint32_t>("key"), 
				(m_mser->pop<std::string>("ip")).c_str(), 
				m_mser->pop<uint32_t>("port"), 
				aisendfailure);
			delete m_mser;
			m_mser = nullptr;
			return ret;
		}

		bool create_connkey(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen,
			boost::function<bool(const char*, uint32_t)> aisendfailure
			)
		{
			m_mser = new tools::mgt_serializecpp(aiconfigtype, apconfigtxt, apconfigtxtlen);
			bool ret = m_asi.create_conkey(
				m_mser->pop<uint32_t>("key"),
				(m_mser->pop<std::string>("ip")).c_str(),
				m_mser->pop<uint32_t>("port"),
				aisendfailure);
			delete m_mser;
			m_mser = nullptr;
			return ret;
		}

    bool create_connect(uint32_t aikey,
      std::string aiserverip,
      uint32_t aiserverport,
      boost::function<bool(const char*, uint32_t)> aisendfailure
      )
    {
      return m_asi.create_connect(aikey, aiserverip, aiserverport, aisendfailure);
    }

		bool create_connect(
			uint32_t aiconfigtype,
			const char* aiconfigpath,
			boost::function<bool(const char*, uint32_t)> aisendfailure
			)
		{
			m_mser = new tools::mgt_serializecpp(aiconfigtype, aiconfigpath);
			bool ret = m_asi.create_connect(
				m_mser->pop<uint32_t>("key"),
				(m_mser->pop<std::string>("ip")).c_str(),
				m_mser->pop<uint32_t>("port"),
				aisendfailure);
			delete m_mser;
			m_mser = nullptr;
			return ret;
		}

		bool create_connect(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen,
			boost::function<bool(const char*, uint32_t)> aisendfailure
			)
		{
			m_mser = new tools::mgt_serializecpp(aiconfigtype, apconfigtxt, apconfigtxtlen);
			bool ret = m_asi.create_connect(
				m_mser->pop<uint32_t>("key"),
				(m_mser->pop<std::string>("ip")).c_str(),
				m_mser->pop<uint32_t>("port"),
				aisendfailure);
			delete m_mser;
			m_mser = nullptr;
			return ret;
		}

    virtual uint8_t type()
    {
      return E_MW_TYPE::E_SOIO_CLIENT;
    }
  };


  /**
  *  socket asio 服务器
  */
  struct socket_asio_arg;

  class middleware_asio_server :
    public socket_middleware_base
  {
    std::vector<middleware_base*> m_send;
		static tools::mgt_serializecpp* m_mser;
		static middleware_asio_server* m_this;
		middleware_asio_server(const middleware_asio_server&);/**不实现*/
		middleware_asio_server& operator=(const middleware_asio_server&);/**不实现*/
		middleware_asio_server(socket_asio_arg& aiarg);
		middleware_asio_server(
			std::vector<boost::function<bool(const char*, uint32_t)> >& aicallbackarr
			);
  public:
		static middleware_asio_server& get_single(socket_asio_arg& aiarg)
		{
			if (m_this == nullptr)
			{
				m_this = new middleware_asio_server(aiarg);
			}
			return *m_this;
		}

		static middleware_asio_server& get_single(
			std::vector<boost::function<bool(const char*, uint32_t)> > aicallbackarr
			)
		{
			if (m_this == nullptr)
			{
				m_this = new middleware_asio_server(aicallbackarr);
			}
			return *m_this;
		}

		static void read_config(
			uint32_t aiconfigtype,
			const char* aiconfigpath
			)
		{
			m_mser = new  tools::mgt_serializecpp(aiconfigtype, aiconfigpath);
		}

		/** 配置内容构造函数 */
		static void read_config(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen
			)
		{
			m_mser = new  tools::mgt_serializecpp(aiconfigtype, apconfigtxt, apconfigtxtlen);
		}

		template <typename T>
		static void get(T& at, const char* apkey)
		{
			if (m_mser != nullptr)
			{
				m_mser->pop(at, apkey);
				return;
			}
			else
			{
				throw 0;
			}
		}

		template <typename T>
		static T&& get(const char* apkey)
		{
			if (m_mser != nullptr)
			{
				return m_mser->pop<T>(apkey);
			}
			else
			{
				throw 0;
			}
		}

    virtual bool send(uint32_t aikey, const char* apdata, uint32_t aiwlen);

    virtual bool close(uint32_t aikey);

    virtual uint8_t type();

  };


  /**
  *  socket asio 客户端
  */
  class middleware_asio_client :
    public socket_middleware_base
  {
    middleware_soio_client m_soio;
  public:
    middleware_asio_client(
      boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback,
      uint32_t aimaxsize,
      uint32_t aievery_once_max_size
      ) :
      m_soio(logic_recv_callback, aimaxsize, aievery_once_max_size)
    {}

		middleware_asio_client(
			uint32_t aiconfigtype,
			const char* aiconfigpath,
			boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback
			) :
			m_soio(aiconfigtype, aiconfigpath, logic_recv_callback)
		{}

		/** 配置内容构造函数 */
		middleware_asio_client(
			uint32_t aiconfigtype,
			const char* apconfigtxt,
			uint32_t apconfigtxtlen,
			boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback
			) :
			m_soio(aiconfigtype, apconfigtxt, apconfigtxtlen, logic_recv_callback)
		{}


    virtual bool send(uint32_t aikey, const  char* apdata, uint32_t aiwlen)
    {
      return m_soio.send(aikey, apdata, aiwlen);
    }

    virtual bool close(uint32_t aikey)
    {
      return m_soio.close(aikey);
    }

    bool create_connect(
      uint32_t aikey,
      std::string aiserverip,
      uint32_t aiserverport,
      boost::function<bool(const char*, uint32_t)> aisendfailure
      )
    {
      return m_soio.create_connect(aikey, aiserverip, aiserverport, aisendfailure);
    }

		bool create_connect(
			uint32_t aiconfigtype,
			const char* aiconfigpath,
			boost::function<bool(const char*, uint32_t)> aisendfailure
			)
		{
			return m_soio.create_connect(aiconfigtype, aiconfigpath, aisendfailure);
		}

    virtual uint8_t type()
    {
      return E_MW_TYPE::E_ASIO_CLIENT;
    }
  };

}


#endif /* MIDDLEWARE_BASE_HPP */


/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */