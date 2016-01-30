///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef GATEWAY_SERVER_H
#define GATEWAY_SERVER_H

#include "accept_key_socket.h"
#include "gateway_socket_base.h"

namespace middleware {

  /*****************************
   **  负责维护 socket server连接 
   *****************************/
  class gateway_socket_server_con :
    public gateway_socket_base
  {
    accept_key_socket m_accept;
  public:
    gateway_socket_server_con(
      uint32_t aiport,
      boost::function<bool(const char*, uint32_t)> logic_recv_callback,
      uint32_t aimaxsize,
      uint32_t aievery_once_max_size,
      boost::function<bool(const char*, uint32_t)> aisendfailure
      ) :
      m_accept(aiport, boost::bind(&gateway_socket_base::recv, (gateway_socket_base*)this, _1), aisendfailure),
      gateway_socket_base(
        aimaxsize,
        aievery_once_max_size,
        logic_recv_callback,
        boost::bind(&accept_key_socket::get_socket, &m_accept, _1, _2)
        )
    {
    }

	/*
	 *  处理发送失败
	 */
    virtual bool sendfailure(SOCKET aisocket, const char* ap, uint32_t aplen)
	{
      return m_accept.close(aisocket, ap, aplen);
    }

	/*
	 *  关闭所有连接
	 */
    bool close()
    {
      return m_accept.closeallconnect();
    }

  };


} //namespace middleware 





#endif
