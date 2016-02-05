///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

﻿#include "middleware/middleware_base/middleware_base.hpp"

#include <iostream>

using std::cout;
using std::endl;


void test_middleware_sm_server()
{
	/** boost::function<bool ( char* ,uint32_t& )> */
	auto funservercallback = [](const char* ap, uint32_t aplen)
	{
		cout << ap << endl;
		return true;
	};
	middleware::middleware_sm_server  abc("kk", 1024, 1024, 128, funservercallback);
	uint32_t lnum = 900;
	char cbuf[128];
	uint32_t litemp;
	while (lnum)
	{
		litemp = sprintf(cbuf, "server2client %d", lnum--);
		abc.send(cbuf, litemp + 1);
	}
}

void test_middleware_sm_client()
{
	/** boost::function<bool ( char* ,uint32_t& )> */
	auto funclientcallback = [](const char* ap, uint32_t aplen)
	{
		cout << ap << endl;
		return true;
	};
	middleware::middleware_sm_client  abc("kk", 1024, 1024, 128, funclientcallback);

	uint32_t lnum = 0;
	char cbuf[128];
	uint32_t litemp;
	while (lnum != 900)
	{
		litemp = sprintf(cbuf, "client2server %d", lnum++);
		abc.send(cbuf, litemp + 1);
	}
}

void test_middleware_la_server()
{
	auto fun = [](const char* ap, uint32_t aplen)
	{
		cout << "server callback[" << ap << "]"<< endl;
		return true;
	};
	middleware::middleware_la_server lmc("lb", 10240, 1024, fun, false);
	uint32_t lsize = 1000;
	char lchar1[64];
	char lchar2[64];
	char*p1 = lchar1;
	char*p2 = lchar2;
	uint32_t llen = 0;
	while (lsize--)
	{
		std::swap(p1, p2);
		llen = snprintf(p1, 64, "%d", lsize);
		lmc.send(p1, llen + 1);
	}
}

void test_middleware_la_client()
{
	auto fun = [](const char* ap, uint32_t aplen)
	{
		cout << "server callback[" << ap << "]" << endl;
		return true;
	};
	middleware::middleware_la_client lmc("lb", 10240, 1024, fun, false);
	uint32_t lsize = 1000;
	char lchar1[64];
	char lchar2[64];
	char*p1 = lchar1;
	char*p2 = lchar2;
	uint32_t llen = 0;
	while (lsize--)
	{
		std::swap(p1, p2);
		llen = snprintf(p1, 64, "%d", lsize);
		lmc.send(p1, llen + 1);
	}
}


//void test_middleware_

int main(int argc, char *argv[])
{

	if (argc >= 3)
	{
		
		if (memcmp(argv[1], "-sm", sizeof("-sm")) == 0)/** 共享内存测试 */
		{
			if (memcmp(argv[2], "-c", sizeof("-c")) == 0)
			{
				test_middleware_sm_client();
			}
			else if(memcmp(argv[2], "-s", sizeof("-s")) == 0)
			{
				test_middleware_sm_server();
			}
		}
		else if (memcmp(argv[1], "-la", sizeof("-la")) == 0)
		{
			boost::thread(boost::function<void()>(test_middleware_la_server));
			boost::thread(boost::function<void()>(test_middleware_la_client));
		}
		else if (memcmp(argv[1], "-la", sizeof("-la")) == 0)
		{
		}

		getchar();


	}
















PRINT_ERR_RET:
	cout <<
				 "/** "<<endl;
	cout <<" *  THIS.EXE -TYPE  -CLIENT OR SERVER "<< endl;
	cout <<" *  -TYPE      sm   共享内存" << endl;
	cout <<" *             la   循环数组" << endl;
	cout <<" *" << endl;
	cout <<" *" << endl;
	cout <<" *  -CLIENT OR SERVER" << endl;
	cout <<" *  example：" << endl;
	cout <<" *          THIS.EXE -sm -c " << endl;
	cout <<" *" << endl;
	cout <<" */" << endl;

	return 0;
}
﻿
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
