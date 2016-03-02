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
      bool m_first;
      serializecpp_jsonbuffer() :
        m_first(true)
      {}

      virtual const char* get_buffer()
      {
        if (m_first)
        {
          m_first = false;
          /** 应该优化.. */
          add_node2("root", m_root);
        }
        std::stringstream lst;
        boost::property_tree::write_json(lst, m_root);
        m_str = lst.str();
        return m_str.c_str();
      }

      virtual void reset(const char* ap, uint32_t aplen)
      {
        m_first = true;
        m_str.clear();
        std::stringstream ss(ap);
        boost::property_tree::read_json<boost::property_tree::ptree>(ss, m_root);
        m_root = get_child( "root" );
      }

      virtual void reset()
      {
        m_first = true;
        this->clear();
      }
    };


  } //namespace middleware
} //namespace tools

#endif //SERIALIZECPP_JSON_BUFFER_HPP
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
