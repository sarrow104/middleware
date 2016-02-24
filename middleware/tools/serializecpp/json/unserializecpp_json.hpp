#ifndef UNSERIALIZECPP_JSON_HPP
#define UNSERIALIZECPP_JSON_HPP

#include "middleware/tools/serializecpp/public/xml_json/xmljson_unserializecpp.hpp"

namespace middleware {
	namespace tools {

		/**
		 *  json�����л�
		 */
		typedef   unserialize_stand<serializecpp_jsonbuffer> unserializecpp_json;

	} //namespace tools
}// namespace middleware


#endif //UNSERIALIZECPP_JSON_HPP

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */