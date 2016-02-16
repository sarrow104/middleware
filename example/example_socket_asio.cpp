///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/socket_asio/socket_asio_mgt.hpp"

int main()
{
	middleware::socket_asio_arg larg( 4 );

	larg.m_activ = false;
	larg.m_extern_activ = false;
	 boost::function<bool(const char*, uint32_t)> apfun = [](const char* ap, uint32_t aplen){
		std::cout << ap << std::endl;
		return true;
	};
	larg.m_callbackfun = &apfun;
	larg.m_everyoncemaxsize = 1024;
	larg.m_extern_everyoncemaxsize = 1024;
	larg.m_extern_loopbuffermaxsize = 10240;
	larg.m_loopbuffermaxsize = 10240;
	larg.m_heartbeat_num = 32;
	larg.m_persecond_recvdatatbyte_num =1024;
	larg.m_port = 13104;
	larg.m_recvpack_maxsize = 1024;
	larg.m_timeout = 10240;
	larg.m_s2c = true;
	larg.m_s2s = true;
	larg.m_session_num = 10240;

	middleware::socket_asio_server_mgt* lsasm = middleware::socket_asio_server_mgt::init( larg );

    return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
