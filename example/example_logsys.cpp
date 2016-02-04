///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/logsys/logsys.hpp"

int main()
{
  int iz = 0;
  int i = 0;
  CREATE_LOG(iz, "庄心妍")
  LOG_ERROR(iz, "一人 独自唱情歌 却越唱越难过.%d", i++)
  LOG_ERROR(iz, "承诺 我已说出口.%d", i++)
  LOG_ERROR(iz, "沉默 把时间偷走.%d", i++)
  LOG_ERROR(iz,  "回忆 它在我左右.%d", i++)
  
  int ic = 0;
  CREATE_LOG(ic, "陈奕迅")
  LOG_ERROR(ic, "想起我不完美.", i++)
  LOG_ERROR(ic, "你会不会逃离我生命的范围.", i++)
  LOG_ERROR(ic, "一个人失眠  .", i++)
  LOG_ERROR(ic, "全世界失眠 .", i++)

	 
  LOG_PLUSH(iz)
  LOG_PLUSH(ic)


  CREATE_LOG(LOG_SOCKET_IO_ID, LOG_SOCKET_IO_STR)
  LOG_ERROR(LOG_SOCKET_IO_ID, "看星星%d颗%s颗连成线.", 1, "两" )
  LOG_PLUSH(LOG_SOCKET_IO_ID)
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
  return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
