///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/middleware_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

#include <iostream>


/** recv call back */
bool rcb(bool aisclient, uint32_t aikey, const char* ap, uint32_t aplen)
{
	middleware::unpack_head_process<middleware::cpack_head::protocol_head> luhp;
	luhp.reset(ap, aplen);
	char ch[sizeof("hello world")] = { 0 };
	luhp.pop(ch);
	std::cout << ch << std::endl;
	return true;
};

/** send failure call back*/
bool sfcb(const char* ap, uint32_t aplen)
{
	std::cout << "send err" << std::endl;
	aplen = 0;
	return true;
};

void test_middleware_asio_server()
{
	boost::function<bool(uint32_t, const char*, uint32_t)> apfun = [](uint32_t ainum, const char* ap, uint32_t aplen) {
		middleware::unpack_head_process<middleware::spack_head::protocol_head> luhp;
		luhp.reset(ap, aplen);
		char ch[sizeof("hello world")] = { 0 };
		luhp.pop(ch);
		std::cout << ch << std::endl;
		middleware::asio_server().send(ainum, ap, aplen);
		return true;
	};

	std::vector<boost::function<bool(const char*, uint32_t)> > ltemp(5);
	for (uint32_t i = 0; i < 5; ++i)
	{
		ltemp[i] = boost::bind(apfun, i, _1, _2);
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
	middleware::middleware_asio_server lser(larg);
	middleware::asio_server(&lser);
	while (1)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}
}

void test_middleware_asio_client()
{
	middleware::middleware_asio_client lclient(boost::bind(&rcb, false, _1, _2, _3), 10240, 1024);
	lclient.create_connect(0, "127.0.0.1", 13140, sfcb);

	middleware::pack_head_process<middleware::cpack_head::protocol_head > lphp(sizeof("hello world"));
	lphp.push("hello world");
	lphp.set_pack_head();
	while (1)
	{
		lclient.send(0, lphp.get_send_buffer(), lphp.get_send_len());
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}
	while (1)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}
}






int main(int argc, char *argv[])
{
	if (argc >= 3)
	{
		if (memcmp(argv[1], "-asio", sizeof("-asio")) == 0)
		{
			if (memcmp(argv[2], "-c", sizeof("-c")) == 0)
			{
				test_middleware_asio_client();
			}
			else if (memcmp(argv[2], "-s", sizeof("-s")) == 0)
			{
				test_middleware_asio_server();
			}
			else
			{
				goto PRINT_MESSAGE;
			}
		}
		else
		{
			goto PRINT_MESSAGE;
		}
		return 0;
	}

PRINT_MESSAGE:
	std::cout << "/** " << std::endl;
	std::cout << " *  THIS.EXE   -asio    [-c|-s]  " << std::endl;
	std::cout << " */" << std::endl;

    return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
