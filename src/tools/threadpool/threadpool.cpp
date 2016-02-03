///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/threadpool/threadpool.hpp"

namespace middleware {
  namespace tools{

    std::vector<workthread*> threadpool::m_thread_list;
    boost::mutex threadpool::m_lock;
    uint32_t threadpool::m_threadsize = 0;
    uint32_t threadpool::m_max_thread = DEFAULT_MAX_THREAD;
    boost_sem threadpool::m_sem(0);

    int workthread::run()
    {
      while( true )
      {
        m_sem.wait();
        m_fun();
        threadpool::push( this );
      }
    }

  } //namespace tools 
} //namespace middleware

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
