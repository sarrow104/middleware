///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef GATEWAY_SHARED_MEMORY_SERVER_H
#define GATEWAY_SHARED_MEMORY_SERVER_H

#include "shared_memory_base.h"

namespace middleware {

  /**
   **  共享内存服务器
   **/
  class shared_memory_server :
    public shared_memory_base
  {
	  /*
	   *  初始化
	   */
	  void init(void* apdress, uint64_t ai_smbyte, const char* ainame, bool aicreate_sem)
	  {
		  if (ai_smbyte <= 0)
		  {
			  throw 0;
		  }
		  m_head = static_cast<SHARED_HEAD*>(apdress);

		  char* lbeg = &(char_ptr(apdress)[sizeof(SHARED_HEAD)]);
		  m_head->m_create_w = lbeg;
		  m_head->m_create_r = lbeg;
		  m_head->m_create_beg = lbeg;
		  m_head->m_create_end = lbeg + ai_smbyte - sizeof(SHARED_HEAD);
		  m_head->m_smbyte = ai_smbyte;
		  m_head->m_wcount = 0;
		  m_head->m_rcount = 0;

		  try
		  {
			  m_wsem = new named_semaphore(open_only, (std::string(ainame) + "_sem_w").c_str());
			  m_rsem = new named_semaphore(open_only, (std::string(ainame) + "_sem_r").c_str());
		  }
		  catch (...)
		  {
			  named_semaphore::remove((std::string(ainame) + "_sem_w").c_str());
			  named_semaphore::remove((std::string(ainame) + "_sem_r").c_str());
			  m_wsem = new named_semaphore(create_only, (std::string(ainame) + "_sem_w").c_str(), 0);
			  m_rsem = new named_semaphore(create_only, (std::string(ainame) + "_sem_r").c_str(), 0);
		  }
	  }
  public:
    shared_memory_server(void* apdress, uint64_t ai_smbyte, uint32_t aieveryonemaxsize, const char* ainame, bool aicreate_sem = false) :
      shared_memory_base(aieveryonemaxsize + EVERY_ONE_SIZE_SIZE/*MAX_EVERY_ONE_SIZE*/)
    {
      init(apdress, ai_smbyte, ainame, aicreate_sem);
    }

    char* wget_start()
    {
      while (true)
      {
        if (char_ptr(m_head->m_create_w) + m_every_one_maxsize >= m_head->m_create_end)
        {
          if (m_head->m_create_w > m_head->m_create_r)
          {
            m_head->m_create_w = m_head->m_create_beg;
            m_head->m_open_w = m_head->m_open_beg;
            break;
          }
          else
          {
            m_wsem->wait();
          }
        }
        else
        {
          break;
        }
      }

      while (true)
      {
        if (m_head->m_create_w < m_head->m_create_r)
        {
          if ((uint32_t)(char_ptr(m_head->m_create_r) - char_ptr(m_head->m_create_w)) >= m_every_one_maxsize/*MAX_EVERY_ONE_SIZE*/)
          {
            break;
          }
          else
          {
            //未有空闲资源
            m_wsem->wait();
          }
        }
        else if (m_head->m_create_w == m_head->m_create_r && m_head->m_wcount > m_head->m_rcount)
        {
          m_wsem->wait();
        }
        else
        {
          break;
        }
      }

      return &((static_cast<char*>(m_head->m_create_w))[EVERY_ONE_SIZE_SIZE]);
    }

    void wget_finish(uint32_t aiwlen)
    {
      m_wlen = aiwlen + EVERY_ONE_SIZE_SIZE;
      *(static_cast<short*>(m_head->m_create_w)) = aiwlen;

      set_create_wptr();
      ++m_head->m_wcount;
      m_rsem->post();
    }

  };

} //namespace middleware

#endif //GATEWAY_SHARED_MEMORY_SERVER_H
