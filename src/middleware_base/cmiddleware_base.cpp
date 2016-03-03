///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/cmiddleware_base.h"
#include "middleware/middleware_base/middleware_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

extern "C" {


void* init_middleware_sm_server(
	uint32_t aiconfigtype,
	const char* aiconfigpath,
	callbackfun logic_fun
  )
{
  return new middleware::middleware_sm_server(
		aiconfigtype, aiconfigpath, logic_fun
    );
}

void* init_middleware_sm_server2(
	uint32_t aiconfigtype,
	const char* apconfigtxt,
	uint32_t apconfigtxtlen,
	callbackfun logic_fun
	)
{
	return new middleware::middleware_sm_server(
		aiconfigtype, apconfigtxt, apconfigtxtlen, logic_fun
		);
}


void* init_middleware_sm_client(
	uint32_t aiconfigtype,
	const char* aiconfigpath,
	callbackfun logic_fun)
{
  return new middleware::middleware_sm_client(
		aiconfigtype, aiconfigpath, logic_fun
    );
}

void* init_middleware_sm_client2(
	int32_t aiconfigtype,
	const char* apconfigtxt,
	uint32_t apconfigtxtlen,
	callbackfun logic_fun)
{
	return new middleware::middleware_sm_client(
		aiconfigtype, apconfigtxt, apconfigtxtlen, logic_fun
		);
}



bool send_middleware(void* ap, char* apdata, uint32_t aiwlen)
{
  return static_cast<middleware::middleware_base*>(ap)->send( apdata, aiwlen );
}

bool close_middleware(void* ap)
{
  return static_cast<middleware::middleware_base*>(ap)->close( );
}



void* init_middleware_la_server(
	uint32_t aiconfigtype,
	const char* aiconfigpath,
	callbackfun logic_fun)
{
  return new middleware::middleware_la_server(
		aiconfigtype, aiconfigpath, logic_fun);
}

void* init_middleware_la_server2(
	uint32_t aiconfigtype,
	const char* apconfigtxt,
	uint32_t apconfigtxtlen,
	callbackfun logic_fun)
{
	return new middleware::middleware_la_server(
		aiconfigtype, apconfigtxt, apconfigtxtlen, logic_fun);
}

void* init_middleware_la_client(
	uint32_t aiconfigtype,
	const char* aiconfigpath,
	callbackfun logic_fun)
{
  return new middleware::middleware_la_client(
		aiconfigtype, aiconfigpath, logic_fun);
}

void* init_middleware_la_client2(
	uint32_t aiconfigtype,
	const char* apconfigtxt,
	uint32_t apconfigtxtlen,
	callbackfun logic_fun)
{
	return new middleware::middleware_la_client(
		aiconfigtype, apconfigtxt, apconfigtxtlen, logic_fun);
}




void* init_middleware_soio_server(
	uint32_t aiconfigtype,
	const char* aiconfigpath,
	multi_recv_fun logic_recv_callback,
	sendfailure_fun aisendfailure
	)
{
  return new middleware::middleware_soio_server(
		aiconfigtype, aiconfigpath, logic_recv_callback, aisendfailure);
}

void* init_middleware_soio_server2(
	uint32_t aiconfigtype,
	const char* apconfigtxt,
	uint32_t apconfigtxtlen,
	multi_recv_fun logic_recv_callback,
	sendfailure_fun aisendfailure
	)
{
	return new middleware::middleware_soio_server(
		aiconfigtype, apconfigtxt, apconfigtxtlen, logic_recv_callback, aisendfailure);
}

void* init_middleware_soio_client(
	uint32_t aiconfigtype,
	const char* aiconfigpath,
	multi_recv_fun logic_recv_callback
	)
{
  return new middleware::middleware_soio_client(
		aiconfigtype, aiconfigpath, logic_recv_callback);
}

void* init_middleware_soio_client2(
	uint32_t aiconfigtype,
	const char* apconfigtxt,
	uint32_t apconfigtxtlen,
	multi_recv_fun logic_recv_callback
	)
{
	return new middleware::middleware_soio_client(
		aiconfigtype, apconfigtxt, apconfigtxtlen, logic_recv_callback);
}

bool create_connkey_soio(
	void* ap,
	uint32_t aiconfigtype,
	const char* aiconfigpath,
	sendfailure_fun aisendfailure
	)
{
  return static_cast<middleware::middleware_soio_client*>(ap)->create_connkey(
		aiconfigtype, aiconfigpath, aisendfailure);
}

bool create_connkey_soio2(
	void* ap,
	uint32_t aiconfigtype,
	const char* apconfigtxt,
	uint32_t apconfigtxtlen,
	sendfailure_fun aisendfailure
	)
{
	return static_cast<middleware::middleware_soio_client*>(ap)->create_connkey(
		aiconfigtype, apconfigtxt, apconfigtxtlen, aisendfailure);
}

bool create_connect_soio(
	void* ap,
	uint32_t aiconfigtype,
	const char* aiconfigpath,
	sendfailure_fun aisendfailure
	)
{
	return static_cast<middleware::middleware_soio_client*>(ap)->create_connect(
		aiconfigtype, aiconfigpath, aisendfailure);
}

bool create_connect_soio2(
	void* ap,
	uint32_t aiconfigtype,
	const char* apconfigtxt,
	uint32_t apconfigtxtlen,
	sendfailure_fun aisendfailure
	)
{
	return static_cast<middleware::middleware_soio_client*>(ap)->create_connect(
		aiconfigtype, apconfigtxt, apconfigtxtlen, aisendfailure);
}

bool send_middleware_soio(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen)
{
  return static_cast<middleware::socket_middleware_base*>(ap)->send(aikey, apdata, aiwlen);
}

bool close_middleware_soio(void* ap, uint32_t aikey)
{
  return static_cast<middleware::socket_middleware_base*>(ap)->close(aikey);
}


void init_middleware_asio_server_1part(
	uint32_t aiconfigtype,
	const char* aiconfigpath
	)
{
	middleware::middleware_asio_server::read_config(aiconfigtype, aiconfigpath);
}

void init_middleware_asio_server_1part2(
	uint32_t aiconfigtype,
	const char* apconfigtxt,
	uint32_t apconfigtxtlen
	)
{
	middleware::middleware_asio_server::read_config(aiconfigtype, apconfigtxt, apconfigtxtlen);
}

uint32_t init_middleware_asio_server_2part(
	)
{
	return middleware::middleware_asio_server::get<uint32_t>("pthread_num");
}

void* init_middleware_asio_server_3part(
	callbackfun* apcallackfunarr,
	uint32_t aicallbackfunsize
	)
{
	std::vector<boost::function<bool(const char*, uint32_t)> > lveccallback;
	if (init_middleware_asio_server_2part() == aicallbackfunsize)
	{
		lveccallback.resize(aicallbackfunsize);
		for (uint32_t i = 0; i < aicallbackfunsize;++i)
		{
			lveccallback[i] = apcallackfunarr[i];
		}

		return &middleware::middleware_asio_server::get_single(lveccallback);
	}
	else
	{
		return NULL;
	}
}


void test_thread( test_fun fun)
{
  boost::thread( boost::function<void()>(fun) );
}

void boost_sleep(uint32_t ainum)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(ainum));
}


}
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
