///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/protocol/mgt_protocol.hpp"
#include "middleware/middleware_base/middleware_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"
#include "middleware/tools/protocol/protocol/test_protocol/protocol_test_map.hpp"

#include <iostream>


void test_middleware_asio_server()
{
  middleware::tools::init_protocol_test();
  middleware::tools::create_server_protocol_mgt(middleware::tools::protocol_test_map);
}


/** recv call back */
bool rcb(bool aisclient, uint32_t aikey, const char* ap, uint32_t aplen)
{
  middleware::unpack_head_process<middleware::cpack_head::protocol_head> luhp;
  luhp.reset(ap, aplen);
  char ch[sizeof("hello world")] = { 0 };
  luhp.pop(ch, sizeof("hello world"));
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

void test_middleware_asio_client()
{
}




int main()
{
  return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
