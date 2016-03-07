#include "middleware/tools/protocol/protocol/test_client_protocol/protocol_test_client_map.hpp"
#include "middleware/tools/protocol/protocol/test_client_protocol/protocol_test_client_0.hpp"



namespace middleware {
  namespace tools {

    type_cprotocol_map protocol_test_client_map;
    void init_protocol_test_client()
    {
      protocol_test_client_map.insert( std::make_pair(E_TEST_CLIENT_0,(protocol_test_client_0*)(nullptr))  );
    }


  } //namespace tools
} //namespace middleware
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
