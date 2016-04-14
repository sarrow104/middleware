#include "middleware/tools/protocol/protocol/test_client_protocol/protocol_test_client_0.hpp"


namespace middleware {
  namespace tools {

    struct protocol_test_client_0::pop_data
    {
      uint32_t  m_num;
      std::string* m_str;
    };

    void protocol_test_client_0::init_data()
    {
      static std::string* lpstrarr = new std::string[m_maxpther];
      m_popdata->m_str = &(lpstrarr[m_pos]);
    }

    void protocol_test_client_0::serialization()
    {

    }

    void protocol_test_client_0::unserialization()
    {
      m_premote2local->pop(*m_popdata->m_str);
    }

    uint32_t protocol_test_client_0::task(uint32_t aikey)
    {
      return 0;
    }

  } //namespace tools
} //namespace middleware


 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
