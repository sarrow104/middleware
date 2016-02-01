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

struct pool_tag{};                  //仅仅用于标记的空类
#define MAKE_POOL(TYPE)					typedef boost::singleton_pool<pool_tag, sizeof(TYPE)> spl_##TYPE;
#define MALLOC_POOL(TYPE)				(TYPE*)spl_##TYPE::malloc()
#define FREE_POOL(TYPE,TYPE_PTR)		spl_##TYPE::free(TYPE_PTR)

#endif
