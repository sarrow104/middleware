///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef GATEWAY_SHARED_MEMORY_CLIENT_H
#define GATEWAY_SHARED_MEMORY_CLIENT_H

#include "shared_memory_base.h"

namespace middleware {

  /**
   **  共享内存客户端
   **/
  class shared_memory_client :
    public shared_memory_base
  {
    shared_memory_client();
    shared_memory_client(const shared_memory_client&);
    bool m_start;/*只有调用过start 才能调用finish*/
    boost::function<bool(char*, uint32_t&)> m_fun;

	/*
	*  初始化
	*/
	void init(void* apdress, uint64_t ai_smbyte, const char* ainame)
	{
		m_head = static_cast<SHARED_HEAD*>(apdress);
		char* lbeg = &(char_ptr(apdress)[sizeof(SHARED_HEAD)]);
		int wpos = char_ptr(m_head->m_create_w) - char_ptr(m_head->m_create_beg);
		m_head->m_open_w = lbeg + wpos;
		m_head->m_open_r = lbeg;
		m_head->m_open_beg = lbeg;
		m_head->m_open_end = lbeg + ai_smbyte - sizeof(SHARED_HEAD);
		//m_is_create = false;
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
    shared_memory_client(void* apdress, uint64_t ai_smbyte, uint32_t aieveryonemaxsize, const char* ainame) :
      m_start(false),
      shared_memory_base(aieveryonemaxsize + EVERY_ONE_SIZE_SIZE/*MAX_EVERY_ONE_SIZE*/)
    {
      init(apdress, ai_smbyte, ainame);
    }
	
	
	/*
	 *  设置回调
	 */
	void set_callback(boost::function<bool(char*, uint32_t&)> aifun)
	{
		m_fun = aifun;
		boost::thread(boost::bind(&shared_memory_client::rget, this));
	}

    /*
	 *  单线程 获取
	 */
    char* rget_strat(uint32_t& ailen)
    {
      if (char_ptr(m_head->m_open_r) + m_every_one_maxsize >= m_head->m_open_end)
      {
        m_head->m_open_r = m_head->m_open_beg;
        m_head->m_create_r = m_head->m_create_beg;
      }
      while (true)
      {
        while (m_head->m_wcount == m_head->m_rcount)
        {
          m_rsem->wait();
        }
        m_start = true;
        /* 获取大小 */
        m_rlen = *(static_cast<uint16_t*>(m_head->m_open_r));
        ailen = m_rlen;
        /* 返回body */
        return &((static_cast<char*>(m_head->m_open_r))[EVERY_ONE_SIZE_SIZE]);
      }
    }

    void rget_finish()
    {
      if (!m_start)
      {
        throw 0;
      }
      else
      {
        m_start = false;
      }
      m_rlen += EVERY_ONE_SIZE_SIZE;
      set_open_rptr();
      ++m_head->m_rcount;
      m_wsem->post();
    }

    void rget()
    {
      uint32_t llen;
      bool lbret = false;
      while (1)
      {
        lbret = m_fun(rget_strat(llen), llen);
        rget_finish();
        if (!lbret)
        {
          break;
        }
      }
    }

    ~shared_memory_client(){}

  };

}


#endif
