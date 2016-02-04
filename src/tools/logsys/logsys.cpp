///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/logsys/logsys.hpp"

namespace middleware{
  namespace tools{

#define LOG_TIME_STANDARD     ("%Y-%m-%d %I:%M:%S")   /** 标准日志时间 */
#define LOG_TIME_FILE       ("%Y_%m_%d %I_%M_%S")   /** 日志文件时间 */
    
    /**
     * 写日志类
     */
  class logsys :
      public pthread
    {
      std::ofstream m_logfile;
      std::string m_logname;
      std::string m_logname_beg;
      module_communicate m_looparray;
      char m_buf1[256];
      char m_buf2[256];
      char* mp_buf1;
      char* mp_buf2;
      uint32_t m_cachingnum;    /** 缓存数目 */
      boost::mutex m_lock;
      static boost::mutex m_creat_lock;
      static std::vector<logsys*> m_log_list;
      static char ltimebuf[32];
      
      /**
      * 获取当前时间字符串
      */
      bool get_time_str( char* ap, uint32_t aplen,const char* aiformat, time_t aitimep = 0 )
      {
        if (aitimep == 0)
        {
          time(&aitimep);
        }
        /** 东八区 +8小时 */
        aitimep += 1800;
        tm* lptm = gmtime(&aitimep);
        strftime(ap, aplen, aiformat, lptm);
        return true;
      }

      bool creat_log()
      {
        boost::filesystem::path path("./log");
        /** 检查创建路径 */
        bool result = boost::filesystem::is_directory(path);
        if (!result)
        {
          boost::filesystem::create_directory(path);
        }

        char lpbuf[256];
        m_logname_beg = "";
        if (get_time_str(lpbuf, 256, LOG_TIME_FILE))
        {
          m_logname_beg += "./log/";
          m_logname_beg += m_logname;
          m_logname_beg += lpbuf;
          m_logfile.open(m_logname_beg.c_str(), std::ios::trunc | std::ios::out);
          if (m_logfile.is_open())
          {
            return true;
          }
          else
          {
            return false;
          }
        }
        else
        {
          return false;
        }
      }

      virtual int run()
      {
        int ltemp = DEFAULT_SAVE_LOG_TIME;
        while (1)
        {
          boost::this_thread::sleep(boost::posix_time::seconds(FLUSH_TIME));
          ltemp -= FLUSH_TIME;
          if (ltemp <= 0)
          {
            ltemp = DEFAULT_SAVE_LOG_TIME;
            flush();
          }
          else
          {
            continue;
          }

          boost::mutex::scoped_lock llock(m_lock);
          {/** 锁作用域 */
            m_logfile.close();
            boost::filesystem::path path(m_logname_beg);

            char lpbuf[256];
            if (get_time_str(lpbuf, 256, LOG_TIME_FILE))
            {
              rename(m_logname_beg.c_str(), (m_logname_beg + "__" + lpbuf + ".log").c_str());
              //boost::filesystem::rename(path, m_logname_beg + "__" + lpbuf + ".log");
            }

          }/** 锁作用域 */

          creat_log();
        }
      }


      /**
       *  写log
       */
      bool write_log(const char* ap, uint32_t aplen)
      {
        if (OPEN_LOGFILE_PRINTF)
        {
          boost::mutex::scoped_lock llock(m_lock);
          {/** 锁作用域 */
            m_logfile << ap << '\n';
          }/** 锁作用域 */
        }

        if (OPEN_CONSOLE_PRINTF)
        {
          std::cout << ap << std::endl;
        }

        return true;
      }

      logsys(const char* ailogname, int& ainum) :
        m_logname(ailogname),
        m_looparray(LOG_SYS_BUFFER_SIZE, LOG_SYS_EVERY_MAX_BUFFER_SIZE, boost::bind(&logsys::write_log, this, _1, _2), false),
        mp_buf1(m_buf1),
        mp_buf2(m_buf2)
      {
        if (creat_log())
        {
          m_log_list.push_back(this);
          ainum = m_log_list.size() - 1;
        }
        else
        {
          ainum = -1;
        }
      }

      /**
      *  获取类型对应字符串
      */
      const char* get_type_str(uint32_t aitype)
      {
        const char* lret = nullptr;
        switch (aitype)
        {
        case LOG_LEVE_WARN:
          lret = LOG_LEVE_WARN_STR;
          break;
        case LOG_LEVE_INFO:
          lret = LOG_LEVE_INFO_STR;
          break;
        case LOG_LEVE_ERROR:
          lret = LOG_LEVE_ERROR_STR;
          break;
        }
        return lret;
      }

    public:
      ~logsys()
      {
        for (uint32_t i = 0; i < m_log_list.size(); i++)
        {
          delete m_log_list[i];
        }
      }


      static logsys* new_examples(int& ainum, const char* ainame = "")
      {
        boost::mutex::scoped_lock llock(m_creat_lock);
        return new logsys(ainame, ainum);
      }

      /**
      * 获取实例
      */
      static logsys* get_examples(int ainum)
      {
        if (ainum >=0 && (uint32_t)ainum < m_log_list.size())
        {
          return m_log_list[ainum];
        }
        else
          {
          return nullptr;
          }
      }

      static void idbyname(int id, const char*& ap)
      {
        logsys* lp = get_examples(id);
        if ( lp == nullptr)
        {
          ap = nullptr;
        }
        else
        {
          ap = lp->m_logname.c_str();
        }
      }

      static void namebyid(int& id, const char* apname)
      {
        std::string lstr( apname );
        uint32_t lisize = m_log_list.size();
        for (uint32_t i = 0; i < lisize; i++)
        {
          if (m_log_list[i]->m_logname == lstr)
          {
            id = (int)i;
            return;
          }
        }
        return;
      }


      /**
      *  写日志
      */
      bool write(uint32_t aitype, const char* ap)
      {
      char lbuf[256];
      get_time_str(lbuf, 256, LOG_TIME_STANDARD);
      std::string lstr(lbuf);
      lstr += '|';
        lstr += get_type_str(aitype);
      lstr += "|%s|%s|";
      int llen = snprintf(lbuf, 256, lstr.c_str(), m_logname.c_str(), ap);
      if (llen > 0)
      {
        boost::mutex::scoped_lock llock(m_lock);
        std::swap(mp_buf1, mp_buf2);
        ++llen;
        memcpy(mp_buf1, lbuf, llen);
        m_looparray.send(mp_buf1, llen);
        return true;
      }
      else
        {
        return false;
      }
      }

      /**
       *  刷新文件流
       */
      void flush()
      {
        boost::mutex::scoped_lock llock(m_lock);
        m_logfile.flush();
      }

    };

    boost::mutex logsys::m_creat_lock;
    std::vector<logsys*> logsys::m_log_list;
  char logsys::ltimebuf[32];

  bool CLOG(int& LOG_ID, const char* LOG_NAME)
  {
    if (middleware::tools::logsys::new_examples(LOG_ID, LOG_NAME) == nullptr)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  bool WLOG(uint32_t LOG_LEVEL, int& LOG_ID, const char* LOG_NAME, const char* ap)
  {
    return middleware::tools::logsys::get_examples(LOG_ID)->write(LOG_LEVE_ERROR, ap);
  }

  bool WPLUSH(int LOG_ID)
  {
    if(!OPEN_LOGFILE_PRINTF)
    {
      return true;
    }
    logsys* lptr = logsys::get_examples(LOG_ID);
    if (lptr == nullptr )
    {
      return false;
    }
    else
    {
      lptr->flush();
      return true;
    }
  }

  } //namespace tools
} //namespace middleware

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
