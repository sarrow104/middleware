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
public:
	gateway_socket_client_con(
		boost::function<bool (const char* ,uint32_t )> logic_recv_callback ,
		uint32_t aimaxsize,
		uint32_t aievery_once_max_size
		) :
		m_connect(boost::bind(&gateway_socket_base::recv, (gateway_socket_base*)this, _1)),
		gateway_socket_base(
			aimaxsize ,
			aievery_once_max_size ,
			logic_recv_callback ,
			boost::bind( &connect_key_socket::get_socket, &m_connect, _1, _2)
			)
	{
		

	}

	bool create_con(uint32_t aikey , std::string aiserverip, uint32_t aiserverport, boost::function<bool(const char*, uint32_t)> aisendfailure)
	{
		return m_connect.send_key( aikey, aiserverip.c_str(), aiserverport, aisendfailure);
	}

	virtual bool sendfailure(SOCKET aisocket, const char* ap, uint32_t aplen)
	{
		return m_connect.reconnect(aisocket);
	}


	bool close( uint32_t aikey )
	{
		return m_connect.closekey(aikey);
	}
};






#endif
