///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/middleware_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_mgt.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

namespace middleware {

  /** 配置文件构造函数 */
  middleware_sm_server::middleware_sm_server(
    uint32_t aiconfigtype, 
    const char* aiconfigpath, 
    boost::function<bool(const char*, uint32_t)> logic_fun
    ):
    m_mser(new tools::mgt_serializecpp(aiconfigtype, aiconfigpath)),
    m_sms(
      m_mser->pop<std::string>("smname").c_str(), 
      m_mser->pop<uint32_t>("client_byte_sum"),
      m_mser->pop<uint32_t>("server_byte_sum"),
      m_mser->pop<uint32_t>("everyonemaxsize"),
      logic_fun, true)
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_sm_server::middleware_sm_server(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    boost::function<bool(const char*, uint32_t)> logic_fun
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, apconfigtxt, apconfigtxtlen)),
    m_sms(
      m_mser->pop<std::string>("smname").c_str(),
      m_mser->pop<uint32_t>("client_byte_sum"),
      m_mser->pop<uint32_t>("server_byte_sum"),
      m_mser->pop<uint32_t>("everymaxsize"),
      logic_fun, true)
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_sm_client::middleware_sm_client(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    boost::function<bool(const char*, uint32_t)> logic_fun
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, aiconfigpath)),
    m_smc(
      m_mser->pop<std::string>("smname").c_str(),
      m_mser->pop<uint32_t>("client_byte_sum"),
      m_mser->pop<uint32_t>("server_byte_sum"),
      m_mser->pop<uint32_t>("everymaxsize"),
      logic_fun, false)
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_sm_client::middleware_sm_client(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    boost::function<bool(const char*, uint32_t)> logic_fun
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, apconfigtxt, apconfigtxtlen)),
    m_smc(
      m_mser->pop<std::string>("smname").c_str(),
      m_mser->pop<uint32_t>("client_byte_sum"),
      m_mser->pop<uint32_t>("server_byte_sum"),
      m_mser->pop<uint32_t>("everyonemaxsize"),
      logic_fun, false)
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_la_server::middleware_la_server(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    FUN_READ_CALLBACK aireadfun
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, aiconfigpath)),
    m_las(
      m_mser->pop<std::string>("laname").c_str(),
      m_mser->pop<uint32_t>("buffersize"),
      m_mser->pop<uint32_t>("everymaxsize"),
      aireadfun, 
      m_mser->pop<bool>("startthread"),
      true)
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_la_server::middleware_la_server(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    FUN_READ_CALLBACK aireadfun
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, apconfigtxt, apconfigtxtlen)),
    m_las(
      m_mser->pop<std::string>("laname").c_str(),
      m_mser->pop<uint32_t>("buffersize"),
      m_mser->pop<uint32_t>("everymaxsize"),
      aireadfun,
      m_mser->pop<bool>("startthread"),
      true)
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_la_client::middleware_la_client(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    FUN_READ_CALLBACK aireadfun
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, aiconfigpath)),
    m_las(
      m_mser->pop<std::string>("laname").c_str(),
      m_mser->pop<uint32_t>("buffersize"),
      m_mser->pop<uint32_t>("everymaxsize"),
      aireadfun,
      m_mser->pop<bool>("startthread"),
      false)
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_la_client::middleware_la_client(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    FUN_READ_CALLBACK aireadfun
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, apconfigtxt, apconfigtxtlen)),
    m_las(
      m_mser->pop<std::string>("laname").c_str(),
      m_mser->pop<uint32_t>("buffersize"),
      m_mser->pop<uint32_t>("everymaxsize"),
      aireadfun,
      m_mser->pop<bool>("startthread"),
      false)
  {
    delete m_mser;
    m_mser = nullptr;
  }


  middleware_soio_server::middleware_soio_server(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback,
    boost::function<bool(const char*, uint32_t)> aisendfailure
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, aiconfigpath)),
    m_asi(
      m_mser->pop<uint32_t>("port"),
      logic_recv_callback, 
      m_mser->pop<uint32_t>("buffersize"),
      m_mser->pop<uint32_t>("everymaxsize"),
      aisendfailure
      )
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_soio_server::middleware_soio_server(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback,
    boost::function<bool(const char*, uint32_t)> aisendfailure
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, apconfigtxt, apconfigtxtlen)),
    m_asi(
      m_mser->pop<uint32_t>("port"),
      logic_recv_callback,
      m_mser->pop<uint32_t>("buffersize"),
      m_mser->pop<uint32_t>("everymaxsize"),
      aisendfailure
      )
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_soio_client::middleware_soio_client(
    uint32_t aiconfigtype,
    const char* aiconfigpath,
    boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, aiconfigpath)),
    m_asi(logic_recv_callback, 
      m_mser->pop<uint32_t>("buffersize"),
      m_mser->pop<uint32_t>("everymaxsize")
      )
  {
    delete m_mser;
    m_mser = nullptr;
  }

  middleware_soio_client::middleware_soio_client(
    uint32_t aiconfigtype,
    const char* apconfigtxt,
    uint32_t apconfigtxtlen,
    boost::function<bool(uint32_t, const char*, uint32_t)> logic_recv_callback
    ) :
    m_mser(new tools::mgt_serializecpp(aiconfigtype, apconfigtxt, apconfigtxtlen)),
    m_asi(logic_recv_callback,
      m_mser->pop<uint32_t>("buffersize"),
      m_mser->pop<uint32_t>("everymaxsize")
      )
  {
    delete m_mser;
    m_mser = nullptr;
  }

  tools::mgt_serializecpp* middleware_asio_server::m_mser = nullptr;
  middleware_asio_server* middleware_asio_server::m_this = nullptr;

  middleware_asio_server::middleware_asio_server(socket_asio_arg& aiarg)
  {
    /** 初始化 */
    socket_asio_server_mgt::init(aiarg);
    m_send = aiarg.m_middlewarearr;
    socket_asio_server_mgt::start();
  }
  middleware_asio_server::middleware_asio_server(
    std::vector<boost::function<bool(const char*, uint32_t)> >& aicallbackarr
    )
  {
    if (m_mser == nullptr)
    {
      /** 错误 */
      return;
    }
    uint32_t lpthreadnum;
    m_mser->pop(lpthreadnum, "pthread_num");

    middleware::socket_asio_arg larg(lpthreadnum, aicallbackarr);

    //larg.m_activ = false;
    m_mser->pop(larg.m_activ, "activ");
    //larg.m_extern_activ = false;
    m_mser->pop(larg.m_extern_activ, "extern_activ");
    // larg.m_everyoncemaxsize = 1024;
    m_mser->pop(larg.m_everyoncemaxsize, "everyoncemaxsize");
    //larg.m_extern_everyoncemaxsize = 1024;
    m_mser->pop(larg.m_extern_everyoncemaxsize, "extern_everyoncemaxsize");
    //larg.m_extern_loopbuffermaxsize = 10240;
    m_mser->pop(larg.m_extern_loopbuffermaxsize, "extern_loopbuffermaxsize");
    //larg.m_loopbuffermaxsize = 10240;
    m_mser->pop(larg.m_extern_loopbuffermaxsize, "loopbuffermaxsize");
    //larg.m_heartbeat_num = 32;
    m_mser->pop(larg.m_heartbeat_num, "heartbeat_num");
    //larg.m_persecond_recvdatatbyte_num = 1024;
    m_mser->pop(larg.m_persecond_recvdatatbyte_num, "persecond_recvdatatbyte_num");
    //larg.m_port = 13140;
    m_mser->pop(larg.m_port, "port");
    //larg.m_recvpack_maxsize = 1024;
    m_mser->pop(larg.m_recvpack_maxsize, "recvpack_maxsize");
    //larg.m_timeout = 10240;
    m_mser->pop(larg.m_timeout, "timeout");
    //larg.m_s2c = true;
    m_mser->pop(larg.m_s2c, "s2c");
    //larg.m_s2s = true;
    m_mser->pop(larg.m_s2s, "s2s");
    //larg.m_session_num = 10240;
    m_mser->pop(larg.m_session_num, "session_num");

    delete m_mser;
    m_mser = nullptr;
    /** 初始化 */
    socket_asio_server_mgt::init(larg);
    m_send = larg.m_middlewarearr;
    socket_asio_server_mgt::start();
  }


  bool middleware_asio_server::send(uint32_t aikey, const char* apdata, uint32_t aiwlen)
  {
    return m_send[aikey]->send( apdata, aiwlen);
  }

  bool middleware_asio_server::close(uint32_t aikey)
  {
    return m_send[aikey]->close();
  }

  uint8_t middleware_asio_server::type()
  {
    return E_MW_TYPE::E_ASIO_SERVER;
  }



} //namespace middleware
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
