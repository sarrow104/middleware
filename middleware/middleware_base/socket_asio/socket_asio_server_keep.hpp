///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_ASIO_SERVER_KEEP_H
#define SOCKET_ASIO_SERVER_KEEP_H


#include "middleware/middleware_base/socket_asio/socket_asio_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

namespace middleware {

	class socket_asio_server_keep :
		public socket_asio_server
	{
		static alonelist3<socket_asio_session_base> m_alone;
	protected:
		virtual socket_asio_session_base* get_session()
		{
			return m_alone.get_instance();
		}

		virtual void release_session(socket_asio_session_base* ap)
		{
			return m_alone.release_instance(ap);
		}
	public:
		/* ai_recv_callbackfun 中应该有 aithreadsize 个回调 */
		socket_asio_server_keep(
			socket_asio_arg& aiarg,
			socket_asio_tcp_io_service_pool* ap
			)
		{
			static bool lfirst = true;
			if (lfirst)
			{
				lfirst = false;
				std::vector<socket_asio_session_base*>& lvec =
					socket_asio_session_base::get_sessionidmapp( /*TYPE_KEEP*/).get_sessionmapp(aiarg.m_session_num);
				uint32_t lthreadmaxsize = aiarg.get_thread_maxsize();
				uint32_t lgroupsize = aiarg.m_session_num / lthreadmaxsize;

				uint32_t linum;
				for (uint32_t i = 0; i < aiarg.m_session_num; ++i)
				{
					if (i % lgroupsize == 0)
					{
						linum = ap->get_next_io_service();
					}
					lvec[i] = new socket_asio_session_keep(aiarg, linum, ap->get_io_service(), &m_alone);
				}

				uint32_t ltemp_num = 0;
				while (ltemp_num != lthreadmaxsize)
				{
					m_alone.create(std::vector<socket_asio_session_base*>(
						lvec.begin() + (ltemp_num*lgroupsize),
						lvec.begin() + (ltemp_num*lgroupsize + lgroupsize)
						));
					++ltemp_num;
				}

			}

			m_spc = aiarg.m_middlewarearr[aiarg.get_thread_pos()];

		}

		virtual bool send(char* apdata, uint32_t aiwlen)
		{
			return m_spc->send(apdata, aiwlen);
		}
	};

} //namespace middleware 
#endif //SOCKET_ASIO_SERVER_KEEP_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
