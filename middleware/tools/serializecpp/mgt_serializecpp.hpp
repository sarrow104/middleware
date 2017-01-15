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
#include "middleware/tools/serializecpp/public/macro_simplify.h"

#include <fstream>


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

      mgt_serializecpp(uint32_t aiseri, const char* aipath)
      {
        read(aiseri,aipath);
      }

      mgt_serializecpp(uint32_t aiseri, const char* aiconfigtxt,uint32_t apconfigtxtlen)
      {
        reset(aiseri,(char*)(aiconfigtxt), apconfigtxtlen);
      }

      void reset( uint32_t aiseri, char* ap, uint32_t aplen )
      {
        m_serialize_type = aiseri;
        GET_SERIALIZE_BUFFER( m_serialize_type, reset(ap,aplen))
          throw 0;
      }

      void reset(uint32_t aiseri)
      {
        m_serialize_type = aiseri;
        GET_SERIALIZE_BUFFER( m_serialize_type, reset())
          throw 0;
      }

      template <typename T>
      void push(T& apdata, const char* apkey = "")
        {
          GET_PUSH_SERIALIZE(m_serialize_type,push, apkey, apdata)
            throw 0;
        }

      template <typename T>
      void push(const T* aivalues, uint32_t ailen, const char* apkey = "")
      {
        GET_PUSH_SERIALIZE(m_serialize_type,push, apkey, aivalues,  ailen)
        throw 0;
      }

      template <typename T>
        void push_map(T& apdata, const char* apkey = "")
        {
          GET_PUSH_SERIALIZE(m_serialize_type,push_map, apkey, apdata)
            throw 0;
        }

      template <typename T>
        void push_set(T& apdata, const char* apkey = "")
        {
          GET_PUSH_SERIALIZE(m_serialize_type, push_set, apkey, apdata)
            throw 0;
        }

      template <typename T>
      void push_struct(T& apdata, const char* apkey = "")
      {
        GET_PUSH_SERIALIZE(m_serialize_type,push_struct, apkey, apdata, *this)
        throw 0;
      }

      template <typename T>
        void pop(T& aivalues, const char* apkey = "")
        {
          GET_POP_SERIALIZE(m_serialize_type, pop, apkey, aivalues)
            throw 0;
        }

    template <typename T>
    T pop(const char* apkey = "")
    {
      T lt;
      GET_POP_NOTRET_SERIALIZE(m_serialize_type, pop, apkey, lt)
      return std::move(lt);
    }

      template <typename T>
        void pop(const T* aivalues, uint32_t ailen, const char* apkey = "")
        {
          GET_POP_SERIALIZE(m_serialize_type,pop, apkey, aivalues, ailen)
            throw 0;
        }

      template <typename T>
        void pop_map(T& apdata, const char* apkey = "")
        {
          GET_POP_SERIALIZE(m_serialize_type, pop_map, apkey, apdata)
            throw 0;
        }


      template <typename T>
      void pop_set(T& apdata, const char* apkey = "")
      {
          GET_POP_SERIALIZE(m_serialize_type, pop_set, apkey, apdata)
          throw 0;
      }

      template <typename T>
      void pop_struct(T& apdata, const char* apkey = "")
      {
          GET_POP_SERIALIZE(m_serialize_type, pop_struct, apkey, apdata, *this)
          throw 0;
      }

      inline uint8_t gettype()const
      {
        return m_serialize_type;
      }

      inline uint32_t get_uselen()
      {
        GET_SERIALIZE_BUFFER( m_serialize_type, get_uselen())
          throw 0;
      }

      const char* get_buffer()
      {
        GET_SERIALIZE_BUFFER( m_serialize_type, get_buffer())
          throw 0;
      }

    void read( uint32_t aiseri,const char* aipath )
    {
      std::ifstream llogfile;
      llogfile.open(aipath,std::ios::binary);
          if (llogfile.is_open())
          {
            llogfile.seekg(0,std::ios::end);   
            size_t lsize = llogfile.tellg();
            llogfile.seekg(0,std::ios::beg);     
            char* lbuf = new char[lsize+1];
            lbuf[lsize] = '\0';
            llogfile.read(lbuf,lsize);
            reset( aiseri, lbuf, lsize );
            delete [] lbuf;
          }
          else
          {
            throw 1;
          }
    }
    };

    template <typename T>
    void pop_ptr_fun(
      middleware::tools::mgt_serializecpp& lp,
      T*& aidata, 
      const char* apkey)
    {
      aidata = new T;
      try
      {
        lp.pop(aidata, apkey);
      }
      catch (...)
      {
        delete aidata;
        aidata = nullptr;
      }
    }

    template <typename T>
    void pop_struct_ptr_fun(
      middleware::tools::mgt_serializecpp& lp,
      T*& aidata, 
      const char* apkey)
    {
      aidata = new T;
      try
      {
        lp.pop_struct(aidata, apkey);
      }
      catch (...)
      {
        delete aidata;
        aidata = nullptr;
      }
    }

  } //namespace tools
}  //namespace middleware



#endif //MGT_SERIALIZECPP_HPP
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
