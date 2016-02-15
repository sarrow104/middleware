///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/socket_asio/socket_asio_mgt.hpp"

namespace middleware {

socket_asio_server_mgt* socket_asio_server_mgt::m_this;
socket_asio_server_mgt::static_member_keep socket_asio_server_mgt::m_smsk;

} //namespace middleware