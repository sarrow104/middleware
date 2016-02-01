///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/logsys/logsys.h"

int main()
{
	rename( "d:\\abc", ( std::string("d:\\abc") + "def" ).c_str() );
	


  int iz = 0;
  middleware::tools::logsys* lp = middleware::tools::logsys::get_examples( iz, "庄心妍");

  
  lp->write( LOG_LEVE_WARN, "一人 独自唱情歌 却越唱越难过." );
  lp->write( LOG_LEVE_WARN, "承诺 我已说出口." );
  lp->write( LOG_LEVE_WARN, "沉默 把时间偷走 ." );
  lp->write( LOG_LEVE_WARN, "回忆 它在我左右 ." );

  int ic = 0;
  middleware::tools::logsys* lp2 = middleware::tools::logsys::get_examples( ic, "陈奕迅");
  lp2->write( LOG_LEVE_WARN, "想起我不完美,你会不会逃离我生命的范围  ." );
  lp2->write( LOG_LEVE_WARN, "一个人失眠 全世界失眠   ." );

  middleware::tools::logsys::get_examples(iz, "庄心妍")->write(LOG_LEVE_WARN, "有一种爱叫做放手,为爱放弃天长地久.");

  boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  lp->flush();
  lp2->flush();
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
  return 0;
}
