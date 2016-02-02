///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/threadpool/threadpool.h"

#include <iostream>

using std::cout;
using std::endl;


void test_thread(int ainum)
{
   boost::this_thread::sleep(boost::posix_time::milliseconds(ainum));
}


int main(int argc,char *argv[])
{ 
  middleware::tools::threadpool::set_maxthread( 5 );
  middleware::tools::threadpool::asyn_thread( boost::bind( &test_thread, rand()% 10 )  );
  middleware::tools::threadpool::asyn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::asyn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::syn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::syn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::syn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::asyn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::asyn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::asyn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::syn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::syn_thread( boost::bind( &test_thread, rand()% 10 ) );
  middleware::tools::threadpool::syn_thread( boost::bind( &test_thread, rand()% 10 ) );

  cout << "thread num = [" << middleware::tools::threadpool::thread_size() << "]" << endl;

  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
    return(0);
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
