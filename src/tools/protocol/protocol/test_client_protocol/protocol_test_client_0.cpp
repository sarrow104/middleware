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

	} //namespace tools
} //namespace middleware

