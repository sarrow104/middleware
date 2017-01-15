///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SERIALIZACPP_XML_HPP
#define SERIALIZACPP_XML_HPP

#include "middleware/tools/serializecpp/public/xml_json/xmljson_serializecpp.hpp"

#include <cstdint>

namespace middleware{
  namespace tools {

    /**
    * xml序列化
    */
    typedef serialize_stand<serializecpp_xmlbuffer> serializecpp_xml;

  } //namespace tools
}  //namespace middleware

#endif //SERIALIZACPP_XML_HPP
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
