///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef XMLJSON_SERIALIZECPP_HPP
#define XMLJSON_SERIALIZECPP_HPP

#include "middleware/tools/serializecpp/json/serializecpp_jsonbuffer.hpp"
#include "middleware/tools/serializecpp/xml/serializecpp_xmlbuffer.hpp"
#include "middleware/tools/serializecpp/public/serializecpp_fun.hpp"

namespace middleware {
  namespace tools {

    template <typename T_STAND>
    class serialize_stand
    {
    public:
      /**
      * 自定义类型 需要实现 bool push_struct(T_STAND& asj, const char* aikey)方法
      */
      template <typename T_DATA, typename T_DATA2>
      static void push_struct(T_STAND& asj, const char* aikey, T_DATA& aivalues, T_DATA2& appush)
      {
        aivalues.push(appush, aikey);
      }

      /** 基础类型 */
    template <typename T_DATA>
    static void push(T_STAND& asj, const char* aikey, const T_DATA& aivalues)
    {                                                                                  
      asj.add_single(aikey, aivalues);
    }

     /**
      *  原始数组
      */
      template <typename T_DATA>
      static void push(T_STAND& asj, const char* aikey, T_DATA* aivaluesarr, uint32_t aivaluesarrsize)
      {
        boost::property_tree::ptree lptree;
        lptree.put<uint32_t>("size", aivaluesarrsize);
        char chbuf[1024];
        int lplen = Binary2Cstr((unsigned char*)(aivaluesarr), aivaluesarrsize*sizeof(T_DATA), (unsigned char*)chbuf, 1024);
        lptree.put<std::string>("data", chbuf);
        asj.add_node(aikey, lptree);
      }

      /**
      * vector数组
      */
      template <typename T_DATA>
      static void push(T_STAND& asj, const char* aikey, std::vector<T_DATA>& aivaluesarr)
      {
        serialize_stand<T_STAND>::push(asj, aikey, aivaluesarr.data(), aivaluesarr.size());
      }

      /**
      * std::string
      */
      static void push(T_STAND& asj, const char* aikey, std::string& aivaluesarr)
      {
        asj.add_single(aikey, aivaluesarr);
      }

      /**
      *  关联容器中的set类
      */
      template <typename T_DATA>
      static void push_set(T_STAND& asj, const char* aikey, T_DATA& aivaluesarr)
      {
        std::vector<typename T_DATA::value_type> lvec;
        for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
        {
          lvec.push_back(*itor);
        }
        serialize_stand<T_STAND>::push(asj, aikey, lvec.data(), aivaluesarr.size());
      }

      template <typename T_DATA>
      static void push_map(T_STAND& asj, const char* aikey, T_DATA& aivaluesarr)
      {
        std::vector<typename T_DATA::key_type> lfirstvec;
        std::vector<typename T_DATA::mapped_type> lsecondvec;
        for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
        {
          lfirstvec.push_back(itor->first);
          lsecondvec.push_back(itor->second);
        }

        boost::property_tree::ptree lptree;
        uint32_t lisize = aivaluesarr.size();
        lptree.put<uint32_t>("size", lisize);
        char lchbuf[1024] = { 0 };
        Binary2Cstr((unsigned char*)(lfirstvec.data()), lisize*sizeof(typename T_DATA::key_type), (unsigned char*)lchbuf, 1024);
        lptree.put<std::string>("key", (char*)lchbuf);

        Binary2Cstr((unsigned char*)(lsecondvec.data()), lisize*sizeof(typename T_DATA::mapped_type), (unsigned char*)lchbuf, 1024);
        lptree.put<std::string>("val", (char*)lchbuf);
        asj.add_node(aikey, lptree);
      }
    };

  } //namespace tools
} //namespace middleware 

#endif //XMLJSON_SERIALIZECPP_HPP
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
