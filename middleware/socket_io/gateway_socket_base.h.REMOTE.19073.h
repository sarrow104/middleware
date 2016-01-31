///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef GATEWAY_SOCKET_BASE_H
#define GATEWAY_SOCKET_BASE_H


#include "socket_head.h"
#include "lpthread.h"
#include "middleware/loop_array/looparray.h"

#include <boost/function.hpp>

#include <algorithm>

namespace middleware {

  /**
   ** socket 基类
   **/
  class gateway_socket_base
  {
    module_communicate m_recvlooparray;
    module_communicate m_sendlooparray;
    boost::function<bool(uint32_t&, SOCKET)> m_socket2key;
    boost::function<bool(uint32_t, SOCKET&)> m_key2socket;
    uint32_t m_ieverymaxsize;
    char* lbuf1; /* 双buffer,loop array在第二次调用send成功前,不能修改第一次的buffer*/
    char* lbuf2; /* 双buffer,loop array在第二次调用send成功前,不能修改第一次的buffer*/

    /*
     *  发送回调
     */
    bool send_callback(const char* ap, uint32_t aplen)
    {
      /* 获取key */
      uint32_t lkey = *((uint32_t*)(ap));
      SOCKET lsocket;
      if (m_key2socket(lkey, lsocket))
      {
        if (g_send(lsocket, ap + sizeof(uint32_t), aplen - sizeof(uint32_t)) <= 0)
        {
          /* 发送失败 */
          sendfailure(lsocket, ap, aplen);
        }
      }
      else
      {
        if (lsocket != 0)
        {
          sendfailure(lsocket, ap, aplen);
        }
      }
      return true;
    }

  public:
    gateway_socket_base(
      /* loop array相关 */
      uint32_t apbuffersize, uint32_t aieverymaxsize, FUN_READ_CALLBACK aireadfun,/* bool apstartthread,*/
      boost::function<bool(uint32_t, SOCKET&)> aikey2socket /* send回调方法从子类获取socket句柄 */
      ) :
      m_recvlooparray(apbuffersize, aieverymaxsize, aireadfun, false),
      m_sendlooparray(apbuffersize, aieverymaxsize, boost::bind(&gateway_socket_base::send_callback, this, _1, _2), false),
      m_ieverymaxsize(aieverymaxsize),
      m_key2socket(aikey2socket),
      lbuf1(new char[m_ieverymaxsize]),
      lbuf2(new char[m_ieverymaxsize])
    {}

    /*
     *  发送消息
     */
    bool send(uint32_t aikey, const char* ap, uint32_t aplen)
    {
      static uint32_t lmaxsize = m_ieverymaxsize - sizeof(uint32_t);
      if (aplen > lmaxsize)
      {
        return false;
      }

      std::swap(lbuf1, lbuf2);
      memcpy(lbuf1, &aikey, aplen);
      memcpy(&(lbuf1[sizeof(uint32_t)]), ap, aplen);
      m_sendlooparray.send(lbuf1, aplen + sizeof(uint32_t));
      return true;
    }


    /*
     *  接收回调
     */
    bool recv(SOCKET sisocket)
    {
      char* lbuf1 = new char[m_ieverymaxsize];
      char* lbuf2 = new char[m_ieverymaxsize];
      uint32_t llen = 0;
      while (1)
      {
        std::swap(lbuf1, lbuf2);
        llen = g_recv(sisocket, lbuf1, m_ieverymaxsize);
        if (llen <= 0)
        {
          /* socket err */
          return false;
        }
        m_recvlooparray.send(lbuf1, llen);
      }
      return true;
    }

    virtual bool sendfailure(SOCKET aisocket, const char* ap, uint32_t aplen) = 0;
  };


} //namespace middleware











#endif
