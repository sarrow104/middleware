///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_ASIO_SESSION_BASE_H
#define SOCKET_ASIO_SESSION_BASE_H


#include "middleware/middleware_base/socket_asio/socket_asio_tcp_io_service_pool.hpp"
#include "middleware/middleware_base/socket_asio/socket_session_mapping.hpp"
#include "middleware/tools/segmentation_pack/segmentation_pack.hpp"

#include <array>

#if WIN32
#define snprintf sprintf_s
#endif

namespace middleware {

  class socket_asio_session_base
  {
  protected:
    class tools_help
    {
    public:

      std::vector< unordered_set<uint32_t> > m_server;  /* 服务器之间保持的连接 */

      session_mapping<socket_asio_session_base*>  m_sessionidmapp;
      std::vector<middleware_base*> m_middlewarearr;
      std::vector<middleware_base*> m_extern_middlewarearr;
      std::vector<close_socket_player* > m_closesocket_help;
      std::vector<check_crc* > m_check_crc;
      std::vector< protocol_server_head > m_php;
      uint32_t m_everyoncemaxsize;    /* 单条最大数据 */
      uint32_t m_timeout;
      bool m_s2s;             /* 服务器与服务器的连接断开是否通知上层 */
      bool m_s2c;             /* 服务器与客户端的连接断开是否通知上层 */

      tools_help()
      {}

      void init(uint32_t aisize)
      {
        m_middlewarearr.resize(aisize);
        m_extern_middlewarearr.resize(aisize);
        m_closesocket_help.resize(aisize);
        m_check_crc.resize(aisize);
        m_server.resize(aisize);
        m_php.resize(aisize);
      }
    };
  };



} //namespace middleware
#endif //SOCKET_ASIO_SESSION_BASE_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
