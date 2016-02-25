///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SERIALIZECPP_JSON_BUFFER_HPP
#define SERIALIZECPP_JSON_BUFFER_HPP

#include "middleware/tools/serializecpp/public/xml_json/xmljson_buffer.hpp"

namespace middleware {
  namespace tools {
  
    struct serializecpp_jsonbuffer:
      public xmljson_buffer
    {
      virtual const char* get_buffer()
      {
        std::stringstream lst;
        boost::property_tree::write_json(lst, m_root);
        m_str = lst.str();
        return m_str.c_str();
      }

      virtual void reset(const char* ap, uint32_t aplen)
      {
        m_str.clear();
        std::stringstream ss(ap);
        boost::property_tree::read_json<boost::property_tree::ptree>(ss, m_root);
      }

      virtual void reset()
      {
        this->clear();
      }
    };


  } //namespace middleware
} //namespace tools

#endif //SERIALIZECPP_JSON_BUFFER_HPP
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
