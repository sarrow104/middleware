///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/protocol/mgt_protocol.hpp"
#include "middleware/middleware_base/middleware_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"
#include "middleware/tools/protocol/protocol/test_server_protocol/protocol_test_server_map.hpp"
#include "middleware/tools/protocol/protocol/test_client_protocol/protocol_test_client_map.hpp"

#include <iostream>


void test_middleware_asio_server()
{
  middleware::tools::init_protocol_test_server();
  middleware::tools::create_server_protocol_mgt(middleware::SERIALIZE_TYPE_XML,"E:/gitfile/middleware/src/tools/protocol/config/server.xml",middleware::tools::protocol_test_server_map);
}


/** send failure call back*/
bool sfcb(const char* ap, uint32_t aplen)
{
  std::cout << "send err" << std::endl;
  aplen = 0;
  return true;
};

void test_middleware_asio_client()
{
  //create_client_protocol_mgt();
  middleware::tools::init_protocol_test_client();
  auto lp = middleware::tools::create_client_protocol_mgt(
    middleware::SERIALIZE_TYPE_XML,
    "E:/gitfile/middleware/config/asio_client.xml",
    middleware::tools::protocol_test_client_map);
  middleware::tools::connect_server(
    lp,
    middleware::SERIALIZE_TYPE_XML,
    "E:/gitfile/middleware/config/asio_client.xml", 
    sfcb);
}




int main()
{
  test_middleware_asio_server();
  //test_middleware_asio_client();
  return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
