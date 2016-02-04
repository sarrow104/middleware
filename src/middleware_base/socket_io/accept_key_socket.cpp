///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/socket_io/accept_key_socket.hpp"

namespace middleware {

  boost::bimap<SOCKET, key_stat>  accept_key_socket::m_socket_key;

} //namespace middleware 

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
