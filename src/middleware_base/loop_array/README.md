循环数组
==========================================



####简单介绍
------------------------------------------
* 用于同进程两个模块间通信
* 也是中间件的基础部件
* 首先我需要整理下循环数组,在接下来的日子里,将依靠它拼凑出更多的功能
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下,很简单)
* [`example_loop_array.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_loop_array.cpp)为测试示例
* 组件通信能力


####以下是单向数据传递示例
------------------------------------------
* 我们先写个回调函数,假设他就是另一个模块的入口
```cpp
    /* 我们需要为定义module_communicate对象做点准备 */
    /* 定义一个匿名函数做为测试的回调函数吧 */
    auto fun = [&lnow,&aisize](char* ap, uint32_t aplen)
    {
	cout << "[" << ap << "]\t[" << time(NULL) - lnow << "s]" << endl;
	return true;
    };
```
* 想要使用循环数组,作为某个模块间通信的桥梁,那么首先我们需要定义module_communicate对象
```cpp
  	/*
	定义对象,
	第一个参数你期望的循环数组大小,
	第二个参数是单条数据最大的长度,
	第三个参数是定义的回调函数
	第四个参数是否启动线程
	*/
	module_communicate lmc(10240, 1024,fun,false);
```
* 我们单向传递数组吧
```cpp
	uint32_t lsize = aisize;
	/*需要注意以下四句,您必须保证在写入前不能修改数据,所以我们使用双buffer解决这个问题*/
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
```

####以下是双向数据传递示例
------------------------------------------
* 首先我们定义两个thread函数,用来表示两个模块
```cpp
   void thread_fun( bool aibo ,uint32_t aisize)
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

```
* 调用线程,模拟两个模块通信
```cpp
    /*
       我们刻意让其中一个不打印回调,另一个打印别人传递给他的数据,
       因为两个数据不同,所以很明显能看出,回调在对方写入后立马被调用
    */
    boost::thread(boost::bind(&test2, true,atoi(argv[1])*100) );
    boost::thread(boost::bind(&test2, false,atoi(argv[1])) );
```
* 以上代码均摘自[`example_loop_array.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_loop_array.cpp)


####题外话
------------------------------------------
简单的示例做完了,您只需要把他用在正确的模块间通信,
可能示例确实有些简单,那么等以后的日子里,
我会将更多的东西融入到一个项目中,他更加复杂与灵活的用法将会得以体现
