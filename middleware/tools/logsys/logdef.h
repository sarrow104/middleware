///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef LOG_DEF_H
#define LOG_DEF_H


namespace middleware {
  namespace tools{

    /** 定义模块存储id的全局变量 */
    extern int g_log_socked_io_id;

  } //namespace tools
}//namespace middleware


/** 为每个使用logsys模块的定义一个宏 */
/** socket io模块 */
#define LOG_SOCKET_IO_ID   ( middleware::tools::g_log_socked_io_id )
#define LOG_SOCKET_IO_STR  ( "socket_io"  )




#endif //LOG_DEF_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
