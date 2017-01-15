///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

namespace middleware {

  middleware_asio_server& asio_server(middleware_asio_server* ap)
  {
    static middleware_asio_server* lap = ap;
    return *lap;
  }

}  //namespace middleware
   /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
