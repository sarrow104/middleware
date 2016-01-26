#include <iostream>
#include "..\include\looparray.h"
#include <string>
#include <cstdio>
#if _MSC_VER
#define snprintf _snprintf
#endif

using std::cout;
using std::endl;
/*
	可生成exe 测试dll中的各段代码
*/
#define TEST_BUFFER_SIZE		(10240)
#define TEST_ONCE_BUFFER_SIZE   (1024)

void test( uint32_t aisize)
{
	cout << "————————————开始测试循环数组————————————————————" << endl;
	time_t lnow;
	auto fun = [&lnow,&aisize](char* ap, uint32_t aplen)
	{
		cout << "[" << ap << "]\t[" << time(NULL) - lnow << "s]" << endl;
		return true;
	};
	module_communicate lmc(TEST_BUFFER_SIZE, TEST_ONCE_BUFFER_SIZE,fun,false);
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



#include "../include/loop_array_middleware.h"

void test2( bool aibo ,uint32_t aisize)
{
	time_t lnow;
	auto fun = [&lnow,&aisize,&aibo](char* ap, uint32_t aplen)
	{
		if(aibo)
			cout << "[" << ap << "]\t[" << time(NULL) - lnow << "s]" << endl;
		return true;
	};
	middleware_looparray lmc( "lb", TEST_BUFFER_SIZE, TEST_ONCE_BUFFER_SIZE,fun,false,aibo);
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
	/* 单向的循环数组,一般发消息与回调模块交互 */
	boost::thread(boost::bind(&test, atoi(argv[1])) );
	/* 双向的循环数组,可以相互交互,不过在线程2里我们故意让第二个不输出 */
	boost::thread(boost::bind(&test2, true,atoi(argv[1])*100) );
	boost::thread(boost::bind(&test2, false,atoi(argv[1])) );
	while (1)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}
	return 0;
}
