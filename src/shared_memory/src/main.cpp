#include <iostream>
using namespace std;
#include "../include/shared_memory_middleware.h"



int main(int argc, char *argv[])  
{
	
	if(memcmp(argv[1],"-s",sizeof("-s")  ) == 0)
	{
		//boost::function<bool ( char* ,uint32_t& )>
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
	else
	{
		//boost::function<bool ( char* ,uint32_t& )>
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



	while (1)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}

	return 0;
}