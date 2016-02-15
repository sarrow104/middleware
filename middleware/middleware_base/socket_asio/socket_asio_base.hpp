///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_ASIO_BASE_H
#define SOCKET_ASIO_BASE_H

#include "middleware/middleware_base/socket_asio/socket_asio_tcp_io_service_pool.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_session_keep.hpp"


namespace middleware {

  class socket_asio_server
  {
    socket_asio_server(const socket_asio_server&);
  protected:
    middleware_base* m_spc;
    socket_asio_server()
    {
      //socket_asio_session_base::init();
    }
  public:
    virtual socket_asio_session_base* get_session() = 0;
    virtual void release_session(socket_asio_session_base* ap) = 0;
    virtual bool send(char* apdata, uint32_t aiwlen) = 0;
  };

} //namespace middleware
#endif //SOCKET_ASIO_BASE_H

  /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */