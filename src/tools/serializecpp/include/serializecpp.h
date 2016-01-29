#ifndef   MSG_BASE_C_H
#define   MSG_BASE_C_H
#include "serializecpp_buffer.h"

namespace middleware{
  namespace tools{

    /*********************
     **序列化基础类型
     *********************/
    class serializecpp_base
    {
    private:
      static void push(void* aptarget, void* apsource, uint32_t aicopysize)
      {
        memcpy(aptarget, apsource, aicopysize);
      }

      /*
       *  关联容器中的set类 
       */
      template <typename T_DATA>
      static uint32_t push_set(char* ap, uint32_t aplen, T_DATA& aivaluesarr)
      {

        uint16_t lsetsize = aivaluesarr.size();
        uint32_t lsize = sizeof(T_DATA::value_type) * aivaluesarr.size() + sizeof(uint16_t);
        if (aplen < lsize)
        {
          return 0;
        }

        /* 数组长度 */
        serializecpp_base::push(ap, &lsetsize, sizeof(uint16_t));
        ap += sizeof(uint16_t);

        for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
        {
          serializecpp_base::push(ap, (void*)&(*itor), sizeof(T_DATA::value_type));
          ap += sizeof(T_DATA::value_type);
          aplen -= sizeof(T_DATA::value_type);
        }
        return lsize;
      }

      /*
       *  关联容器中的map类
       */
      template <typename T_DATA>
      static uint32_t push_map(char* ap, uint32_t aplen, T_DATA& aivaluesarr)
      {

        uint16_t lsetsize = aivaluesarr.size();
        uint32_t lsize = sizeof(T_DATA::value_type::first_type) * aivaluesarr.size() + sizeof(T_DATA::value_type::second_type) * aivaluesarr.size() + sizeof(uint16_t);
        if (aplen < lsize)
        {
          return 0;
        }

        /* 数组长度 */
        serializecpp_base::push(ap, &lsetsize, sizeof(uint16_t));
        ap += sizeof(uint16_t);

        uint32_t ltemp = sizeof(T_DATA::value_type::first_type) + sizeof(T_DATA::value_type::second_type);
        for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
        {
          serializecpp_base::push(ap, (void*)&(itor->first), sizeof(T_DATA::value_type::first_type));
          ap += sizeof(T_DATA::value_type::first_type);
          aplen -= sizeof(T_DATA::value_type::first_type);
          serializecpp_base::push(ap, (void*)&(itor->second), sizeof(T_DATA::value_type::second_type));
          ap += sizeof(T_DATA::value_type::second_type);
          aplen -= sizeof(T_DATA::value_type::second_type);
        }
        return lsize;
      }

    public:
      /*
       * 内置类型
       * int
       * float
       * double
       * ....
       * 和整个没有指针成员的struct结构
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

      /*
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
        /* 数组长度 */
        serializecpp_base::push(ap, &aivaluesarrsize, sizeof(uint16_t));
        ap += sizeof(uint16_t);
        serializecpp_base::push(ap, (void*)(aivaluesarr), sizeof(T_DATA) * aivaluesarrsize);
        return lsize;
      }

      /*
       * vector数组
       */
      template <typename T_DATA>
      static uint32_t push(char* ap, uint32_t aplen, std::vector<T_DATA>& aivaluesarr)
      {
        return serializecpp_base::push(ap, aplen, aivaluesarr.data(), aivaluesarr.size());
      }

      /*
       * std::string
       */
      static uint32_t push(char* ap, uint32_t aplen, std::string& aivaluesarr)
      {
        return serializecpp_base::push(ap, aplen, aivaluesarr.c_str(), aivaluesarr.size());
      }

/* 宏定义为了方便添加 set map 类的关联容器 */
#define DEF_PUSH_SET_TYPE( TYPE )    \
  template <typename T_DATA>\
  static uint32_t push(char* ap, uint32_t aplen, TYPE<T_DATA>& aivaluesarr)\
  {\
    return push_set( ap, aplen, aivaluesarr);\
  }

#define DEF_PUSH_MAP_TYPE( TYPE )  \
  template <typename T_DATA1,typename T_DATA2>\
  static uint32_t push(char* ap, uint32_t aplen, TYPE<T_DATA1,T_DATA2>& aivaluesarr)\
  {\
    return push_map( ap, aplen, aivaluesarr );\
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

    /****************
     *序列化类
     ****************/
    class serializecpp
    {
      /* 返回可用空间 */
      static uint32_t get_have_len(serializecpp_buffer* mp_buffer_data)
      {
        return mp_buffer_data->get_len() - mp_buffer_data->get_uselen();
      }
    public:
      //基础
      template <typename T_DATA>
      static bool push(serializecpp_buffer* mp_buffer_data, T_DATA& aivalues)
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
} //namespace middleware


#endif
