///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef CLOSE_SOCKET_PLAYER_H
#define CLOSE_SOCKET_PLAYER_H

#include "middleware/middleware_base/socket_io/lpthread.hpp"

#include <cstdint>

namespace middleware {

	class socket_asio_session_base;

	class close_socket_session
	{
		struct data_pair
		{
			socket_asio_session_base* m_ptr;
			uint32_t closesocketcount;
			bool m_send_protocol;
		};
		boost::mutex m_lock;											/* 用于关闭socket的锁 */
		boost_sem m_sem;
		std::vector< data_pair > m_ls;
	public:
		close_socket_session() :
			m_sem(0)
		{
			m_ls.reserve(1024);
			boost::thread(boost::bind(&close_socket_session::close_run, this));
		}
		void close_run();

		void push_close(socket_asio_session_base* ap, uint32_t aiclosesocketcount, bool asend_protocol)
		{
			m_lock.lock();
			uint32_t lsize = m_ls.size();
			m_ls.resize(lsize + 1);
			m_ls[lsize].m_ptr = ap;
			m_ls[lsize].closesocketcount = aiclosesocketcount;
			m_ls[lsize].m_send_protocol = asend_protocol;
			m_sem.post();
			m_lock.unlock();
		}
	};

}  //namespace middleware 
#endif //CLOSE_SOCKET_PLAYER_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */