#ifndef PARSER_JSON_HPP
#define PARSER_JSON_HPP

#include "middleware/tools/serializecpp/json/serializecpp_jsonbuffer.hpp"

namespace middleware{
  namespace tools {

    /**
    *序列化基础类型
    */
    class serializecpp_json_base
    {
    public:
      /**
      * 内置类型
      * int
      * float
      * double
      * ....
      * 和整个没有指针成员的struct结构
      */
      template <typename T_DATA>
      static void push(serializecpp_jsonbuffer& asj,const char* aikey, const T_DATA& aivalues)
      {
        boost::property_tree::ptree lptree;
        lptree.put<T_DATA>(aikey,aivalues);
        asj.add_node(lptree);
      }
      
      static void push(serializecpp_jsonbuffer& asj, const char* aikey, const int& aivalues)
      {
        boost::property_tree::ptree lptree;
        lptree.put<T_DATA>(aikey, aivalues);
        asj.add_node(lptree);
      }



      /**
       *  原始数组
       */
      template <typename T_DATA>
      static uint32_t push(char* ap, uint32_t aplen, T_DATA* aivaluesarr, uint32_t aivaluesarrsize)
      {
        uint32_t lsize = sizeof(T_DATA) * aivaluesarrsize + sizeof(uint16_t);
        if (aplen < lsize)
        {
          return 0;
        }
        /** 数组长度 */
        serializecpp_base::push(ap, &aivaluesarrsize, sizeof(uint16_t));
        ap += sizeof(uint16_t);
        serializecpp_base::push(ap, (void*)(aivaluesarr), sizeof(T_DATA) * aivaluesarrsize);
        return lsize;
      }

      /**
      * vector数组
      */
      template <typename T_DATA>
      static uint32_t push(char* ap, uint32_t aplen, std::vector<T_DATA>& aivaluesarr)
      {
        return serializecpp_base::push(ap, aplen, aivaluesarr.data(), aivaluesarr.size());
      }

      /**
      * std::string
      */
      static uint32_t push(char* ap, uint32_t aplen, std::string& aivaluesarr)
      {
        return serializecpp_base::push(ap, aplen, aivaluesarr.c_str(), aivaluesarr.size());
      }

#if 0
      /*set*/
      template <typename T_DATA>
      static uint32_t push(char* ap, uint32_t aplen, std::set<T_DATA>& aivaluesarr)
      {
        return serializecpp_base::push_set(ap, aplen, aivaluesarr);
      }

      /* hash set */
      template <typename T_DATA>
      static uint32_t push(char* ap, uint32_t aplen, std::unordered_set<T_DATA>& aivaluesarr)
      {
        return serializecpp_base::push_set(ap, aplen, aivaluesarr);
      }
#else
      DEF_PUSH_SET_TYPE(std::set)
        DEF_PUSH_SET_TYPE(std::unordered_set)
#endif

#if 0
        /* map*/
        template <typename T_DATA1, typename T_DATA2>
      static uint32_t push(char* ap, uint32_t aplen, std::map<T_DATA1, T_DATA2>& aivaluesarr)
      {
        return push_map(ap, aplen, aivaluesarr);
      }

      /* hash map*/
      template <typename T_DATA1, typename T_DATA2>
      static uint32_t push(char* ap, uint32_t aplen, std::unordered_map<T_DATA1, T_DATA2>& aivaluesarr)
      {
        return push_map(ap, aplen, aivaluesarr);
      }
#else
        DEF_PUSH_MAP_TYPE(std::map)
        DEF_PUSH_MAP_TYPE(std::unordered_map)
#endif

    };


    /**
    *序列化类
    */
    class serializecpp_json
    {
      /** 返回可用空间 */
      static uint32_t get_have_len(serializecpp_buffer* mp_buffer_data)
      {
        return mp_buffer_data->get_len() - mp_buffer_data->get_uselen();
      }
    public:
      //基础
      template <typename T_DATA>
      static bool push(serializecpp_buffer* mp_buffer_data, std::string& aikey , T_DATA& aivalues)
      {
        uint32_t lretvalues = serializecpp_base::push(mp_buffer_data->get_nowpos_buffer(), serializecpp::get_have_len(mp_buffer_data), aivalues);
        mp_buffer_data->get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

      template <typename T_DATA>
      static bool push(serializecpp_buffer* mp_buffer_data, const char* aikey, T_DATA& aivalues)
      {
        uint32_t lretvalues = serializecpp_base::push(mp_buffer_data->get_nowpos_buffer(), serializecpp::get_have_len(mp_buffer_data), aivalues);
        mp_buffer_data->get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }



      template <typename T_DATA>
      static bool push(serializecpp_buffer* mp_buffer_data, const T_DATA* aivalues, uint32_t ailen)
      {
        uint32_t lretvalues = serializecpp_base::push(mp_buffer_data->get_nowpos_buffer(), serializecpp::get_have_len(mp_buffer_data), aivalues, ailen);
        mp_buffer_data->get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }
    };


  } //namespace tools
}  //namespace middleware

#endif //PARSER_JSON_HPP

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
