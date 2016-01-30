线程池
==========================================

####简单介绍
------------------------------------------
* 统一管理线程,通过回调进行work分配
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下,很简单)
* [`example_threadpool.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_threadpool.cpp)为测试示例

####以下是简单针对各个小用法的例子
------------------------------------------
* 设置最大线程数目
```cpp
  middleware::tools::threadpool::set_maxthread( 5 );
```

* 启动线程,执行指定的函数
```cpp
  /* 定义一个 测试的函数 */
  void test_thread(int ainum)
  {
     boost::this_thread::sleep(boost::posix_time::milliseconds(ainum));
  }
```
  * 异步启动
```cpp
   middleware::tools::threadpool::asyn_thread( boost::bind( &test_thread, rand()% 10 ) );
```
  * 同步启动
```cpp
   middleware::tools::threadpool::syn_thread( boost::bind( &test_thread, rand()% 10 ) );
```

####最后
------------------------------------------
如果您有更好的建议,可以通过邮件方式联系我
希望与您共同进步
