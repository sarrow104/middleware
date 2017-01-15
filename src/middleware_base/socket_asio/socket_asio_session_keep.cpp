///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/socket_asio/socket_asio_session_keep.hpp"

namespace middleware {

  std::vector<socket_asio_session_keep::broadcasting_list> socket_asio_session_keep::m_broadcasting_list;
  alonelist3<socket_asio_session_base>* socket_asio_session_keep::mp_alone;
  std::vector<tools::segmentation_pack<socket_asio_session_keep*>* > socket_asio_session_keep::m_segmentation;
  uint32_t socket_asio_session_keep::m_persecond_recvdatatbyte_num;


} //namespace middleware


/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */