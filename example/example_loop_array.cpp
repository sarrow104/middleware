///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/loop_array/looparray.h"
#include "middleware/loop_array/loop_array_middleware.h"

#include <iostream>
#include <string>
#include <cstdio>
#if _MSC_VER
#define snprintf _snprintf
#endif

using std::cout;
using std::endl;

/** 循环数组buffer大小 */
#define TEST_BUFFER_SIZE       (10240)
/** 单条数据最大字节 */
#define TEST_ONCE_BUFFER_SIZE   (1024)

void test( uint32_t aisize)
{
  time_t lnow;
  auto fun = [&lnow,&aisize](const char* ap, uint32_t aplen)
  {
    cout << "[" << ap << "]\t[" << time(NULL) - lnow << "s]" << endl;
    return true;
  };
  middleware::module_communicate lmc(TEST_BUFFER_SIZE, TEST_ONCE_BUFFER_SIZE,fun,false);
  lnow = time(NULL);
  uint32_t lsize = aisize;
  char lchar1[64];
  char lchar2[64];
  char*p1 = lchar1;
  char*p2 = lchar2;
  uint32_t llen = 0;
  while (lsize--)
  {
    std::swap(p1, p2);
    llen = snprintf( p1 , 64, "%d" , lsize);
    lmc.send(p1, llen+1);
  }
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
}

void test2( bool aibo ,uint32_t aisize)
{
  time_t lnow;
  auto fun = [&lnow,&aisize,&aibo](const char* ap, uint32_t aplen)
  {
    if(aibo)
      cout << "[" << ap << "]\t[" << time(NULL) - lnow << "s]" << endl;
    return true;
  };
  middleware::middleware_looparray lmc( "lb", TEST_BUFFER_SIZE, TEST_ONCE_BUFFER_SIZE,fun,false,aibo);
  lnow = time(NULL);
  uint32_t lsize = aisize;
  char lchar1[64];
  char lchar2[64];
  char*p1 = lchar1;
  char*p2 = lchar2;
  uint32_t llen = 0;
  while (lsize--)
  {
    std::swap(p1, p2);
    llen = snprintf( p1 , 64, "%d" , lsize);
    lmc.send(p1, llen+1);
  }
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
}

int main(int argc,char** argv)
{
  if( argc == 1 )
  {
    cout << "|缺少测试参数|num|" << endl;
    return 0;
  }
  /** 单向的循环数组,一般发消息与回调模块交互 */
  boost::thread(boost::bind(&test, atoi(argv[1])) );
  /** 双向的循环数组,可以相互交互,不过在线程2里我们故意让第二个不输出 */
  boost::thread(boost::bind(&test2, true,atoi(argv[1])*100) );
  boost::thread(boost::bind(&test2, false,atoi(argv[1])) );
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
  return 0;
}
