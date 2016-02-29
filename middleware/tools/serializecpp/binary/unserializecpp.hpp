///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef UNSERIALIZECPP_H
#define UNSERIALIZECPP_H

#include "middleware/tools/serializecpp/public/endian/endian_local2net.hpp"

#include <cstdlib>

namespace middleware {
  namespace tools {
    /**
     *  反序列化基类
     */
    class unserializecpp_base
    {
      static void pop(void* aptarget, void* apsource, uint32_t aicopysize)
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
        gendian_local2net.endian(aivalues, aplen);
      }
    public:
      /**
       *  map pop
       */
      template <typename T_DATA>
      static uint32_t pop_map(char* ap, uint32_t aplen, T_DATA& aivaluesarr)
      {
        typedef typename T_DATA::key_type   first_type;
        typedef typename T_DATA::mapped_type   second_type;

        if (aplen < sizeof(uint16_t))
        {
          return 0;
        }

        uint16_t larraysize;
        pop(ap, aplen, larraysize);
        ap += sizeof(uint16_t);
        uint32_t lsize = sizeof(first_type) * larraysize + sizeof(second_type) * larraysize + sizeof(uint16_t);

        std::pair<first_type, second_type> ltempkey;
        for (uint16_t i = 0; i < larraysize; ++i)
        {
          unserializecpp_base::pop((void*)&(ltempkey.first), ap, sizeof(first_type));
        //  endian(ltempkey.first);
          ap += sizeof(first_type);
          aplen -= sizeof(first_type);
          unserializecpp_base::pop((void*)&(ltempkey.second), ap, sizeof(second_type));
        //  endian(ltempkey.second);
          ap += sizeof(second_type);
          aplen -= sizeof(second_type);
          aivaluesarr.insert(ltempkey);
        }
        return lsize;
      }

      /**
       *  set pop
       */
      template <typename T_DATA>
      static uint32_t pop_set(char* ap, uint32_t aplen, T_DATA& aivaluesarr)
      {
        if (aplen < sizeof(uint16_t))
        {
          return 0;
        }

        uint16_t larraysize;
        pop(ap, aplen, larraysize);
        ap += sizeof(uint16_t);
        uint32_t lsize = sizeof(uint16_t) + larraysize * sizeof(typename T_DATA::value_type);

        typename T_DATA::value_type ltemp;
        for (uint16_t i = 0; i < larraysize; ++i)
        {
          unserializecpp_base::pop((void*)(&ltemp), ap, sizeof(typename T_DATA::value_type));
          ap += sizeof(typename T_DATA::value_type);

    //      endian(ltemp);
          aivaluesarr.insert(ltemp);
        }

        return lsize;
      }

 

      /**
       * 内置类型
       * int
       * float
       * double
       * ....
       */
      template <typename T_DATA>
      static uint32_t pop(char* ap, uint32_t aplen, T_DATA& aivalues)
      {

        if (aplen < sizeof(T_DATA))
        {
          return 0;
        }

        unserializecpp_base::pop(&aivalues, ap, sizeof(T_DATA));
        return sizeof(T_DATA);
      }

      /**
       *  原始数组
       */
      template <typename T_DATA>
      static uint32_t pop(char* ap, uint32_t aplen, T_DATA* aivaluesarr, uint32_t aivaluesarrsize)
      {

        if (aplen < sizeof(uint16_t))
        {
          return 0;
        }

        uint16_t larraysize;
        pop(ap, aplen, larraysize);
        ap += sizeof(uint16_t);
        uint32_t lsize = sizeof(uint16_t) + larraysize * sizeof(T_DATA);

        if (larraysize > aivaluesarrsize || lsize > aplen)
        {
          return 0;
        }

        unserializecpp_base::pop((void*)(aivaluesarr), ap, larraysize * sizeof(T_DATA));

        endian((typename std::remove_const<T_DATA>::type*)aivaluesarr, larraysize);

        return lsize;
      }

