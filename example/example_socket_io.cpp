///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/socket_io/gateway_client.hpp"
#include "middleware/middleware_base/socket_io/gateway_server.hpp"

#include <iostream>

using namespace std;


void test_client()
{
  /** recv call back */
  auto rcb = [](const char* ap,uint32_t aplen)
  {
    cout << *( (uint32_t*)ap) << endl;
    aplen = 0;
    return true;
  };

  /** send failure call back*/
  auto sfcb = [](const char* ap, uint32_t aplen)
  {
    cout << *((uint32_t*)ap) << endl;
    aplen = 0;
    return true;
  };

  middleware::gateway_socket_client_con lclient(rcb, 10240, 1024);
  lclient.create_con( 0 ,"127.0.0.1", 13140, sfcb);
  char lbuf[] = "我爱你";
  while(1)
  {
    lclient.send( 0,lbuf, sizeof( lbuf ) );
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
  return;
}

void test_server()
{
  middleware::gateway_socket_server_con* lp;
  /** recv call back */
  auto rcb = [](const char* ap, uint32_t aplen)
  {
    cout << ap << endl;
    aplen = 0;
    return true;
  };

  /** send failure call back*/
  auto sfcb = [](const char* ap, uint32_t aplen)
  {
    cout << ap << endl;
    aplen = 0;
    return true;
  };

  middleware::gateway_socket_server_con lserver( 13140, rcb, 10240, 1024, sfcb);
  lp = &lserver;
  uint32_t lnum = 0;
  while(1)
  {
    lserver.send( 0, (const char*)&(++lnum), sizeof(uint32_t) );
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

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
