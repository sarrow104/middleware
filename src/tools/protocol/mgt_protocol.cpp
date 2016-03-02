
#include "middleware/tools/protocol/mgt_protocol.hpp"

namespace middleware {
  namespace tools {
    
  /**
   *  创建服务器
   */

   middleware_asio_server* create_server_protocol_mgt(
    type_server_protocol_map& apromap,
    const char* aiconfigpath
    )
    {
      mgt_serializecpp lreadconfig;
      lreadconfig.read(SERIALIZE_TYPE_XML,aiconfigpath);
      uint32_t lpthreadnum;
      lreadconfig.pop(lpthreadnum, "pthread_num");
      std::vector<boost::function<bool(const char*, uint32_t)> > ltemp(lpthreadnum);
      for (uint32_t i = 0; i < lpthreadnum; ++i)
      {
        ltemp[i] = boost::bind(
          &mgt_server_protocol::run_task,
          new mgt_server_protocol(apromap, 5, 1024),
          i, 0, _1, _2);
      }

      middleware::socket_asio_arg larg(lpthreadnum, ltemp);

      //larg.m_activ = false;
      lreadconfig.pop(larg.m_activ, "activ");
      //larg.m_extern_activ = false;
      lreadconfig.pop(larg.m_extern_activ, "extern_activ");
     // larg.m_everyoncemaxsize = 1024;
      lreadconfig.pop(larg.m_everyoncemaxsize, "everyoncemaxsize");
      //larg.m_extern_everyoncemaxsize = 1024;
      lreadconfig.pop(larg.m_extern_everyoncemaxsize, "extern_everyoncemaxsize");
      //larg.m_extern_loopbuffermaxsize = 10240;
      lreadconfig.pop(larg.m_extern_loopbuffermaxsize, "extern_loopbuffermaxsize");
      //larg.m_loopbuffermaxsize = 10240;
      lreadconfig.pop(larg.m_extern_loopbuffermaxsize, "loopbuffermaxsize");
      //larg.m_heartbeat_num = 32;
      lreadconfig.pop(larg.m_heartbeat_num, "heartbeat_num");
      //larg.m_persecond_recvdatatbyte_num = 1024;
      lreadconfig.pop(larg.m_persecond_recvdatatbyte_num, "persecond_recvdatatbyte_num");
      //larg.m_port = 13140;
      lreadconfig.pop(larg.m_port, "port");
      //larg.m_recvpack_maxsize = 1024;
      lreadconfig.pop(larg.m_recvpack_maxsize, "recvpack_maxsize");
      //larg.m_timeout = 10240;
      lreadconfig.pop(larg.m_timeout, "timeout");
      //larg.m_s2c = true;
      lreadconfig.pop(larg.m_s2c, "s2c");
      //larg.m_s2s = true;
      lreadconfig.pop(larg.m_s2s, "s2s");
      //larg.m_session_num = 10240;
      lreadconfig.pop(larg.m_session_num, "session_num");
      return new middleware_asio_server(larg);
    }


    middleware_asio_client* create_client_protocol_mgt(
    type_client_protocol_map& apromap,
    const char* aiconfigpath
    )
    {

      mgt_serializecpp lreadconfig;
      lreadconfig.read(SERIALIZE_TYPE_XML, aiconfigpath);
      uint32_t lkey;
      lreadconfig.pop(lkey, "key");
      std::string lstrip;
      lreadconfig.pop(lstrip, "ip");
      uint32_t lport;
      lreadconfig.pop(lport, "port");
      uint32_t leveryoncemaxsize;
      lreadconfig.pop(leveryoncemaxsize, "everyoncemaxsize");
      uint32_t lloopbuffermaxsize;
      lreadconfig.pop(lloopbuffermaxsize, "loopbuffermaxsize");
      

      boost::function<bool(uint32_t,const char*, uint32_t)> lrecv =  boost::bind(
            &mgt_client_protocol::run_task,
           new mgt_client_protocol(apromap,1, 1024),
           0, _1, _2, _3);
      
      
      return (new middleware::middleware_asio_client(lrecv, lloopbuffermaxsize, leveryoncemaxsize));
    }

    void connect_server(
      middleware_asio_client* ap, 
      boost::function<bool(const char*, uint32_t)> aisendfailure,
      const char* aiconfigpath)
    {
      mgt_serializecpp lreadconfig;
      lreadconfig.read(SERIALIZE_TYPE_XML, aiconfigpath);
      uint32_t lkey;
      lreadconfig.pop(lkey, "key");
      std::string lstrip;
      lreadconfig.pop(lstrip, "ip");
      uint32_t lport;
      lreadconfig.pop(lport, "port");
      uint32_t leveryoncemaxsize;
      lreadconfig.pop(leveryoncemaxsize, "everyoncemaxsize");
      uint32_t lloopbuffermaxsize;
      lreadconfig.pop(lloopbuffermaxsize, "loopbuffermaxsize");
      ap->create_connect(lkey, lstrip.c_str(), lport, aisendfailure);
    }

  } //namespace tools
} //namespace middleware

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */