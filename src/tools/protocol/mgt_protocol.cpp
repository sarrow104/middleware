
#include "middleware/tools/protocol/mgt_protocol.hpp"

namespace middleware {
	namespace tools {

		middleware_asio_server* create_server_protocol_mgt(std::unordered_map<uint32_t, protocol_base<spack_head::protocol_head, spack_head::protocol_head>* >& apromap)
		{
			std::vector<boost::function<bool(const char*, uint32_t)> > ltemp(5);
			for (uint32_t i = 0; i < 5; ++i)
			{
				ltemp[i] = boost::bind(
					&mgt_protocol<spack_head::protocol_head, spack_head::protocol_head>::run_task,
					new mgt_protocol<spack_head::protocol_head, spack_head::protocol_head>(apromap, 5, 1024),
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


		middleware_asio_client* create_client_protocol_mgt(uint32_t aikey)
		{
			//   boost::bind(
			//      &mgt_protocol<cpack_head::protocol_head, cpack_head::protocol_head>::run_task,
			//     new mgt_protocol<cpack_head::protocol_head, cpack_head::protocol_head>(5, 1024),
			//     0, 0, _1, _2);
			// middleware::middleware_asio_client lclient(boost::bind(&rcb, false, _1, _2, _3), 10240, 1024);
			// lclient.create_connect(0, "127.0.0.1", 13140, sfcb);
			return nullptr;
		}


	} //namespace tools
} //namespace middleware
