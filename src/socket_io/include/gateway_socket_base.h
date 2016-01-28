#ifndef GATEWAY_SOCKET_BASE_H
#define GATEWAY_SOCKET_BASE_H
#include "socket_head.h"
#include "lpthread.h"
#include "../../loop_array/include/looparray.h"
#include <function.hpp>
#include <algorithm>

#define MORE_THREAD_SEND  0    //是否支持多线程send



class gateway_socket_base
{
	module_communicate m_recvlooparray;
	module_communicate m_sendlooparray;
	function<bool(uint32_t, SOCKET&)> m_callbacksocket;
	uint32_t m_ieverymaxsize;
public:
	gateway_socket_base(
		/* loop array相关 */
		uint32_t apbuffersize, uint32_t aieverymaxsize,FUN_READ_CALLBACK aireadfun,/* bool apstartthread,*/
		/* send回调方法从子类获取socket句柄 */
		function<bool(uint32_t, SOCKET&)> aicallbacksocket
		):
		m_recvlooparray( apbuffersize, aieverymaxsize, aireadfun, false),
		m_sendlooparray( apbuffersize, aieverymaxsize, boost::bind( &gateway_socket_base::send_callback, this, _1, _2 ), false),
		m_ieverymaxsize( aieverymaxsize),
		m_callbacksocket( aicallbacksocket )
	{}

	bool send(uint32_t aikey, const char* ap, uint32_t aplen)
	{
		static uint32_t lmaxsize = m_ieverymaxsize - sizeof(uint32_t);
		if( aplen > lmaxsize)
		{
			return false;
		}

		/*  */
		static char* lbuf1 = new char[m_ieverymaxsize];
		static char* lbuf2 = new char[m_ieverymaxsize];

		std::swap( lbuf1, lbuf2 );

		memcpy( lbuf1, &aikey, aplen );
		memcpy( &(lbuf1[sizeof(uint32_t)]), ap, aplen );
		m_sendlooparray.send( lbuf1, aplen+ sizeof(uint32_t) );
		return true;

	}



	bool send_callback(const char* ap, uint32_t aplen)
	{
		/* 获取key */
		uint32_t lkey = *( (uint32_t*)(ap) );
		SOCKET lsocket;
		if( m_callbacksocket(lkey, lsocket) )
		{
			if (g_send(lsocket, ap + sizeof(uint32_t), aplen - sizeof(uint32_t)) <= 0)
			{
				/* 发送失败 */
				sendfailure(lsocket, ap, aplen);
			}
		}
		else
		{
			if (lsocket != 0)
			{
				sendfailure(lsocket, ap, aplen);
			}
		}
		return true;
	}


	bool recv( SOCKET sisocket)
	{
		char* lbuf1 = new char[m_ieverymaxsize];
		char* lbuf2 = new char[m_ieverymaxsize];
		uint32_t llen = 0;
		while(1)
		{
			std::swap( lbuf1, lbuf2 );
			llen =  g_recv( sisocket, lbuf1, m_ieverymaxsize );
			if( llen <= 0)
			{
				/*错误*/
				cout << endl;
			}

			m_recvlooparray.send( lbuf1, llen );

		}
		return true;
	}



	virtual bool sendfailure(SOCKET aisocket, const char* ap, uint32_t aplen) = 0;
};














#endif
