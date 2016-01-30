///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/loop_array/looparray.h"

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>

#include <cstdint>
#include <iostream>

typedef boost::interprocess::interprocess_semaphore boost_sem;

namespace middleware {

  /**
   ** 循环数组基础
   **/
  class loop_array
  {
    size_t m_array_size;            /* 数组容量 */
    char* m_array_beg_ptr;          /* 数组开始指针 */
    char* m_array_end_ptr;          /* 数组结束指针 */
    char* m_write;                  /* 写指针位置 */
    char* m_read;                    /* 读指针位置 */
    boost_sem* m_write_sem;          /* 用来通知处理 */
    boost_sem* m_read_sem;          /* 用来通知接收 */

    uint16_t m_temp_len;            /* 临时缓冲区有效数据长度 */
    char* m_temp_arr;                /* 临时缓冲区 */
    boost::mutex  m_lock;            /* 用于保证 在读中m_write,m_write_bool的拷贝 与 写中m_write,m_write_bool修改 互斥  同理还有 m_read  m_read_bool */
    boost::function<bool(const char*, uint32_t)>  m_read_fun;  /* 读回调 */
    boost::function<bool(char*, uint32_t&)>  m_write_fun;      /* 写回调 */

    uint32_t m_rcount;   /* 读的次数 */
    uint32_t m_wcount;   /* 写的次数 */

    bool m_malloc;

    /*与close相关*/
    bool m_close;
    bool m_write_close;
    bool m_read_close;

  public:
    ~loop_array()
    {
      delete[] m_temp_arr;
      m_temp_arr = NULL;

      if (m_malloc)
      {
        delete[] m_array_beg_ptr;
        m_array_beg_ptr = NULL;
      }
    }

    loop_array(
      char* aibuffer, size_t size,
      uint32_t aieverymaxsize, boost::function<bool(const char*, uint32_t)> areadfun,
      boost::function<bool(char*, uint32_t&)> awritefun
      ) :
      m_write_sem(new boost_sem(0)),
      m_read_sem(new boost_sem(0)),
      m_array_size(size),
      m_temp_len(0),
      m_read_fun(areadfun),
      m_write_fun(awritefun),
      m_temp_arr(new char[aieverymaxsize]),
      m_malloc(false),
      m_rcount(0),
      m_wcount(0),
      m_close(false),
      m_write_close(false),
      m_read_close(false)
    {
      m_array_beg_ptr = aibuffer;
      m_array_end_ptr = (m_array_beg_ptr + size);
      m_write = (m_array_beg_ptr);
      m_read = (m_array_beg_ptr);
    }


    loop_array(
      size_t size, uint32_t aieverymaxsize,
      boost::function<bool(const char*, uint32_t)> areadfun,
      boost::function<bool(char*, uint32_t&)> awritefun
      ) :
      m_write_sem(new boost_sem(0)),
      m_read_sem(new boost_sem(0)),
      m_array_size(size),
      m_temp_len(0),
      m_read_fun(areadfun),
      m_write_fun(awritefun),
      m_temp_arr(new char[aieverymaxsize]),
      m_malloc(true),
      m_rcount(0),
      m_wcount(0),
      m_close(false),
      m_write_close(false),
      m_read_close(false)
    {
      m_array_beg_ptr = new char[size];
      m_array_end_ptr = (m_array_beg_ptr + size);
      m_write = (m_array_beg_ptr);
      m_read = (m_array_beg_ptr);
    }

    loop_array* start_run(bool aibool)
    {
      if (m_write_sem != NULL)
      {
        delete m_write_sem;
      }

      if (m_read_sem != NULL)
      {
        delete m_read_sem;
      }
      m_write_sem = (new boost_sem(0));
      m_read_sem = (new boost_sem(0));
      m_temp_len = (0);
      m_rcount = (0);
      m_wcount = (0);
      m_write = (m_array_beg_ptr);
      m_read = (m_array_beg_ptr);

      boost::thread(boost::bind(&loop_array::write_run, this));
      if (aibool)
      {
        read_run();
      }
      else
      {
        boost::thread(boost::bind(&loop_array::read_run, this));
      }
      return this;
    }

    char* write_start(uint32_t& ailen)
    {
      char* ltemp;
      uint32_t ltempcount;
      while (1)
      {
        {/* 锁区间 */
          boost::mutex::scoped_lock llock(m_lock);
          ltemp = m_read;
          ltempcount = m_rcount;

        }/* 锁区间 */
        if (m_write > ltemp)
        {
          ailen = m_array_end_ptr - m_write;
          break;
        }
        else if (m_write < ltemp)
        {
          ailen = ltemp - m_write;
          break;
        }
        else // ==
        {
          if (m_wcount > ltempcount)
          {
            m_write_sem->wait();
            continue;
          }
          else
          {
            ailen = m_array_end_ptr - m_write;
            break;
          }
        }
      }

      return m_write;
    }

    void write_finish(uint32_t ailen)
    {
      {/* 锁区间 */
        boost::mutex::scoped_lock llock(m_lock);

        m_write += ailen;
        if (m_write >= m_array_end_ptr)
        {
          ++m_wcount;
          m_write = m_array_beg_ptr;
        }
      }/* 锁区间 */
      m_read_sem->post();
    }

    void write_run()
    {
      uint32_t llen;
      char* p;
      while (1)
      {
        if (m_close)/*是否被关闭*/
        {
          m_write_close = true;
          return;
        }
        p = write_start(llen);
        if (m_write_fun(p, llen))
        {
          write_finish(llen);
        }
        else
        {
          break;
        }
      }
    }

