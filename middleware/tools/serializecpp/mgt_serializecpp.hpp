///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef MGT_SERIALIZECPP_HPP
#define MGT_SERIALIZECPP_HPP

#include "middleware/tools/serializecpp/binary/serializecpp.hpp"
#include "middleware/tools/serializecpp/binary/unserializecpp.hpp"
#include "middleware/tools/serializecpp/json/serializecpp_json.hpp"
#include "middleware/tools/serializecpp/json/unserializecpp_json.hpp"
#include "middleware/tools/serializecpp/xml/serializecpp_xml.hpp"
#include "middleware/tools/serializecpp/xml/unserializecpp_xml.hpp"

namespace middleware {
  /**
   * 协议类型
   */
  enum
  {
    SERIALIZE_TYPE_BINARY,                              // 二进制 
    SERIALIZE_TYPE_JSON,                                //  JSON 
    SERIALIZE_TYPE_XML,                                 //  XML 
  };

  namespace tools{

    class mgt_serializecpp
    {
      serializecpp_buffer m_sbuf;
      serializecpp_jsonbuffer m_json_sbuf;
      serializecpp_xmlbuffer m_xml_sbuf;
      uint8_t m_serialize_type;                     /** 序列化类型 */
    public:
      mgt_serializecpp( uint32_t aibytes) :
        m_sbuf(aibytes)
      {}

      mgt_serializecpp() {}

#define GET_SERIALIZE_BUFFER( TYPE, FUN )  \
	switch(TYPE)\
	  {\
	case SERIALIZE_TYPE_BINARY:\
	 return m_sbuf.FUN;\
	 case SERIALIZE_TYPE_JSON:\
	 return m_json_sbuf.FUN;\
	 case SERIALIZE_TYPE_XML:\
		return m_xml_sbuf.FUN;\
        }

#define GET_PUSH_SERIALIZE( TYPE, FUN, ... )  \
	switch(TYPE)\
	  {\
	case SERIALIZE_TYPE_BINARY:\
	tools::serializecpp::FUN(m_sbuf,__VA_ARGS__);\
	  return;\
	 case SERIALIZE_TYPE_JSON:\
	 tools::serializecpp_json::FUN(m_json_sbuf,__VA_ARGS__);\
		  return;\
	 case SERIALIZE_TYPE_XML:\
		tools::serializecpp_json::FUN(m_xml_sbuf,__VA_ARGS__);\
		  return;\
        }
#define GET_POP_SERIALIZE( TYPE, FUN )  \
	switch(TYPE)\
	  {\
	case SERIALIZE_TYPE_BINARY:\
	tools::unserializecpp::FUN;\
	  return;\
	 case SERIALIZE_TYPE_JSON:\
	 tools::unserializecpp::FUN;\
		  return;\
	 case SERIALIZE_TYPE_XML:\
		tools::unserializecpp::FUN;\
		  return;\
        }
      void reset( uint32_t aiseri, char* ap, uint32_t aplen )
      {
        m_serialize_type = aiseri;
		GET_SERIALIZE_BUFFER( m_serialize_type, reset(ap,aplen))
#if 0
		switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          m_sbuf.reset(ap,aplen);
          return;
        case SERIALIZE_TYPE_JSON:
          m_json_sbuf.reset(ap, aplen);
          return;
        case SERIALIZE_TYPE_XML:
          m_xml_sbuf.reset(ap, aplen);
          return;
        }*/
#endif 
		throw 0;
      }

      void reset(uint32_t aiseri)
      {
        m_serialize_type = aiseri;
		GET_SERIALIZE_BUFFER( m_serialize_type, reset())
#if 0
        switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          m_sbuf.reset();
          return;
        case SERIALIZE_TYPE_JSON:
          m_json_sbuf.reset();
          return;
        case SERIALIZE_TYPE_XML:
          m_xml_sbuf.reset();
          return;
        }
#endif 
        throw 0;
      }

