///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef UNSERIALIZECPP_XML_HPP
#define UNSERIALIZECPP_XML_HPP


#include "middleware/tools/serializecpp/public/xml_json/xmljson_unserializecpp.hpp"

namespace middleware {
  namespace tools {

    /**
     *  xml鍙嶅簭鍒楀寲
     */
    typedef  unserialize_stand<serializecpp_xmlbuffer> unserializecpp_xml;

  } //namespace tools
}// namespace middleware

#endif //UNSERIALIZECPP_XML_HPP

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */