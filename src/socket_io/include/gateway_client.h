#ifndef GATEWAY_CLIENT_H
#define GATEWAY_CLIENT_H

#include "connect_key_socket.h"
#include "gateway_socket_base.h"
#include <unordered_map>
using std::unordered_map;

/* 负责维护 socket server连接 */
class gateway_socket_client_con :
	public gateway_socket_base
{
	connect_key_socket m_connect;
	uint32_t m_serverport ;
	std::string m_serverip ;

	SOCKET get_socket( uint32_t aikey )
	{
		return m_connect.get_socket( aikey );
	}
public:
	gateway_socket_client_con(
		boost::function<bool ( char* ,uint32_t )> logic_recv_callback ,
		uint32_t aimaxsize,
		uint32_t aievery_once_max_size
		) :
		gateway_socket_base(
			aimaxsize ,
			aievery_once_max_size ,
			logic_recv_callback ,
			boost::bind( &gateway_socket_client_con::get_socket, this, _1)
			),
		m_connect(boost::bind( &gateway_socket_base::recv, (gateway_socket_base*)this ,_1))
	{
		

	}

	bool create_con(uint32_t aikey , std::string aiserverip, uint32_t aiserverport  )
	{
		return m_connect.send_key( aikey, aiserverip.c_str(), aiserverport );
	}

	
};






#endif