      template <typename T>
      void push(T& apdata, const char* apkey = "")
      {
		  GET_PUSH_SERIALIZE(m_serialize_type,push, apkey, apdata)
#if 0
        switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          tools::serializecpp::push(m_sbuf,apkey, apdata);
          return;
        case SERIALIZE_TYPE_JSON:
          tools::serializecpp_json::push(m_json_sbuf, apkey, apdata);
          return;
        case SERIALIZE_TYPE_XML:
          tools::serializecpp_xml::push(m_xml_sbuf, apkey, apdata);
          return;
        }
#endif
        throw 0;
      }

      template <typename T>
      bool push(const T* aivalues, uint32_t ailen, const char* apkey = "")
      {
		   GET_PUSH_SERIALIZE(m_serialize_type,push, apkey, aivalues,  ailen)
       /* switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          tools::serializecpp::push(m_sbuf, apkey, aivalues,  ailen);
          return;
        case SERIALIZE_TYPE_JSON:
          tools::serializecpp_json::push(m_json_sbuf, apkey, aivalues, ailen);
          return;
        case SERIALIZE_TYPE_XML:
          tools::serializecpp_xml::push(m_xml_sbuf, apkey, aivalues, ailen);
          return;
        }*/
        throw 0;
      }


      template <typename T>
      void push_map(T& apdata, const char* apkey = "")
      {
		    GET_PUSH_SERIALIZE(m_serialize_type,push_map, apkey, apdata)
       /* switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          tools::serializecpp::push_map(m_sbuf, apkey, apdata);
          return;
        case SERIALIZE_TYPE_JSON:
          tools::serializecpp_json::push_map(m_json_sbuf, apkey, apdata);
          return;
        case SERIALIZE_TYPE_XML:
          tools::serializecpp_xml::push_map(m_xml_sbuf, apkey, apdata);
          return;
        }*/
        throw 0;
      }

      template <typename T>
      void push_set(T& apdata, const char* apkey = "")
      {
		   GET_PUSH_SERIALIZE(m_serialize_type,push, apkey, apdata)
       /* switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          tools::serializecpp::push_set(m_sbuf, apkey, apdata);
          return;
        case SERIALIZE_TYPE_JSON:
          tools::serializecpp_json::push_set(m_json_sbuf, apkey, apdata);
          return;
        case SERIALIZE_TYPE_XML:
          tools::serializecpp_xml::push_set(m_xml_sbuf, apkey, apdata);
          return;
        }*/
        throw 0;
      }

      template <typename T>
      void push_struct(T& apdata, const char* apkey = "")
      {
		  GET_PUSH_SERIALIZE(m_serialize_type,push_struct, apkey, apdata)
      /*  switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          tools::serializecpp::push_struct(m_sbuf,apkey, apdata);
          return;
        case SERIALIZE_TYPE_JSON:
          tools::serializecpp_json::push_struct(m_json_sbuf, apkey, apdata);
          return;
        case SERIALIZE_TYPE_XML:
          tools::serializecpp_xml::push_struct(m_xml_sbuf, apkey, apdata);
          return;
        }*/
        throw 0;
      }

      template <typename T>
      void pop(T& aivalues, const char* apkey = "")
      {
		  GET_POP_SERIALIZE(m_serialize_type,pop(m_sbuf, apkey, aivalues))
        /*switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          tools::unserializecpp::pop(m_sbuf,apkey, aivalues);
          return;
        case SERIALIZE_TYPE_JSON:
          tools::unserializecpp_json::pop(m_json_sbuf, apkey, aivalues);
          return;
        case SERIALIZE_TYPE_XML:
          tools::unserializecpp_xml::pop(m_xml_sbuf, apkey, aivalues);
          return;
        }*/
        throw 0;
      }

      template <typename T>
      void pop(const T* aivalues, uint32_t ailen, const char* apkey = "")
      {
		  GET_POP_SERIALIZE(m_serialize_type,pop(m_sbuf, apkey, aivalues, ailen))
       /* switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          tools::unserializecpp::pop(m_sbuf,apkey, aivalues, ailen);
          return;
        case SERIALIZE_TYPE_JSON:
          tools::unserializecpp_json::pop(m_json_sbuf, apkey, aivalues, ailen);
          return;
        case SERIALIZE_TYPE_XML:
          tools::unserializecpp_xml::pop(m_xml_sbuf, apkey, aivalues, ailen);
          return;
        }*/
        throw 0;
      }

      inline uint8_t gettype()const
      {
        return m_serialize_type;
      }

      inline uint32_t get_uselen()
      {
		  GET_SERIALIZE_BUFFER( m_serialize_type, get_uselen())
        /*switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          return m_sbuf.get_uselen();
        case SERIALIZE_TYPE_JSON:
          return m_json_sbuf.get_uselen();
        case SERIALIZE_TYPE_XML:
          return m_xml_sbuf.get_uselen();
        }*/
        throw 0;
      }

      const char* get_buffer()
      {
		  GET_SERIALIZE_BUFFER( m_serialize_type, get_buffer())
        /*switch (m_serialize_type)
        {
        case SERIALIZE_TYPE_BINARY:
          return m_sbuf.get_buffer();
        case SERIALIZE_TYPE_JSON:
          return m_json_sbuf.get_buffer();
        case SERIALIZE_TYPE_XML:
          return m_xml_sbuf.get_buffer();
        }*/
        throw 0;
      }
    };


  } //namespace tools
}  //namespace middleware



#endif //MGT_SERIALIZECPP_HPP
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
