#ifndef ACCEPT_KEY_SOCKET_H
#define ACCEPT_KEY_SOCKET_H

#include "lpthread.h"
#include "socket_head.h"

#include <bimap.hpp>
#include <iostream>
#include <function.hpp>

class accept_key_socket
{

	static boost::bimap<SOCKET,uint32_t>  m_socket_key;
	SOCKET m_ssock;
	uint32_t m_port;
	boost::mutex m_lock;
#ifdef WIN32
	int m_len ;
#else
	socklen_t m_len;
#endif
	boost::function<bool(SOCKET)> m_recvfun;
public:

	accept_key_socket( uint32_t aiport,boost::function<bool(SOCKET)> aifun) :
		m_port( aiport ),
		m_recvfun( aifun )
	{
		boost::thread( boost::bind( &accept_key_socket::initsocket , this ) );
	}

	void initsocket()
	{
		struct sockaddr_in m_saddr;

#ifdef WIN32
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			WSACleanup();
		}
#endif

		m_saddr.sin_family = AF_INET;
		m_saddr.sin_port = htons( m_port );
		m_saddr.sin_addr.s_addr = htonl(INADDR_ANY);
		m_ssock =socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (m_ssock == INVALID_SOCKET)
		{
			std::cout << WSAGetLastError()  << std::endl;
			throw 0;
		}
		if ( ::bind(m_ssock, (struct sockaddr*)&m_saddr, sizeof(m_saddr)) == SOCKET_ERROR)
		{
			std::cout << WSAGetLastError()  << std::endl;
			throw 0;
		}
		m_len = sizeof( sockaddr_in );

		if (listen(m_ssock, 10) == SOCKET_ERROR)
		{
			std::cout << WSAGetLastError()  << std::endl;
			throw 0;
		}

		run_accept();
	}

	void run_accept()
	{
		SOCKET l_csock;
		struct sockaddr_in l_caddr;

		
		while( 1 )
		{
			l_csock = accept(m_ssock, (struct sockaddr *)&l_caddr, &m_len);
			if (l_csock == INVALID_SOCKET)
			{
				/*	犹豫该如何处理
					因为看了socket书上说有可能不是因为closesocket导致accept返回
					有这种可能不知道sentos上是怎么实现的
				*/
				//std::cout << WSAGetLastError() << std::endl;
				return ;
			}
			//	std::cout << "SOCKET : " << l_csock << std::endl;
			boost::thread( boost::bind( &accept_key_socket::recv_key, this , l_csock ) ) ;
		}
	}


	void close_accept_socket( )
	{
		closesocket( m_ssock );
	}


	void recv_key( SOCKET aisocket )
	{
		char lirecv[ 32 ];

		int liret;
		
		liret = g_recv( aisocket , lirecv , 32 );
		if( liret <= 0 )
		{
			closesocket( aisocket );
			return;
		}
		
		if(liret < sizeof(uint32_t))
		{
			closesocket( aisocket );
			return;
		}

		uint32_t lkey = *((uint32_t*)( lirecv ));

		auto itor = m_socket_key.left.find( lkey );
		if ( itor != m_socket_key.left.end( ) )
		{
			closesocket( aisocket );
			return;
		}

		m_socket_key.insert( boost::bimap<SOCKET,uint32_t>::value_type( aisocket, lkey  ) );


		/* 回复 */
		if( g_send( aisocket , lirecv , liret ) <= 0 )
		{
			closesocket( aisocket );
			return;
		}
		boost::thread( boost::bind( &accept_key_socket::recv, this, aisocket) );

	}

	void recv( SOCKET aisocket )
	{
		m_recvfun( aisocket );
	}
	

	SOCKET get_socket( uint32_t aikey )
	{
		auto itor = m_socket_key.right.find( aikey );
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
