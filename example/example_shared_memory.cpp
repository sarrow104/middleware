///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/shared_memory/shared_memory_middleware.hpp"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	if( argc >= 2  )
	{
		if(memcmp(argv[1],"-s",sizeof("-s")  ) == 0)
		{
			/** boost::function<bool ( char* ,uint32_t& )> */
			auto funservercallback = []( char* ap ,uint32_t& aplen)
			{
				cout << ap << endl;
				aplen = 0;
				return true;
			};
			middleware::middleware_sms  abc( "kk" , 1024 , 1024 , 128, funservercallback );
			uint32_t lnum = 900;
			char cbuf[128];
			uint32_t litemp;
			while( lnum )
			{
				litemp = sprintf( cbuf,"server2client %d" , lnum--);
				abc.send( cbuf , litemp+1 );
			}
		}
		else if(memcmp(argv[1],"-c",sizeof("-c")  ) == 0)
		{
			/** boost::function<bool ( char* ,uint32_t& )> */
			auto funclientcallback = []( char* ap ,uint32_t& aplen)
			{
				cout << ap << endl;
				aplen = 0;
				return true;
			};
			middleware::middleware_smc  abc( "kk", 1024, 1024, 128, funclientcallback );

			uint32_t lnum = 0;
			char cbuf[128];
			uint32_t litemp;
			while( lnum != 900 )
			{
				litemp = sprintf( cbuf,"client2server %d" , lnum++);
				abc.send( cbuf , litemp+1 );
			}
		}
		else
		{
			goto PRINT_ERR_RET;
		}

		while (1)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(20));
		}

	}
PRINT_ERR_RET:
	cout << "确保您输入正确的参数:" << endl;
	cout << "-s" << endl;
	cout << "-c" << endl;

	return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