    void read_run()
    {

      char* ltemp_write;
      char* ltemp_read = m_read;
      uint32_t lbodylen;                  /* 剩余body长度 */
      char lclen[sizeof(uint16_t)] = { 0 };    /* 出现数据不足sizeof( uint16_t )时 暂存数据 */
      bool lcbool = false;                /* lclen中是否有数据 */
      size_t ailen;                      /* 可用长度 */
      bool bodyrecvover = true;            /* 是否需要获取头部的size*/
      uint32_t temp_size = 0;              /*记录前面长度有几字节  一般两字节  也会出现一字节 */

      uint32_t lbodylen_copy;
      uint32_t ltempwcount;
      uint32_t ltemprcount = m_rcount;


      bool is_same = true;          /* 读副本与真正的读指针同步频率  */
      while (1)
      {
        if (m_close)/* 是否被关闭 */
        {
          m_read_close = true;
          return;
        }

        {/* 锁区间 */
          boost::mutex::scoped_lock llock(m_lock);

          //重置m_read
          if (ltemp_read >= m_array_end_ptr)
          {
            ltemp_read = m_array_beg_ptr;
            ++ltemprcount;
          }

          if (temp_size == 0 && is_same)
          {
            is_same = false;
            m_read = ltemp_read;
            m_rcount = ltemprcount;
          }
          //拷贝临时 write ptr 和 写状态
          ltemp_write = m_write;
          ltempwcount = m_wcount;
        }/* 锁区间 */


        /* 获取此刻可用的数据长度 */
        if (ltemp_read > ltemp_write)
        {
          ailen = m_array_end_ptr - ltemp_read;
        }
        else if (ltemp_read < ltemp_write)
        {
          ailen = ltemp_write - ltemp_read;
        }
        else//==
        {
          if (ltemprcount < ltempwcount)
          {
            ailen = m_array_end_ptr - ltemp_read;
          }
          else
          {
            m_read_sem->wait();
            continue;
          }
        }

        if (bodyrecvover)
        {
          /* 获取body len uint16_t */
          if (lcbool)
          {
            lclen[1] = ltemp_read[0];
            lbodylen = *((uint16_t*)(lclen));
            lbodylen_copy = lbodylen;
            temp_size = 1;
            ltemp_read += 1;
          }
          else
          {
            if (ailen < sizeof(uint16_t)) //ailen == 1
            {
              if (ltemp_read + ailen == m_array_end_ptr)
              {
                lclen[0] = ltemp_read[0];
                lcbool = true;
                ltemp_read += ailen;
              }
              continue;
            }
            else
            {
              lbodylen = *((uint16_t*)(ltemp_read));
              lbodylen_copy = lbodylen;
              ltemp_read += 2;
              temp_size = 2;
            }
          }
        }

        /* 获取body */
        if (lbodylen + temp_size > ailen)
        {
          bodyrecvover = false;
          if (m_temp_len == 0)
          {
            temp_size = ailen - temp_size;
            memcpy(m_temp_arr, ltemp_read, temp_size);
            m_temp_len = temp_size;
            lbodylen -= temp_size;
            ltemp_read += temp_size;
          }
          else
          {
            memcpy(&(m_temp_arr[m_temp_len]), ltemp_read, ailen);
            m_temp_len += ailen;
            lbodylen -= ailen;
            ltemp_read += ailen;
          }
          temp_size = 0;
        }
        else
        {
          if (m_temp_len != 0)
          {
            memcpy(&(m_temp_arr[m_temp_len]), ltemp_read, lbodylen);
            /* ret do sth*/
            if (!m_read_fun(m_temp_arr, lbodylen_copy))
            {
              return;
            }
            ltemp_read += lbodylen;
          }
          else
          {
            //ret = ltemp_read;
            if (!m_read_fun(ltemp_read, lbodylen_copy))
            {
              return;
            }
            ltemp_read += lbodylen_copy;
          }
          is_same = true;
          bodyrecvover = true;
          lcbool = false;
          lbodylen = 0;
          m_temp_len = 0;
          temp_size = 0;
          m_write_sem->post();
        }
      }
    }

    void close()
    {
      m_close = false;
      bool& lwrite_close = m_write_close;
      bool& lread_close = m_read_close;
      loop_array* lthis = this;
      auto lfun = [&lwrite_close, &lread_close, &lthis]() {
        while (!(lwrite_close&&lread_close))
        {
          boost::this_thread::sleep(boost::posix_time::milliseconds(200));
        }
        delete lthis;
      };

      boost::thread lt(lfun);
    }


  };





  loop_array* new_loop_array(uint32_t apbuffersize, uint32_t aieverymaxsize, FUN_READ_CALLBACK aireadfun, FUN_WRITE_CALLBACK aiwritefun)
  {
    return (new loop_array(apbuffersize, aieverymaxsize, aireadfun, aiwritefun));
  }



  loop_array* new_loop_array(char* aibuffer, uint32_t apbuffersize, uint32_t aieverymaxsize, FUN_READ_CALLBACK aireadfun, FUN_WRITE_CALLBACK aiwritefun)
  {
    return (new loop_array(aibuffer, apbuffersize, aieverymaxsize, aireadfun, aiwritefun));
  }



  void start_run(loop_array* ap, bool aibool)
  {
    if (aibool)
    {
      delete ap->start_run(aibool);
    }
    else
    {
      ap->start_run(aibool);
    }
  }



  void delete_loop_array(loop_array* ap)
  {
    if (ap != NULL)
    {
      ap->close();
    }
  }

}