///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef XMLJSON_UNSERIALIZECPP_HPP
#define XMLJSON_UNSERIALIZECPP_HPP

#include "middleware/tools/serializecpp/json/serializecpp_jsonbuffer.hpp"
#include "middleware/tools/serializecpp/xml/serializecpp_xmlbuffer.hpp"
#include "middleware/tools/serializecpp/public/serializecpp_fun.hpp"

#include <cstdint>

#ifndef POP_POP_BASE_TYPE
#define POP_POP_BASE_TYPE(TYPE)                                                                       \
     static void pop(T_STAND& asj, const char* aikey, TYPE& aivalues)                     \
     {                                                                                                    \
        asj.template get<TYPE>(aikey,aivalues);                                                                    \
     }
#endif //POP_JSONBASE_TYPE

namespace middleware {
  namespace tools {

    template <typename T_STAND>
    class unserialize_stand
    {
    public:
      /**
      * 自定义类型
      */
      template <typename T_DATA>
      static void pop(T_STAND& asj, const char* aikey, T_DATA& aivalues)
      {
        aivalues.pop(asj, aikey, aivalues);
      }

      POP_POP_BASE_TYPE(uint8_t)
      POP_POP_BASE_TYPE(uint16_t)
      POP_POP_BASE_TYPE(uint32_t)
      POP_POP_BASE_TYPE(uint64_t)
      POP_POP_BASE_TYPE(float)
      POP_POP_BASE_TYPE(double)

      /**
       *  原始数组
       */
      template <typename T_DATA>
      static void pop(T_STAND& asj, const char* aikey, T_DATA* aivaluesarr, uint32_t& aivaluesarrsize)
      {
        boost::property_tree::ptree& lret = asj.get_child(aikey);
        std::string ldatastr;
        asj.template get<std::string>("data", ldatastr);
        Cstr2Binary(ldatastr.c_str(), (unsigned char*)aivaluesarr, sizeof(T_DATA)*aivaluesarrsize);
        asj.template get<std::uint32_t>("size", aivaluesarrsize);
      }

      /**
      * vector 数组
      */
      template <typename T_DATA>
      static void pop(T_STAND& asj, const char* aikey, std::vector<T_DATA>& aivaluesarr)
      {
        boost::property_tree::ptree& lret = asj.get_child(aikey);
        uint32_t lisize = lret.get<std::uint32_t>("size");
        aivaluesarr.resize(lisize);
        std::string ldatastr = lret.get<std::string>("data");
        Cstr2Binary((unsigned char*)ldatastr.c_str(), (unsigned char*)aivaluesarr.data(), sizeof(T_DATA)*lisize);
      }

      /**
      * string
      */
      static void pop(T_STAND& asj, const char* aikey, std::string& aivaluesarr)
      {
        boost::property_tree::ptree& lret = asj.get_child(aikey);
        uint32_t lisize = lret.get<std::uint32_t>("size");
        aivaluesarr.resize(lisize);
        std::string ldatastr = lret.get<std::string>("data");
        Cstr2Binary((unsigned char*)ldatastr.c_str(), (unsigned char*)aivaluesarr.data(), lisize);
      }

      /**
      *  关联容器中的set类
      */
      template <typename T_DATA>
      static void pop_set(T_STAND& asj, const char* aikey, T_DATA& aivaluesarr)
      {
        std::vector<typename T_DATA::value_type> lvec;
        unserialize_stand<T_STAND>::pop(asj, aikey, lvec);
        aivaluesarr.insert(lvec.begin(), lvec.end());
      }
      template <typename T_DATA >
      static void pop_map(T_STAND& asj, const char* aikey, T_DATA& aivaluesarr)
      {
        boost::property_tree::ptree& lret = asj.get_child(aikey);
        std::string lkey;
        asj.template get<std::string>(lret, "key", lkey);
        std::string lval;
        asj.template get<std::string>(lret, "val", lval);

        uint32_t lsize;
        asj.template get<std::uint32_t>(lret, "size", lsize);

        std::vector<typename T_DATA::key_type> lfirstvec;
        std::vector<typename T_DATA::mapped_type> lsecondvec;

        lfirstvec.resize(lsize);
        lsecondvec.resize(lsize);
        Cstr2Binary((unsigned char*)lkey.c_str(), (unsigned char*)lfirstvec.data(), sizeof(T_DATA)*lsize);
        Cstr2Binary((unsigned char*)lval.c_str(), (unsigned char*)lsecondvec.data(), sizeof(T_DATA)*lsize);

        for (uint32_t i = 0; i < lsize; ++i)
        {
          aivaluesarr.insert(std::make_pair(lfirstvec[i], lsecondvec[i]));
        }
      }
    };

  } //namespace tools
} //namespace middleware 

#endif //XMLJSON_SERIALIZECPP_HPP

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
