///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/socket_io/lpthread.h"
#include "middleware/loop_array/looparray.h"
#include "logdef.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdint>
#include <cstdio>

<<<<<<< HEAD
#define FLUSH_TIME				(5)				/** 多久向文件刷一次数据 */
#define LOG_LEVE_WARN     		(0)   			/** 警告 */
#define LOG_LEVE_INFO     		(1)   			/** 输出 */
#define LOG_LEVE_ERROR    		(2)   			/** 错误 */
#define LOG_LEVE_WARN_STR     "WARN"   	  /** 警告 */
#define LOG_LEVE_INFO_STR     "INFO"   	  /** 输出 */
#define LOG_LEVE_ERROR_STR    "ERROR"     /** 错误 */

#define OPEN_LOGFILE_PRINTF				(true)			   /** 是否文本输出 */
#define OPEN_CONSOLE_PRINTF				(true)			   /** 是否打开控制台输出,多个线程输出可能会乱,因为 */
#define OPEN_LOG_ERROR    				(1)						 /** 是否开始标志为错误的打印 */
#define OPEN_LOG_INFO     				(0)						 /** 是否开启标志为普通输出信息的打印 */
#define OPEN_LOG_WARN     				(1)						 /** 是否开启标志为警告的打印 */
#define CHECK_PRINTF_LOG( OPEN_LOG_TYPE )      ( ( OPEN_LOGFILE_PRINTF || OPEN_CONSOLE_PRINTF ) && OPEN_LOG_TYPE ) /** 检查是否打印 */

#define DEFAULT_SAVE_LOG_TIME			(12*60*60*60)  /** 12 hour */
=======
#define LOG_LEVE_WARN           0       /** 警告 */
#define LOG_LEVE_INFO           1       /** 输出 */
#define LOG_LEVE_ERROR          2       /** 错误 */
#define LOG_LEVE_WARN_STR       "WARN"  /** 警告 */
#define LOG_LEVE_INFO_STR       "INFO"  /** 输出 */
#define LOG_LEVE_ERROR_STR      "ERROR" /** 错误 */


#define OPEN_CONSOLE_PRINTF     (1)     /** 是否打开控制台输出,多个文件输出可能会乱 */

#define DEFAULT_SAVE_LOG_TIME   (20)    //(12*60*60*60)  /** 12 hour */

>>>>>>> 851e4d4c1f16c4a0d974ed7074d7eeac44ceb1f0

#define LOG_SYS_BUFFER_SIZE             (2048)
#define LOG_SYS_EVERY_MAX_BUFFER_SIZE   (256)

#ifdef _MSC_VER

#define snprintf _snprintf

#endif //_MSC_VER

#define LOG_PRINTF( LOG_LEVEL, LOG_ID, LOG_NAME, FORMAT, ...)  \
{\
<<<<<<< HEAD
	char lch[256];\
	if( snprintf( lch, 256, FORMAT, __VA_ARGS__) > 0 )\
	{\
		 middleware::tools::WLOG( LOG_LEVEL, LOG_ID, LOG_NAME, lch);\
	}\
=======
        char lch[256];\
        if( snprintf( lch, 256, FORMAT, __VA_ARGS__) > 0 )\
        {\
                 middleware::tools::logsys::get_examples(LOG_ID, LOG_NAME)->write(LOG_LEVE_ERROR, lch);\
        }\
>>>>>>> 851e4d4c1f16c4a0d974ed7074d7eeac44ceb1f0
}

/**
 * 创建一个日志文件
 */
#define CREATE_LOG( LOG_ID, LOG_NAME )    middleware::tools::CLOG(LOG_ID,LOG_NAME);

 /**
 *  打印一条日志
 */
