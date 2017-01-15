///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/socket_asio/close_socket_session.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_session_base.hpp"

#include <unordered_set>
//#include "socket_asio_session_keep.h"

namespace middleware {

  void close_socket_session::close_run()
  {
    std::vector< data_pair > m_temp_ls;
    std::vector< data_pair >::iterator itor;
    while (1)
    {
      m_sem.wait();
      m_lock.lock();
      if (m_ls.empty())
      {
        m_lock.unlock();
        continue;
      }
      else
      {
        m_temp_ls.swap(m_ls);
        m_lock.unlock();
      }

      itor = m_temp_ls.begin();

      while (itor != m_temp_ls.end())
      {
        (*itor).m_ptr->close((*itor).closesocketcount, (*itor).m_send_protocol);
        ++itor;
      }

      m_temp_ls.clear();
    }
  }

} //namespace middleware 

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */

