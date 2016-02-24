#ifndef SERIALIZECPP_XML_BUFFER_HPP
#define SERIALIZECPP_XML_BUFFER_HPP

#include "middleware/tools/serializecpp/public/xml_json/xmljson_buffer.hpp"

namespace middleware {
	namespace tools {
	
		struct serializecpp_xmlbuffer :
			public xmljson_buffer
		{
			virtual void get_data(std::stringstream& st)
			{
				boost::property_tree::write_xml(st, m_root);
			}

			virtual void reset(const char* ap, uint32_t aplen)
			{
				boost::property_tree::read_xml<boost::property_tree::ptree>(std::stringstream(ap), m_root);
			}
		};

	} //namespace middleware
} //namespace tools

#endif //SERIALIZECPP_JSON_BUFFER_HPP
