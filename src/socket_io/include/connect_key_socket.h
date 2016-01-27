#ifndef CONNECT_KEY_SOCKET_H
#define CONNECT_KEY_SOCKET_H
#include "lpthread.h"
#include "socket_head.h"
#include <function.hpp>
#include <bimap.hpp>

/* 负责维护 socket client连接 */
class connect_key_socket 
{
	struct key_ip_port
	{
		uint32_t m_key;
		std::string m_ip;
		uint32_t m_port;
		key_ip_port( uint32_t aikey,std::string aiip, uint32_t aiport ):
			m_key( aikey),
			m_ip( aiip),
			m_port( aiport)
		{}
		key_ip_port( uint32_t aikey):
			m_key( aikey)
		{}
		bool operator<(const key_ip_port& r)const 
		{
			return m_key < r.m_key;
		}
	};
	SOCKET m_socket;
	static boost::bimap<SOCKET,key_ip_port>  m_socket_key;
	boost::function<bool(SOCKET)> m_recvfun;
public:
	connect_key_socket( boost::function<bool(SOCKET)> airecv):
		m_recvfun(airecv)
	{
		initsocket();
	}

	void initsocket( )
	{
#ifdef WIN32
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			WSACleanup();
			return;
		}
#endif
		return ;
	}

	SOCKET create_con( uint32_t aikey ,const char* aiserverip, int32_t aiserverport   )
	{

		SOCKET lsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	
		//构建服务器地址信息  
		struct sockaddr_in laddr ;
		laddr.sin_family = AF_INET;  
		laddr.sin_port = htons( aiserverport ); 
		laddr.sin_addr.s_addr = inet_addr( aiserverip );  
		//连接服务器  
		while (connect( lsock , (struct sockaddr *)&laddr, sizeof( laddr )) == SOCKET_ERROR)  
		{  
			//ErrlogWrite(G_LOGMSG_SOCKET,ERROR_SOCKET_INIT_NUM,"connect()");
			closesocket( lsock );   
#ifdef WIN32
			WSACleanup();
#endif
			boost::this_thread::sleep(boost::posix_time::seconds(1) );
		}  

		return lsock;

	}

	bool send_key(  uint32_t aikey ,const char* aiserverip, int32_t aiserverport )
	{
		
		m_socket = create_con( aikey, aiserverip, aiserverport );

		char lsendkey[ 32 ] = { 0 };
		char lrecvkey[ 32 ] = { 0 };

		size_t lsendlen = strlen( lsendkey ) + 1;
			
		if( g_send( m_socket , (const char*)&aikey, sizeof( uint32_t )  ) > 0 )
		{
			size_t lrecvlen  =  g_recv( m_socket , lrecvkey , 32  );
			if( lrecvlen < sizeof( uint32_t ) )
			{
				//失败
				return false;
			}
			else
			{
				if( *( (uint32_t*)(lrecvkey) ) != aikey)
				{
					//失败
					return false;
				}
			}
		}

		key_ip_port lkey_ip_port( aikey , aiserverip, aiserverport );


		m_socket_key.insert( boost::bimap<SOCKET,key_ip_port>::value_type(  m_socket, lkey_ip_port ) );

		boost::thread( boost::bind( &connect_key_socket::recv, this, m_socket) );
		return true;
	}
	
	void recv( SOCKET aisocket )
	{
		m_recvfun( aisocket );
	}


	SOCKET get_socket( uint32_t aikey )
	{
		auto itor = m_socket_key.right.find( key_ip_port(aikey) );
		if( itor != m_socket_key.right.end() )
		{
			return itor->second;
		}
		else
		{
			return 0;
		}
	}
};






#endif