///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef LOOP_ARRAY_H
#define LOOP_ARRAY_H

#include "middleware/middleware_base/loop_array/loop_array_write_callback_helper.hpp"

namespace middleware{

  /**
   *  单向循环数组
   */
  class module_communicate
  {
    bool            m_startthread;    /** 是否开启线程支持 */
    loop_array*      m_la_handle;
    uint32_t        m_buffer_size;
    uint32_t        m_every_once_max_size;
    FUN_READ_CALLBACK  m_readfun;
    loop_array_write_callback_helper m_callback_helper;

    bool send_run(char* ap, uint32_t& aplen)
    {
      return m_callback_helper.write_once(ap, aplen);
    }
  public:

    module_communicate(uint32_t apbuffersize, uint32_t aieverymaxsize, FUN_READ_CALLBACK aireadfun,/* FUN_WRITE_CALLBACK aiwritefun,*/ bool apstartthread) :
      m_buffer_size(apbuffersize),
      m_every_once_max_size(aieverymaxsize),
      m_readfun(aireadfun),
      m_startthread(apstartthread)
    {
      m_la_handle = new_loop_array(
        m_buffer_size,
        m_every_once_max_size,
        aireadfun,
        boost::bind(&module_communicate::send_run, this, _1, _2)
        );
      start_run(m_la_handle, m_startthread);
    }

    /**
     *  发送字符串
     */
    bool send(char* ap, uint32_t aplen)
    {
      return m_callback_helper.push_write_once(ap, aplen);
    }

    /**
     *  关闭通信组件
     */
    bool close()
    {
      delete_loop_array(m_la_handle);
      return true;
    }
  };

} //namespace middleware
#endif //LOOP_ARRAY_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
