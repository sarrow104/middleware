///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef   MSG_BASE_C_H
#define   MSG_BASE_C_H
#include "middleware/tools/serializecpp/binary/serializecpp_buffer.hpp"
#include "middleware/tools/serializecpp/public/endian/endian_local2net.hpp"

#include <string.h>

namespace middleware{
  namespace tools{

    /**
     *序列化基础类型
     */
    class serializecpp_base
    {
    private:
      static void push(void* aptarget, void* apsource, uint32_t aicopysize)
      {
        memcpy(aptarget, apsource, aicopysize);
      }

      template <typename T_DATA>
      static void endian(T_DATA& aivalues)
      {
        gendian_local2net.endian(aivalues);
      }

      template <typename T_DATA>
      static void endian(T_DATA* aivalues,uint32_t aplen)
      {
        gendian_local2net.endian(aivalues,aplen);
      }

    public:
      /**
       *  关联容器中的set类
       */
      template <typename T_DATA>
      static uint32_t push_set(char* ap, uint32_t aplen, T_DATA& aivaluesarr)
      {

        uint16_t lsetsize = aivaluesarr.size();
        uint32_t lsize = sizeof(typename T_DATA::value_type) * aivaluesarr.size() + sizeof(uint16_t);
        if (aplen < lsize)
        {
          return 0;
        }

        /** 数组长度 */
        serializecpp_base::push(ap, &lsetsize, sizeof(uint16_t));
        ap += sizeof(uint16_t);

        typename T_DATA::value_type ltemp;

        for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
        {
          ltemp = *itor;
          endian(ltemp);
          serializecpp_base::push(ap, &ltemp, sizeof(typename T_DATA::value_type));
          ap += sizeof(typename T_DATA::value_type);
          aplen -= sizeof(typename T_DATA::value_type);
        }
        return lsize;
      }

      /**
       *  关联容器中的map类
       */
      template <typename T_DATA>
      static uint32_t push_map(char* ap, uint32_t aplen, T_DATA& aivaluesarr)
      {

        uint16_t lsetsize = aivaluesarr.size();
        uint32_t lsize = sizeof(typename T_DATA::key_type) * aivaluesarr.size() + sizeof(typename T_DATA::mapped_type) * aivaluesarr.size() + sizeof(uint16_t);
        if (aplen < lsize)
        {
          return 0;
        }

        /** 数组长度 */
        serializecpp_base::push(ap, &lsetsize, sizeof(uint16_t));
        ap += sizeof(uint16_t);

        uint32_t ltemp = sizeof(typename T_DATA::key_type) + sizeof(typename T_DATA::mapped_type);
        typename T_DATA::key_type lfirsttemp;
        typename T_DATA::mapped_type lsecondtemp;
        for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
        {
          lfirsttemp = itor->first;
          lsecondtemp = itor->second;
          endian(lfirsttemp);
          endian(lsecondtemp);

          serializecpp_base::push(ap, &lfirsttemp, sizeof(typename T_DATA::value_type::first_type));
          ap += sizeof(typename T_DATA::value_type::first_type);
          aplen -= sizeof(typename T_DATA::value_type::first_type);
          serializecpp_base::push(ap, &lsecondtemp, sizeof(typename T_DATA::value_type::second_type));
          ap += sizeof(typename T_DATA::value_type::second_type);
          aplen -= sizeof(typename T_DATA::value_type::second_type);
        }
        return lsize;
      }

      
      /**
       * 基本类型+（不可以直接拷贝的结构字节序可以是个问题）
       */
    template <typename T_DATA>
    static uint32_t push(char* ap, uint32_t aplen, const T_DATA& aivalues)
    {                                                                                     
      if (aplen < sizeof(T_DATA))
      {                                                                                   
        return 0;                                                                         
      }                                                                                   
      push(ap, (void*)(&aivalues), sizeof(T_DATA));
      return sizeof(T_DATA);
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
        if (gendian_local2net.get_trans())
        {
          std::vector<char> lch;
          lch.resize(aivaluesarrsize*sizeof(T_DATA));
          memcpy(lch.data(), aivaluesarr, aivaluesarrsize*sizeof(T_DATA));
          uint16_t ltempaivaluesarrsize = aivaluesarrsize;
          endian(ltempaivaluesarrsize);
          serializecpp_base::push(ap, &ltempaivaluesarrsize, sizeof(uint16_t));
          ap += sizeof(uint16_t);
          endian((typename std::remove_const<T_DATA>::type*)lch.data(), aivaluesarrsize);
          serializecpp_base::push(ap, lch.data(), sizeof(T_DATA) * aivaluesarrsize);
        }
        else
        {
          /** 数组长度 */
          serializecpp_base::push(ap, &aivaluesarrsize, sizeof(uint16_t));
          ap += sizeof(uint16_t);
          serializecpp_base::push(ap, (void*)(aivaluesarr), sizeof(T_DATA) * aivaluesarrsize);
        }
       
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

    };

    /**
     *序列化类
     */
    class serializecpp
    {
      /** 返回可用空间 */
      static uint32_t get_have_len(serializecpp_buffer& ap_buffer_data)
      {
        return ap_buffer_data.get_len() - ap_buffer_data.get_uselen();
      }
    public:
      //基础
      template <typename T_DATA>
      static bool push(serializecpp_buffer& ap_buffer_data, KeyPlaceholder/*apkey占位*/,T_DATA& aivalues)
      {
        uint32_t lretvalues = serializecpp_base::push(ap_buffer_data.get_nowpos_buffer(), serializecpp::get_have_len(ap_buffer_data), aivalues);
        ap_buffer_data.get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

      template <typename T_DATA>
      static bool push(serializecpp_buffer& ap_buffer_data, KeyPlaceholder/*apkey占位*/, const T_DATA* aivalues, uint32_t ailen)
      {
        uint32_t lretvalues = serializecpp_base::push(ap_buffer_data.get_nowpos_buffer(), serializecpp::get_have_len(ap_buffer_data), aivalues, ailen);
        ap_buffer_data.get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

    template <typename T_DATA>
      static bool push_map(serializecpp_buffer& ap_buffer_data, KeyPlaceholder/*apkey占位*/, T_DATA& aivalues)
      {
        uint32_t lretvalues = serializecpp_base::push_map(ap_buffer_data.get_nowpos_buffer(), serializecpp::get_have_len(ap_buffer_data), aivalues);
        ap_buffer_data.get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

     template <typename T_DATA>
      static bool push_set(serializecpp_buffer& ap_buffer_data, KeyPlaceholder/*apkey占位*/, T_DATA& aivalues)
      {
        uint32_t lretvalues = serializecpp_base::push_set(ap_buffer_data.get_nowpos_buffer(), serializecpp::get_have_len(ap_buffer_data), aivalues);
        ap_buffer_data.get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

      template <typename T_DATA, typename T_DATA2>
      static bool push_struct(
        serializecpp_buffer& ap_buffer_data, 
        KeyPlaceholder/*apkey占位*/, 
        T_DATA& aivalues,
        T_DATA2& appush)
      {
        return aivalues.push(appush);
      }

    };
  } //namespace tools
} //namespace middleware

#endif
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