      /**
       * vector 数组
       */
      template <typename T_DATA>
      static uint32_t pop(char* ap, uint32_t aplen, std::vector<T_DATA>& aivaluesarr)
      {
        if (aplen < sizeof(uint16_t))
        {
          return 0;
        }

        uint16_t larraysize;
        pop(ap, aplen, larraysize);
        ap += sizeof(uint16_t);
        uint32_t lsize = sizeof(uint16_t) + larraysize * sizeof(T_DATA);

        if (lsize > aplen)
        {
          return 0;
        }

        aivaluesarr.resize(larraysize);

        unserializecpp_base::pop((void*)(aivaluesarr.data()), ap, larraysize * sizeof(T_DATA));
        return lsize;
      }

      /**
       *  std::string
       */
      static uint32_t pop(char* ap, uint32_t aplen, std::string& aivaluesarr)
      {
        if (aplen < sizeof(uint16_t))
        {
          return 0;
        }

        uint16_t larraysize;
        pop(ap, aplen, larraysize);
        ap += sizeof(uint16_t);
        uint32_t lsize = sizeof(uint16_t) + larraysize;

        if (lsize > aplen)
        {
          return 0;
        }

        aivaluesarr.resize(larraysize);
        unserializecpp_base::pop((void*)(aivaluesarr.data()), ap, larraysize);
        return lsize;
      }


    };

    /**
     *  反序列化类
     */
    class unserializecpp
    {
      /*
       *  返回可用空间
       */
      static uint32_t get_have_len(serializecpp_buffer& mp_buffer_data)
      {
        return mp_buffer_data.get_len() - mp_buffer_data.get_uselen();
      }
      
    public:
      /*
       * 基础
       */
      template <typename T_DATA>
      static bool pop(serializecpp_buffer& mp_buffer_data, KeyPlaceholder/*apkey占位*/,T_DATA& aivalues)
      {
        uint32_t lretvalues = unserializecpp_base::pop(mp_buffer_data.get_nowpos_buffer(), unserializecpp::get_have_len(mp_buffer_data), aivalues);
        mp_buffer_data.get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

      template <typename T_DATA>
      static bool pop(serializecpp_buffer& mp_buffer_data, KeyPlaceholder/*apkey占位*/,const T_DATA* aivalues, uint32_t ailen)
      {
        uint32_t lretvalues = unserializecpp_base::pop(mp_buffer_data.get_nowpos_buffer(), unserializecpp::get_have_len(mp_buffer_data), aivalues, ailen);
        mp_buffer_data.get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

    template <typename T_DATA>
      static bool pop_set(serializecpp_buffer& mp_buffer_data, KeyPlaceholder /*apkey占位*/,T_DATA& aivalues)
      {
        uint32_t lretvalues = unserializecpp_base::pop_set(mp_buffer_data.get_nowpos_buffer(), unserializecpp::get_have_len(mp_buffer_data), aivalues);
        mp_buffer_data.get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

    template <typename T_DATA>
      static bool pop_map(serializecpp_buffer& mp_buffer_data, KeyPlaceholder /*apkey占位*/,T_DATA& aivalues)
      {
        uint32_t lretvalues = unserializecpp_base::pop_map(mp_buffer_data.get_nowpos_buffer(), unserializecpp::get_have_len(mp_buffer_data), aivalues);
        mp_buffer_data.get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

      /** 自定义struct 需要实现bool pop(serializecpp_buffer& mp_buffer_data)方法 */
      template <typename T_DATA>
      static bool pop_struct(serializecpp_buffer& mp_buffer_data, KeyPlaceholder /*apkey占位*/,T_DATA& aivalues)
      {
        return aivalues.pop( mp_buffer_data );
      }

    };

  }  //namespace tools
}  //namespace middleware 

#endif //UNSERIALIZECPP_H
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
