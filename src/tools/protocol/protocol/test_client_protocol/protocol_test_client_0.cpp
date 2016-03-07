#include "middleware/tools/protocol/protocol/test_client_protocol/protocol_test_client_0.hpp"


namespace middleware {
	namespace tools {

		struct protocol_test_client_0::pop_data
		{
			uint32_t  m_num;
			std::string* m_str;
		};

		void protocol_test_client_0::init_data(char* aprecv, char* apsend)
		{
			static std::string* lpstr = new std::string();
			((pop_data*)(aprecv))->m_str = lpstr;
			
		}

	} //namespace tools
} //namespace middleware

