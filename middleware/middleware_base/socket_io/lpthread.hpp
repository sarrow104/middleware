#ifndef B_PTHREAD_H
#define B_PTHREAD_H

//使用boost
#include <boost/thread/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using std::list;

typedef boost::interprocess::interprocess_semaphore boost_sem;

/** 线程基类 */
class pthread
{
public:
	pthread():
		thrd( boost::bind( &pthread::run, this ) )
	{}
	virtual int run()=0;
private:
	boost::thread thrd;
};



#endif

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
