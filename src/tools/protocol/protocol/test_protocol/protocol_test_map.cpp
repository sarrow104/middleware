#include "middleware/tools/protocol/protocol/test_protocol/protocol_test_map.hpp"
#include "middleware/tools/protocol/protocol/test_protocol/protocol_test_0.hpp"



namespace middleware {
  namespace tools {

    type_server_protocol_map protocol_test_map;
    void init_protocol_test()
    {
      protocol_test_map.insert( std::make_pair(E_TEST_0,(protocol_test_0*)(nullptr))  );
    }


  } //namespace tools
} //namespace middleware
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
