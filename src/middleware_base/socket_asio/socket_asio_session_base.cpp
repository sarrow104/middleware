///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/socket_asio/socket_asio_session_base.hpp"

namespace middleware {

  uint32_t socket_asio_session_base::m_tempadd;
  socket_asio_session_base::tools_help socket_asio_session_base::m_tools;
  const boost::asio::socket_base::linger socket_asio_session_base::mo_option(true, 0);
  uint32_t socket_asio_session_base::m_heartbeat_num;       /* 心跳协议号 */

} //namespace middleware 

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */