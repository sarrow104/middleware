#include "middleware/tools/protocol/protocol/test_server_protocol/protocol_test_server_map.hpp"
#include "middleware/tools/protocol/protocol/test_server_protocol/protocol_test_server_0.hpp"



namespace middleware {
  namespace tools {

    type_server_protocol_map protocol_test_server_map;
    void init_protocol_test_server()
    {
      protocol_test_server_map.insert( std::make_pair(E_TEST_SERVER_0,(protocol_test_server_0*)(nullptr))  );
    }


  } //namespace tools
} //namespace middleware
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