#define LOG_ERROR( LOG_ID, FORMAT, ...)  if(CHECK_PRINTF_LOG(LOG_LEVE_ERROR))LOG_PRINTF( LOG_LEVE_ERROR, LOG_ID, "", FORMAT, __VA_ARGS__)
#define LOG_INFO( LOG_ID, FORMAT, ...)   if(CHECK_PRINTF_LOG(LOG_LEVE_INFO))LOG_PRINTF( LOG_LEVE_INFO, LOG_ID, "", FORMAT, __VA_ARGS__)
#define LOG_WARN( LOG_ID, FORMAT, ...)   if(CHECK_PRINTF_LOG(LOG_LEVE_WARN))LOG_PRINTF( LOG_LEVE_WARN, LOG_ID, "", FORMAT, __VA_ARGS__)

<<<<<<< HEAD
#define LOG_PLUSH( LOG_ID )    middleware::tools::WPLUSH(LOG_ID);

namespace middleware{
  namespace tools{
  /**
   * 以下函数不能直接调用,
   * 但却无法硬性限制
   */

	/**
	 *  创建日志
	 */
	bool CLOG(int& LOG_ID, const char* LOG_NAME);
	
   /**
    *  写日志
	*/
   bool WLOG(uint32_t LOG_LEVEL, int& LOG_ID, const char* LOG_NAME, const char* ap);

  /**
   *  像日志刷新数据
   */
  bool WPLUSH(int LOG_ID);
=======
/**
 * portable version localtime_r and gmtime_r
 *
 * from
 * https://gist.github.com/jonforums/2701145
 *
 * 2016-02-02
 */
#if defined(__MINGW64_VERSION_MAJOR)
/* XXX why isn't this in sec_api/time_s.h? */
# if defined(_USE_32BIT_TIME_T)
#  define gmtime_s _gmtime32_s
# else
#  define gmtime_s _gmtime64_s
# endif
#elif defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
# define BUILDING_WITH_MINGW32
# define gmtime_r(tp, tm) (gmtime((tp)))
# define localtime_r(tp, tm) (localtime((tp)))
#endif

#if defined(_MSC_VER) || defined(__MINGW64_VERSION_MAJOR)
# define gmtime_r(tp, tm) ((gmtime_s((tm), (tp)) == 0) ? (tm) : NULL)
# define localtime_r(tp, tm) ((localtime_s((tm), (tp)) == 0) ? (tm) : NULL)
#endif

namespace middleware{
  namespace tools{

    /**
     * 写日志类
     */
    class logsys:
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
      boost::mutex m_lock;
      static boost::mutex m_creat_lock;
      static std::vector<logsys*> m_log_list;

      /**
       * 获取当前时间字符串
       */
      bool get_now( char* ap, uint32_t aplen, char aseparator1 = '-', char aseparator2 = ':', time_t aitimep = 0 )
      {
        if( aitimep == 0)
        {
          time(&aitimep);
        }

        /* 东八区 +8小时 */
        // aitimep += 1800;  
        // tm* lptm = gmtime(&aitimep);

        struct tm lt;

        tzset(); // set timezone
        //! http://stackoverflow.com/questions/19170721/real-time-awareness-of-timezone-change-in-localtime-vs-localtime-r

        struct tm* lptm = localtime_r(&aitimep, &lt);

        /** int lhour = lptm->tm_hour+8; */
        /** lhour = lhour > 23 ? lhour - 24 : lhour; */

        // FIXME TODO
        // strftime will be more suitable here!
        return snprintf(
          ap,
          aplen,
          "%d%c%d%c%d %d%c%d%c%d",
          lptm->tm_year + 1970,         /* 年 */
          aseparator1,                  /* 分隔符 */
          lptm->tm_mon + 1,             /* 月 */
          aseparator1,                  /* 分隔符 */
          lptm->tm_mday,                /* 日 */

          lptm->tm_hour,                   /* 小时 */
          aseparator2,             /* 分隔符 */
          lptm->tm_min,            /* 分钟 */
          aseparator2,             /* 分隔符 */
          lptm->tm_sec             /* 秒 */
          ) >= 0;
      }

