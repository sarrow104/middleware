///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SERIALIZECPP_BUFFER_H
#define SERIALIZECPP_BUFFER_H

#include <cstdint>
#include <ostream>
#include <vector>
#include <memory>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>



/******************* 
 **serializecpp buffer 
 ********************/
namespace middleware {
  namespace tools{

    struct serializecpp_buffer
    {
      //获取已使用长度
      inline uint32_t& get_uselen()
      {
        return mi_buffer_at;
      }
      //获取总空间大小
      inline uint32_t& get_len()
      {
        return mi_buffer_size;
      }

      /*
      aibuf 自定义空间buffer
      aisize 自定义空间buffer 大小
      */
      serializecpp_buffer(char* aibuf, uint32_t aisize) :
        mc_buffer(aibuf),
        mi_buffer_size(aisize),
        mi_buffer_at(0),
        mb_malloc(false)
      {}

      /*
      aisize buffer 大小
      */
      serializecpp_buffer(uint32_t aisize) :
        mc_buffer(new char[aisize]),
        mi_buffer_size(aisize),
        mi_buffer_at(0),
        mb_malloc(true)
      {}

      ~serializecpp_buffer()
      {
        if (mb_malloc)
        {
          delete[] mc_buffer;
        }
      }

      inline char* get_buffer()
      {
        return mc_buffer;
      }

      inline char* get_nowpos_buffer()
      {
        return &(mc_buffer[mi_buffer_at]);
      }

      inline void reset()
      {
        mi_buffer_at = 0;
      }

      //检查空间是否足够
      inline bool check(size_t aisize)
      {
        return (mi_buffer_size < aisize) ? false : true;
      }
    private:
      char* mc_buffer;
      uint32_t mi_buffer_size;
      uint32_t mi_buffer_at;
      bool mb_malloc;
    };

  }  //namespace tools
} //namespace middleware

#endif //SERIALIZECPP_BUFFER_H
