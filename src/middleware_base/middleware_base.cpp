///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)
#include "middleware/middleware_base/middleware_base.hpp"

#include "middleware/middleware_base/socket_asio/socket_asio_mgt.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

namespace middleware {

	middleware_asio_server::middleware_asio_server(socket_asio_arg& aiarg)
	{
		/** 初始化 */
		socket_asio_server_mgt::init(aiarg);
		m_send = aiarg.m_middlewarearr;
		socket_asio_server_mgt::start();
	}

	bool middleware_asio_server::send(uint32_t aikey, const char* apdata, uint32_t aiwlen)
	{
		return m_send[aikey]->send( apdata, aiwlen);
	}

	bool middleware_asio_server::close(uint32_t aikey)
	{
		return m_send[aikey]->close();
	}

	uint8_t middleware_asio_server::type()
	{
		return E_MW_TYPE::E_ASIO_SERVER;
	}


} //namespace middleware
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