      bool creat_log()
      {
        boost::filesystem::path path("./log");
        /** 检查创建路径 */
        bool result = boost::filesystem::is_directory(path);
        if( !result )
        {
          boost::filesystem::create_directory(path);
        }

        char lpbuf[256];
        if( get_now( lpbuf, 256, '_', '_' ) )
        {
          m_logname_beg += "./log/";
          m_logname_beg += m_logname;
          m_logname_beg += lpbuf;
          m_logfile.open(m_logname_beg.c_str() , std::ios::trunc|std::ios::out);
          if( m_logfile.is_open() )
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
        while(1)
        {
          boost::this_thread::sleep(boost::posix_time::seconds(DEFAULT_SAVE_LOG_TIME));
          boost::mutex::scoped_lock llock(m_lock);
          {/** 锁作用域 */
            m_logfile.close();
            boost::filesystem::path path(m_logname_beg); 

            char lpbuf[256];
            if( get_now( lpbuf, 256, '_', '_' ) )
            {
              rename( m_logname_beg.c_str(), ( m_logname_beg + "__" + lpbuf + ".log" ).c_str() );
              //boost::filesystem::rename(path, m_logname_beg + "__" + lpbuf + ".log");  
            }

          }/** 锁作用域 */
        }
      }

      /** 写log  */
      bool write_log( const char* ap, uint32_t aplen )
      {
        m_logfile << ap << '\n';
        if( OPEN_CONSOLE_PRINTF )
        {
          std::cout << ap << std::endl;
        }
        //std::cout << ap << "[" << aplen<< "]" << std::endl;
        return true;
      }

      logsys( const char* ailogname , int& ainum):
        m_logname( ailogname),
        m_looparray( LOG_SYS_BUFFER_SIZE , LOG_SYS_EVERY_MAX_BUFFER_SIZE, boost::bind( &logsys::write_log, this, _1, _2 ) ,false ),
        mp_buf1(m_buf1),
        mp_buf2(m_buf2)
      {
        if( creat_log() )
        {
          m_log_list.push_back( this );
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
      const char* get_type_str( uint32_t aitype)
      {
        const char* lret = nullptr;
        switch( aitype )
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

      /**
       * 获取实例
       * 1.ainame == "" 并且 ainum有正常值 则返回该ainum对应的日志对象
       * 2.ainume != ""
       *  一 ainum正常值  直接返回ainum对应的日志对象
       *  二  如果ainum非正常值则新建
       */
      static logsys* get_examples( int& ainum , const char* ainame = "")
      {
        if( ainame == NULL || ainame[0] == '\0')
        {
          if( (uint32_t)ainum >= m_log_list.size() )
          {
            return nullptr;
          }
          else
          {
            return m_log_list[ainum];
          }
        }
        else
        {
          if( ainum == 0 || (uint32_t)ainum >= m_log_list.size() )
          {
            boost::mutex::scoped_lock llock(m_creat_lock);
            return new logsys( ainame , ainum );
          }
          else
          {
            if( m_log_list[ainum]->m_logname ==  ainame)
            {
              return m_log_list[ainum];
            }
            else
            {
              return nullptr;
            }
          }
        }
      }

      /**
       *  写日志
       */
      void write( uint32_t aitype, const char* ap,...)
      {
        char lbuf[256];
        if( get_now( lbuf , 256 ) )
        {
          std::string lstr(lbuf);
          lstr += '|';
          lstr += get_type_str(aitype);
          lstr += "|%s|%s|";
          int llen = snprintf( lbuf, 256, lstr.c_str(), m_logname.c_str(), ap  );
          if( llen > 0  )
          {
            boost::mutex::scoped_lock llock(m_lock);
            std::swap(mp_buf1, mp_buf2);
            ++llen;
            memcpy( mp_buf1, lbuf, llen);
            m_looparray.send( mp_buf1 , llen);
          }
        }
      }

      /**
       *  刷新文件流
       */
      void flush()
      {
        m_logfile.flush();
      }

    };
>>>>>>> 851e4d4c1f16c4a0d974ed7074d7eeac44ceb1f0

  } //namespace tools
} //namespace middleware

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
