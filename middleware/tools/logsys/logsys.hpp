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

#define LOG_SYS_BUFFER_SIZE             (2048)
#define LOG_SYS_EVERY_MAX_BUFFER_SIZE   (256)

#ifdef _MSC_VER

#define snprintf _snprintf

#endif //_MSC_VER

#define LOG_PRINTF( LOG_LEVEL, LOG_ID, LOG_NAME, FORMAT, ...)  \
{\
	char lch[256];\
	if( snprintf( lch, 256, FORMAT, __VA_ARGS__) > 0 )\
	{\
		 middleware::tools::WLOG( LOG_LEVEL, LOG_ID, LOG_NAME, lch);\
	}\
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

  } //namespace tools
} //namespace middleware

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
