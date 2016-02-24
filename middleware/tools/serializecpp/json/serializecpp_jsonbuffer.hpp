#ifndef SERIALIZECPP_JSON_BUFFER_HPP
#define SERIALIZECPP_JSON_BUFFER_HPP

#include "middleware/tools/serializecpp/public/xml_json/xmljson_buffer.hpp"


namespace middleware {
	namespace tools {
	
		struct serializecpp_jsonbuffer:
			public xmljson_buffer
		{
			virtual void get_data(std::stringstream& st)
			{
				boost::property_tree::write_json(st, m_root);
			}

			virtual void reset(const char* ap, uint32_t aplen)
			{
				boost::property_tree::read_json<boost::property_tree::ptree>(std::stringstream(ap), m_root);
			}
		};


	} //namespace middleware
} //namespace tools

#endif //SERIALIZECPP_JSON_BUFFER_HPP
