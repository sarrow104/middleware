///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SERIALIZECPP_CLASS_H
#define SERIALIZECPP_CLASS_H

#include "middleware/tools/serializecpp/serializecpp.hpp"
#include "middleware/tools/serializecpp/unserializecpp.hpp"

namespace middleware {
  namespace tools {

    /****************************
     **类序列化  需要实现pop push方法
     *****************************/
    class serializecpp_class
    {
      serializecpp_buffer* mp;
      virtual bool pop( /*serializecpp_buffer* ap*/) = 0;
      virtual bool push( /*serializecpp_buffer* ap*/) = 0;
    protected:
      serializecpp_class() :
        mp(nullptr)
      {}
    public:
      bool pop(serializecpp_buffer* ap)
      {
        if (mp != nullptr)
        {
          return false;
        }
        mp = ap;
        bool lret;
        try
        {
          lret = pop();
        }
        catch (...)
        {
          lret = false;
        }

        mp = nullptr;
        return lret;


      }
      bool push(serializecpp_buffer* ap)
      {
        if (mp != nullptr)
        {
          return false;
        }
        mp = ap;
        bool lret;
        try
        {
          lret = push();
        }
        catch (...)
        {
          lret = false;
        }

        mp = nullptr;
        return lret;
      }

      serializecpp_buffer* get_serializecpp_buffer()
      {
        return mp;
      }

      void check(bool airet)
      {
        if (!airet)
        {
          throw "check err..";
        }
      }
    };

  }  //namespace tools
}  //namespace middleware 




#endif //SERIALIZECPP_CLASS_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
