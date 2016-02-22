///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/protocol/protocol/protocol_cout.hpp"
#include "middleware/tools/protocol/mgt_protocol.hpp"
#include "middleware/middleware_base/middleware_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

#include <iostream>


void test_middleware_asio_server()
{
	middleware::tools::create_server_protocol_mgt();
}

void test_middleware_asio_client()
{
	/*middleware::middleware_asio_client lclient(boost::bind(&rcb, false, _1, _2, _3), 10240, 1024);
	lclient.create_connect(0, "127.0.0.1", 13140, sfcb);

	middleware::pack_head_process<middleware::cpack_head::protocol_head > lphp( 1024 );
	std::string lstr("hello world");
	lphp.push(lstr);
	lphp.set_pack_head(middleware::PRO_NUM::EPN_COUT);
	while (1)
	{
		lclient.send(0, lphp.get_send_buffer(), lphp.get_send_len());
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}
	while (1)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}*/
}




int main()
{

	return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
