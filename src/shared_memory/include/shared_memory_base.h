#ifndef GATEWAY_SHARED_MEMORY_BASE_H
#define GATEWAY_SHARED_MEMORY_BASE_H


#include <interprocess/sync/named_semaphore.hpp>
#include <function.hpp>
#include <thread.hpp>
#include "shared_memory.h"


using boost::interprocess::named_semaphore;
using boost::interprocess::create_only_t;
using boost::interprocess::open_only_t;
using boost::interprocess::open_or_create;


namespace middleware {

#define EVERY_ONE_SIZE_SIZE    ( sizeof(uint16_t) )                  /* 每条消息包装时加上的SIZE所占的空间 */

  /* 共享内存头 */
  struct SHARED_HEAD
  {
    uint64_t m_smbyte;          /* 共享内存的字节数 */

    void* m_create_w;          /* 写指针 */
    void* m_create_r;          /* 读指针 */
    void* m_create_beg;          /* 首地址 */
    void* m_create_end;          /* 末地址 */

    void* m_open_w;            /* 写指针 */
    void* m_open_r;            /* 读指针 */
    void* m_open_beg;          /* 首地址 */
    void* m_open_end;          /* 末地址 */

    uint32_t m_wcount;
    uint32_t m_rcount;

  };

  /**
   **  共享内存基类
   **/
  class shared_memory_base
  {

  protected:
    named_semaphore*  m_wsem;
    named_semaphore*  m_rsem;
    SHARED_HEAD* m_head;
    uint16_t m_wlen;
    uint16_t m_rlen;
    uint32_t m_every_one_maxsize;

    shared_memory_base(uint32_t aieverymaxsize) :
      m_every_one_maxsize(aieverymaxsize)
    {

    }

#if 0
    ~shared_memory_base()
    {

      delete m_wsem;
      m_wsem = NULL;
      delete m_rsem;
      m_rsem = NULL;
    }
#endif

    void set_every_one_maxsize(uint32_t aieverymaxsize)
    {
      m_every_one_maxsize = aieverymaxsize;
    }

    char* char_ptr(void* aiptr)
    {
      return static_cast<char*>(aiptr);
    }

    void set_create_wptr()
    {
      m_head->m_create_w = char_ptr(m_head->m_create_w) + m_wlen;
      m_head->m_open_w = char_ptr(m_head->m_open_w) + m_wlen;
    }


    void set_open_rptr()
    {
      m_head->m_open_r = char_ptr(m_head->m_open_r) + m_rlen;
      m_head->m_create_r = char_ptr(m_head->m_create_r) + m_rlen;
    }


    uint64_t max_byte()
    {
      return m_head->m_smbyte;
    }


    /* 以下函数可以用来测试  */

    //void print()
    //{
    //  std::cout<< "[Create Message ]"  << std::endl;
    //  std::cout<< "共享内存的字节数[ " << m_head->m_smbyte          << " ]"  << std::endl;
    //  std::cout<< "写指针所在位置[ "   << (uint32_t)m_head->m_create_w    << " ]" << std::endl;
    //  std::cout<< "读指针所在位置[ "   << (uint32_t)m_head->m_create_r    << " ]" << std::endl;
    //  std::cout<< "首地址[ "       << (uint32_t)m_head->m_create_beg    << " ]" << std::endl;
    //  std::cout<< "末地址[ "       << (uint32_t)m_head->m_create_end    << " ]" << std::endl;
    //  std::cout<< "[Worker Message ]"  << std::endl;
    //  std::cout<< "写指针所在位置[ "   << (uint32_t)m_head->m_open_w      << " ]" << std::endl;
    //  std::cout<< "读指针所在位置[ "   << (uint32_t)m_head->m_open_r      << " ]" << std::endl;
    //  std::cout<< "首地址[ "       << (uint32_t)m_head->m_open_beg    << " ]" << std::endl;
    //  std::cout<< "末地址[ "       << (uint32_t)m_head->m_open_end    << " ]" << std::endl;
    //  std::cout<< "[ "       << (uint32_t)m_head->m_create_beg - (uint32_t)m_head->m_create_end    << " ]" << std::endl;
    //  std::cout<< "[ "       << (uint32_t)m_head->m_open_beg - (uint32_t)m_head->m_open_end    << " ]" << std::endl;
    //}


  };


} // namespace middleware







#endif