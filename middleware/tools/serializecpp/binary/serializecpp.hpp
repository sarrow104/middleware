///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef   SERIALIZECPP_HPP
#define   SERIALIZECPP_HPP
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

        uint32_t lsetsize = aivaluesarr.size();
        uint32_t lsize = sizeof(typename T_DATA::value_type) * lsetsize + sizeof(uint32_t);
        if (aplen < lsize)
        {
          return 0;
        }

        /** 数组长度 */
		endian(lsetsize);
        serializecpp_base::push(ap, &lsetsize, sizeof(uint32_t));
        ap += sizeof(uint32_t);

        typename T_DATA::value_type ltemp;

        for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
        {
          ltemp = *itor;
          endian(ltemp);
          serializecpp_base::push(ap, &ltemp, sizeof(typename T_DATA::value_type));
          ap += sizeof(typename T_DATA::value_type);
        }
        return lsize;
      }

      /**
       *  关联容器中的map类
       */
      template <typename T_DATA>
      static uint32_t push_map(char* ap, uint32_t aplen, T_DATA& aivaluesarr)
      {

        uint32_t lsetsize = aivaluesarr.size();
        uint32_t lsize = sizeof(typename T_DATA::key_type) * lsetsize + sizeof(typename T_DATA::mapped_type) * lsetsize + sizeof(uint32_t);
        if (aplen < lsize)
        {
          return 0;
        }

        /** 数组长度 */
		endian(lsetsize);
        serializecpp_base::push(ap, &lsetsize, sizeof(uint32_t));
        ap += sizeof(uint32_t);

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
          serializecpp_base::push(ap, &lsecondtemp, sizeof(typename T_DATA::value_type::second_type));
          ap += sizeof(typename T_DATA::value_type::second_type);
        }
        return lsize;
      }

      
      /**
       * 基本类型
       */
    template <typename T_DATA>
    static uint32_t push(char* ap, uint32_t aplen, T_DATA& aivalues)
    {                                                                                     
      if (aplen < sizeof(T_DATA))
      {                                                                                   
        return 0;                                                                         
      }     
	  T_DATA ltemp(aivalues);
	  endian( ltemp );
	  push(ap, &ltemp, sizeof(T_DATA));
      return sizeof(T_DATA);
    }
      /**
       *  原始数组
       */
      template <typename T_DATA>
      static uint32_t push(char* ap, uint32_t aplen, T_DATA* aivaluesarr, uint32_t aivaluesarrsize)
      {
        uint32_t lsize = sizeof(T_DATA) * aivaluesarrsize + sizeof(uint32_t);
        if (aplen < lsize)
        {
          return 0;
		}

		/** 保存数组长度 */
		uint32_t ltempaivaluesarrsize = aivaluesarrsize;
		endian( ltempaivaluesarrsize );
		push( ap, aplen, ltempaivaluesarrsize );
		ap += sizeof(uint32_t);
		/** 保存数据 */
		std::vector<char> lch;
		lch.resize(aivaluesarrsize*sizeof(T_DATA));
		memcpy(lch.data(), aivaluesarr, aivaluesarrsize*sizeof(T_DATA));
		endian((typename std::remove_const<T_DATA>::type*)lch.data(), aivaluesarrsize);
		serializecpp_base::push(ap, lch.data(), sizeof(T_DATA) * aivaluesarrsize);

        return lsize;
      }
	/** 特化基础类型SpecializationBasisType */ 
	  template <typename T_DATA>
	  static uint32_t SpecializationBasisType(char* ap, uint32_t aplen, const T_DATA* aivaluesarr, uint32_t aivaluesarrsize)
	  {
		uint32_t lsize = sizeof(T_DATA) * aivaluesarrsize + sizeof(uint32_t);
        if (aplen < lsize)
        {
          return 0;
		}
		/** 保存数组长度 */
		uint32_t ltempaivaluesarrsize = aivaluesarrsize;
		endian( ltempaivaluesarrsize );
		serializecpp_base::push(ap, &ltempaivaluesarrsize, sizeof(uint32_t));
		ap += sizeof(uint32_t);
		/** 保存数据 */
		serializecpp_base::push(ap, (void*)aivaluesarr, sizeof(T_DATA) * aivaluesarrsize);
        return lsize;\
	  }
#define SB_PUSH_TYPE(TYPE)  \
	  static uint32_t push(char* ap, uint32_t aplen, const TYPE* aivaluesarr, uint32_t aivaluesarrsize)\
      {\
        return SpecializationBasisType(ap,aplen,aivaluesarr,aivaluesarrsize);\
      }

	  SB_PUSH_TYPE(uint8_t)
	  SB_PUSH_TYPE(int8_t)
	  SB_PUSH_TYPE(bool)
	  SB_PUSH_TYPE(float)
	  SB_PUSH_TYPE(double)

      /**
       * vector数组
       */
      template <typename T_DATA>
      static uint32_t push(char* ap, uint32_t aplen, std::vector<T_DATA>& aivaluesarr)
      {
        return serializecpp_base::push(ap, aplen, (T_DATA*)aivaluesarr.data(), aivaluesarr.size());
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
      static bool push(
        serializecpp_buffer& ap_buffer_data, 
        KeyPlaceholder/*apkey占位*/,
        T_DATA& aivalues)
      {
        uint32_t lretvalues = serializecpp_base::push(ap_buffer_data.get_nowpos_buffer(), serializecpp::get_have_len(ap_buffer_data), aivalues);
        ap_buffer_data.get_uselen() += lretvalues;
        return (lretvalues == 0) ? false : true;
      }

    //指针类型
    template <typename T_DATA>
    static bool push(
      serializecpp_buffer& ap_buffer_data, 
      KeyPlaceholder/*apkey占位*/,
      T_DATA* aivalues)
      {
     if( aivalues != nullptr)
     {
       uint8_t lnull = STRUCT_NOT_NULL;
       push(ap_buffer_data, "", lnull );
       return push(ap_buffer_data, "", *aivalues );
     }
     else
     {
       uint8_t lnull = STRUCT_IS_NULL;
       return push(ap_buffer_data, "", lnull );
     }
        
      }

    template <typename T_DATA, typename T_DATA2>
    static bool push_struct(
      serializecpp_buffer& ap_buffer_data,
      KeyPlaceholder/*apkey占位*/, 
      T_DATA* aivalues,
      T_DATA2& appush)
    {
      if (aivalues != nullptr)
      {
        uint8_t lnull = STRUCT_NOT_NULL;
        push(ap_buffer_data, "", lnull);
        return push_struct(ap_buffer_data, "", *aivalues, appush);
      }
      else
      {
        uint8_t lnull = STRUCT_IS_NULL;
        return push(ap_buffer_data, "", lnull);
      }

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

#endif  //SERIALIZECPP_HPP
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
