///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef LOG_HPP
#define LOG_HPP

#define FLUSH_TIME            (5)       /** 多久向文件刷一次数据 */
#define LOG_LEVE_WARN         (0)         /** 警告 */
#define LOG_LEVE_INFO         (1)         /** 输出 */
#define LOG_LEVE_ERROR        (2)         /** 错误 */
#define LOG_LEVE_WARN_STR     "WARN"      /** 警告 */
#define LOG_LEVE_INFO_STR     "INFO"      /** 输出 */
#define LOG_LEVE_ERROR_STR    "ERROR"     /** 错误 */

#ifndef __cplusplus
#define true   (1)
#define false  (0)
#endif
#define OPEN_LOGFILE_PRINTF       (true)         /** 是否文本输出 */
#define OPEN_CONSOLE_PRINTF       (true)         /** 是否打开控制台输出,多个线程输出可能会乱,因为 */
#define OPEN_LOG_ERROR            (1)            /** 是否开始标志为错误的打印 */
#define OPEN_LOG_INFO             (0)            /** 是否开启标志为普通输出信息的打印 */
#define OPEN_LOG_WARN             (1)            /** 是否开启标志为警告的打印 */
#define CHECK_PRINTF_LOG( OPEN_LOG_TYPE )      ( ( OPEN_LOGFILE_PRINTF || OPEN_CONSOLE_PRINTF ) && OPEN_LOG_TYPE ) /** 检查是否打印 */

#define DEFAULT_SAVE_LOG_TIME           (12*60*60*60)  /** 12 hour */

#define LOG_SYS_BUFFER_SIZE             (2048)
#define LOG_SYS_EVERY_MAX_BUFFER_SIZE   (256)

#ifdef _MSC_VER
# ifndef snprintf
#  define snprintf _snprintf
# endif //snprintf
#endif //_MSC_VER

#ifdef __cplusplus
# define LOG_PRINTF( LOG_LEVEL, LOG_ID, LOG_NAME, FORMAT, ...)           \
  {                                                                      \
    char lch[256];                                                       \
    if( snprintf( lch, 256, FORMAT, __VA_ARGS__) > 0 )                   \
    {                                                                    \
      middleware::tools::WLOG( LOG_LEVEL, LOG_ID, LOG_NAME, lch);        \
    }                                                                    \
  }
#else
# define LOG_PRINTF( LOG_LEVEL, LOG_ID, LOG_NAME, FORMAT, ...)           \
  {                                                                      \
    char lch[256];                                                       \
    if( snprintf( lch, 256, FORMAT, __VA_ARGS__) > 0 )                   \
    {                                                                    \
      WLOG( LOG_LEVEL, &LOG_ID, LOG_NAME, lch);                          \
    }                                                                    \
  }
#endif //__cplusplus



/**
* 创建一个日志文件
*/
#ifdef __cplusplus
# define CREATE_LOG( LOG_ID, LOG_NAME )    middleware::tools::CLOG(LOG_ID,LOG_NAME);
#else
# define CREATE_LOG( LOG_ID, LOG_NAME )    CLOG(&LOG_ID,LOG_NAME);
#endif //__cplusplus
/**
*  打印一条日志
*/
#define LOG_ERROR( LOG_ID, FORMAT, ...)  if(CHECK_PRINTF_LOG(LOG_LEVE_ERROR)) {LOG_PRINTF( LOG_LEVE_ERROR, LOG_ID, "", FORMAT, __VA_ARGS__)}
#define LOG_INFO( LOG_ID, FORMAT, ...)   if(CHECK_PRINTF_LOG(LOG_LEVE_INFO)) {LOG_PRINTF( LOG_LEVE_INFO, LOG_ID, "", FORMAT, __VA_ARGS__)}
#define LOG_WARN( LOG_ID, FORMAT, ...)   if(CHECK_PRINTF_LOG(LOG_LEVE_WARN)) {LOG_PRINTF( LOG_LEVE_WARN, LOG_ID, "", FORMAT, __VA_ARGS__)}


#ifdef __cplusplus
#define LOG_PLUSH( LOG_ID )    middleware::tools::WPLUSH(LOG_ID);
#else
#define LOG_PLUSH( LOG_ID )    WPLUSH(LOG_ID);
#endif //__cplusplus

#endif //LOG_HPP

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
