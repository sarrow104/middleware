
#include "middleware/tools/protocol/mgt_protocol.hpp"

namespace middleware {
  namespace tools {

    /**
    *  创建服务器
    */

    middleware_asio_server* create_server_protocol_mgt(
      uint32_t aiconfigtype,
      const char* aiconfigpath,
      type_server_protocol_map& apromap
      )
    {

      middleware_asio_server::read_config( SERIALIZE_TYPE_XML, aiconfigpath);
      uint32_t lpthread_num;
      middleware_asio_server::get(lpthread_num,"pthread_num");
      std::vector<boost::function<bool(const char*, uint32_t)> > ltemp(lpthread_num);
      for (uint32_t i = 0; i < lpthread_num; ++i)
      {
        ltemp[i] = boost::bind(
          &mgt_server_protocol::run_task,
          new mgt_server_protocol(apromap, 5, 1024),
          i, 0, _1, _2);
      }

      return &middleware_asio_server::get_single(ltemp);

    }


    middleware_asio_client* create_client_protocol_mgt(
      uint32_t aiconfigtype,
      const char* aiconfigpath,
      type_client_protocol_map& apromap
      )
    {
      boost::function<bool(uint32_t,const char*, uint32_t)> lrecv =  boost::bind(
        &mgt_client_protocol::run_task,
        new mgt_client_protocol(apromap,1, 1024),
        0, _1, _2, _3);

      return (new middleware::middleware_asio_client(aiconfigtype, aiconfigpath, lrecv));
    }

    void connect_server(
      middleware_asio_client* ap,
      uint32_t aiconfigtype,
      const char* aiconfigpath, 
      boost::function<bool(const char*, uint32_t)> aisendfailure
      )
    {
      ap->create_connect(aiconfigtype, aiconfigpath,aisendfailure);
    }

  } //namespace tools
} //namespace middleware

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */