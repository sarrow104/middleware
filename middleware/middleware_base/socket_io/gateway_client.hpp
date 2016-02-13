///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef GATEWAY_CLIENT_H
#define GATEWAY_CLIENT_H

#include "middleware/middleware_base/socket_io/connect_key_socket.hpp"
#include "middleware/middleware_base/socket_io/gateway_socket_base.hpp"

#include <unordered_map>

using std::unordered_map;

namespace middleware {

  /**
   * 负责维护 socket server连接
   */
  class gateway_socket_client_con :
    public gateway_socket_base
  {
    connect_key_socket m_connect;
    uint32_t m_serverport;
    std::string m_serverip;

  public:
    gateway_socket_client_con(
      boost::function<bool(uint32_t ,const char*, uint32_t)> logic_recv_callback,
      uint32_t aimaxsize,
      uint32_t aievery_once_max_size
      ) :
      m_connect(boost::bind(&gateway_socket_base::recv, this, _1, _2)),
      gateway_socket_base(
        aimaxsize,
        aievery_once_max_size,
        logic_recv_callback,
        boost::bind(&connect_key_socket::get_socket, &m_connect, _1, _2)
        )
    {
    }

    /**
     * 根据指定key ip 端口 创建连接
     */
    bool create_conkey(uint32_t aikey, std::string aiserverip, uint32_t aiserverport, boost::function<bool(const char*, uint32_t)> aisendfailure)
    {
      return m_connect.send_key(aikey, aiserverip.c_str(), aiserverport, aisendfailure, true);
    }

	/**
     * 根据指定key ip 端口 创建连接
     */
    bool create_connect(uint32_t aikey, std::string aiserverip, uint32_t aiserverport, boost::function<bool(const char*, uint32_t)> aisendfailure)
    {
      return m_connect.send_key(aikey, aiserverip.c_str(), aiserverport, aisendfailure, false);
    }


    /**
     * 处理发送失败
     */
    virtual bool sendfailure(SOCKET aisocket, const char* ap, uint32_t aplen)
    {
      return m_connect.reconnect(aisocket);
    }

    /**
     *  通过key,关闭指定连接
     */
    bool close(uint32_t aikey)
    {
      return m_connect.closekey(aikey);
    }

  };

} //namespace middleware
#endif //GATEWAY_CLIENT_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
