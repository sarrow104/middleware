#include "gateway_client.h"
#include "gateway_server.h"
#include <iostream>
using namespace std;


void test_client()
{
	auto callfun = [](char* ap,uint32_t aplen)
	{
		cout << ap << endl;
		aplen = 0;
		return true;
	};

	gateway_socket_client_con lclient( callfun, 10240, 1024);
	lclient.create_con( 0 ,"127.0.0.1", 13140);
	char lbuf[] = "ÎÒ°®Äã";
	while(1)
	{
		lclient.send( 0,lbuf, sizeof( lbuf ) );
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}


	return;
}

void test_server()
{
	gateway_socket_server_con* lp;
	auto callfun = [&lp](char* ap,uint32_t aplen)
	{
		//cout << ap << endl;
		//aplen = 0;
		//lp->send( 0, "haha",sizeof("haha"));
		return true;
	};

	gateway_socket_server_con lclient( 13140, callfun, 10240, 1024);
	lp = &lclient;
	char lbuf[] = "ÎÒ°®Äã";
	while(1)
	{
		lclient.send( 0,lbuf, sizeof( lbuf ) );
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}

	return;
}


int main()
{
	boost::thread( function<void()>(test_server) );
	boost::thread( function<void()>(test_client) );

	while (1)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}
	return 0;
}