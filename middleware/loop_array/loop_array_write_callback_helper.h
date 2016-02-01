///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef LOOP_ARRAY_WRITE_CALLBACK_HELPER_H
#define LOOP_ARRAY_WRITE_CALLBACK_HELPER_H

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>

typedef boost::interprocess::interprocess_semaphore boost_sem;

/** #define _DLL_SAMPLE */
#ifdef _MSC_VER
#   define _DLL_SAMPLE
#   ifdef _DLL_SAMPLE
#       define DLL_SAMPLE_API __declspec(dllexport)
#   else
#       define DLL_SAMPLE_API __declspec(dllimport)
#   endif //_DLL_SAMPLE
#else
#   define DLL_SAMPLE_API
#endif  //_MSC_VER

namespace middleware {

  class loop_array;

  /** 写回调 */
  typedef boost::function<bool(char*, uint32_t&)> FUN_WRITE_CALLBACK;

  /** 读回调 */
  typedef boost::function<bool(const char*, uint32_t)>   FUN_READ_CALLBACK;

  /**
   *  创建循环数组
   *  @param 缓冲区字节数
   *  @param 发送单条数据最大字节
   *  @param 写回调
   *  @param 读回调
   */
  DLL_SAMPLE_API loop_array* new_loop_array(uint32_t apbuffersize, uint32_t aieverymaxsize, FUN_READ_CALLBACK aireadfun, FUN_WRITE_CALLBACK aiwritefun);


  /**
   *  创建循环数组
   *  @param 已分配的缓冲区
   *  @param 缓冲区字节数
   *  @param 发送单条数据最大字节
   *  @param 写回调
   *  @param 读回调
   */
  DLL_SAMPLE_API loop_array* new_loop_array(char* aibuffer, uint32_t apbuffersize, uint32_t aieverymaxsize, FUN_READ_CALLBACK aireadfun, FUN_WRITE_CALLBACK aiwritefun);

  /**
   *  开启循环数组
   */
  DLL_SAMPLE_API void start_run(loop_array*, bool aibool);

  /**
   *  销毁循环数组
   */
  DLL_SAMPLE_API void delete_loop_array(loop_array* ap);


  DLL_SAMPLE_API char* loop_write_start(uint32_t& ailen);

  DLL_SAMPLE_API void write_finish(uint32_t ailen);



  /**
   *  辅助循环数组 写 回调
   *  用于保存根据名称生成的两个单向的循环数组
   */
  class loop_array_write_callback_helper
  {
    /* send_run_fun 函数私有变量*/
    bool write_once____the_first;
    uint32_t write_once____ltemp;
    uint32_t write_once____ltemp2;
    uint16_t write_once____lsize;
    bool write_once____is_push_head_size;

    boost_sem m_ready_sem1;
    boost_sem m_ready_sem2;
    char* sendbuffer;
    uint16_t sendbuffersize;
    boost::mutex  m_mutex;

  public:
    loop_array_write_callback_helper() :
      m_ready_sem1(0),
      m_ready_sem2(0),
      sendbuffer(NULL),
      sendbuffersize(0),
      write_once____the_first(true)
    {}


    bool write_once(char* ap, uint32_t& aplen)
    {
      while (sendbuffer == NULL || sendbuffersize == 0)
      {
        m_ready_sem1.wait();
      }
      if (write_once____the_first)
      {
        write_once____ltemp = sizeof(uint16_t);
        write_once____ltemp2 = 0;
        write_once____lsize = sendbuffersize;
        write_once____is_push_head_size = true;
        write_once____the_first = false;
      }
      if (write_once____is_push_head_size)
      {
        if (aplen < write_once____ltemp)
        {
          /** 未写完size */
          ap[0] = ((char*)(&write_once____lsize))[write_once____ltemp2];
          ++write_once____ltemp2;
          if (write_once____ltemp == sizeof(uint16_t))
          {
            --write_once____ltemp;
            return true;
          }
          else
          {
            write_once____ltemp = 1;
            write_once____is_push_head_size = false;
            ap += write_once____ltemp;
          }

        }
        else
        {
          if (write_once____ltemp == 1)
          {
            ap[0] = ((char*)(&write_once____lsize))[1];
            write_once____ltemp = 1;
          }
          else
          {
            *((uint16_t*)(ap)) = write_once____lsize;
            write_once____ltemp = 2;
          }
          write_once____is_push_head_size = false;
          ap += write_once____ltemp;
        }
      }
      if (sendbuffersize > (aplen - write_once____ltemp))
      {
        memcpy(ap, (sendbuffer), (aplen - write_once____ltemp));
        /* 重置 */
        sendbuffersize -= (aplen - write_once____ltemp);
        sendbuffer += (aplen - write_once____ltemp);
        write_once____ltemp = 0;
      }
      else
      {
        memcpy(ap, sendbuffer, sendbuffersize);
        aplen = sendbuffersize + write_once____ltemp;
        write_once____the_first = true;
        sendbuffer = NULL;
        sendbuffersize = 0;
        m_ready_sem2.post();
      }
      return true;
    }

    /*
     *  发送数组
     *  传入的指针是不能再第二次成功调用前修改其数据的,
     *  因为他只不过是保存了指针,真正的拷贝是在线程里进行的
     *  可被多线程调用
     */
    bool push_write_once(char* ap, uint32_t aplen)
    {
      boost::mutex::scoped_lock llock(m_mutex);
      while (1)
      {
        if (sendbuffer == NULL && sendbuffersize == 0)
        {
          sendbuffer = ap;
          sendbuffersize = aplen;
          m_ready_sem1.post();
          return true;
        }
        else
        {
          m_ready_sem2.wait();
        }
      }
    }
  };

} //namespace middleware

#endif //LOOP_ARRAY_WRITE_CALLBACK_HELPER_H
