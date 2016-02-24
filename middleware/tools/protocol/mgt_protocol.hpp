///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef MGT_PROTOCOL_HPP
#define MGT_PROTOCOL_HPP

#include "middleware/tools/protocol/protocol_base.hpp"


namespace middleware {
	namespace tools {

		template <typename T_PHP1, typename T_PHP2>
		class mgt_protocol
		{
			std::vector< std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>*>* > m_promap_arr;
			std::vector< unpack_head_process<T_PHP1>* > m_premote2local_arr;
			std::vector< pack_head_process<T_PHP2>* > m_plocal2remote_arr;
			std::vector< middleware_base*> m_middle_arr;
		public:
			mgt_protocol(uint32_t aimaxthreadnum, uint32_t aieverybytes)//, std::vector<>)
			{
				m_promap_arr.resize(aimaxthreadnum);
				m_premote2local_arr.resize(aimaxthreadnum);
				m_plocal2remote_arr.resize(aimaxthreadnum);
				for (uint32_t i = 0; i < aimaxthreadnum; ++i)
				{
					m_premote2local_arr[i] = new unpack_head_process<T_PHP1>();
				}

				for (uint32_t i = 0; i < aimaxthreadnum; ++i)
				{
					m_plocal2remote_arr[i] = new pack_head_process<T_PHP2>(aieverybytes);
				}

				for (uint32_t i = 0; i < aimaxthreadnum; ++i)
				{

					m_promap_arr[i] =
						protocol_base<T_PHP1, T_PHP2>::new_protocol_base_map(
							aimaxthreadnum,
							m_premote2local_arr[i],
							m_plocal2remote_arr[i]
							);
				}
			}

			bool run_task(uint32_t aipos, uint32_t aikey, const char* ap, uint32_t aplen)
			{
				m_premote2local_arr[aipos]->reset(ap, aplen);
				T_PHP1* lphp = m_premote2local_arr[aipos]->get_head();
				uint32_t lprotocolnum = lphp->get_protocol_num();
				auto ltempfind = m_promap_arr[aipos]->find(lprotocolnum);

				if (ltempfind->first)
				{
					/**  write log */
					return true;
				}
				else
				{
					T_PHP2* lphp2 = m_plocal2remote_arr[aipos]->get_head();
					m_plocal2remote_arr[aipos]->set_pack_head(lphp);
					lphp2->get_error() = ltempfind->second->run_task(aikey);
					m_middle_arr[aipos]->send(lphp2->get_buffer(), lphp2->get_buffer_len());
				}
				return true;
			}


		};


		middleware_asio_server* create_server_protocol_mgt()
		{
			std::vector<boost::function<bool(const char*, uint32_t)> > ltemp(5);
			for (uint32_t i = 0; i < 5; ++i)
			{
				ltemp[i] = boost::bind(
					&mgt_protocol<spack_head::protocol_head, spack_head::protocol_head>::run_task,
					new mgt_protocol<spack_head::protocol_head, spack_head::protocol_head>(5, 1024),
					i, 0, _1, _2);
			}

			middleware::socket_asio_arg larg(5, ltemp);

			larg.m_activ = false;
			larg.m_extern_activ = false;

			larg.m_everyoncemaxsize = 1024;
			larg.m_extern_everyoncemaxsize = 1024;
			larg.m_extern_loopbuffermaxsize = 10240;
			larg.m_loopbuffermaxsize = 10240;
			larg.m_heartbeat_num = 32;
			larg.m_persecond_recvdatatbyte_num = 1024;
			larg.m_port = 13140;
			larg.m_recvpack_maxsize = 1024;
			larg.m_timeout = 10240;
			larg.m_s2c = true;
			larg.m_s2s = true;
			larg.m_session_num = 10240;
			return new middleware_asio_server(larg);
		}

		middleware_asio_client* create_client_protocol_mgt()
		{
			 //boost::bind(
			//		&mgt_protocol<cpack_head::protocol_head, cpack_head::protocol_head>::run_task,
			//		new mgt_protocol<cpack_head::protocol_head, cpack_head::protocol_head>(5, 1024),
			//		0, 0, _1, _2);
			//middleware::middleware_asio_client lclient(boost::bind(&rcb, false, _1, _2, _3), 10240, 1024);
			//lclient.create_connect(0, "127.0.0.1", 13140, sfcb);

			return nullptr;
		}


	} // namespace tools
}//namespace middleware
#endif //MGT_PROTOCOL_HPP

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
