#ifndef GATEWAY_SERVER_H
#define GATEWAY_SERVER_H

#include "accept_key_socket.h"
#include "gateway_socket_base.h"


/* 负责维护 socket server连接 */
class gateway_socket_server_con :
	public gateway_socket_base
{

	accept_key_socket m_accept;
	SOCKET get_socket( uint32_t aikey )
	{
		return m_accept.get_socket( aikey );
	}
public:

	gateway_socket_server_con(
		uint32_t aiport,
		boost::function<bool ( char* ,uint32_t )> logic_recv_callback ,
		uint32_t aimaxsize,
		uint32_t aievery_once_max_size
		) :
		gateway_socket_base(
			aimaxsize ,
			aievery_once_max_size ,
			logic_recv_callback ,
			boost::bind( &gateway_socket_server_con::get_socket, this, _1)
			),
		m_accept( aiport, boost::bind( &gateway_socket_base::recv, (gateway_socket_base*)this, _1 ) )
	{
	}
	

};








#endif
