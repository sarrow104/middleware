///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef PROTOCOL_TEST_CLIENT_COUT
#define PROTOCOL_TEST_CLIENT_COUT

#include "middleware/tools/protocol/protocol_base.hpp"
#include "middleware/tools/protocol/protocol/test_client_protocol/protocol_test_client_num.hpp"

#include <string>

namespace middleware {
  namespace tools {

    class protocol_test_client_0 :
      public protocol_client_base
    {
      std::string mpop_ls;
    public:
			protocol_test_client_0() :
				protocol_client_base(E_TEST_CLIENT_0)
      {}

      virtual bool task(uint32_t aikey)
      {
        return 0;
      }
      virtual void serialization()
      {

      }
      virtual void unserialization()
      {
        m_premote2local->pop(mpop_ls);
      }
      virtual protocol_client_base* new_own()
      {
        return new protocol_test_client_0();
      }

    };

  } //namespace tools
}//namespace middleware
#endif //PROTOCOL_TEST_CLIENT_COUT

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
