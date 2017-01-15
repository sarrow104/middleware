///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef LOG_SYS_HPP
#define LOG_SYS_HPP

#include "middleware/middleware_base/socket_io/lpthread.hpp"
#include "middleware/middleware_base/loop_array/looparray.hpp"
#include "middleware/tools/logsys/logdef.hpp"
#include "middleware/tools/logsys/log.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdint>
#include <cstdio>

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

#endif //LOG_SYS_HPP

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
