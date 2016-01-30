共享内存,同pc间两进程通信
==========================================
####简单介绍
------------------------------------------
 *  用于同pc间两进程通信，
 *  也是中间件的基础部件，
 *  如果两个模块进程中有一个崩溃了,数据不会丢失,另一个进程会在共享内存被存满后阻塞,这样我们重启即可继续处理我们的数据了
 *  其中需要引用到 [c++ boost库](http://www.boost.org/)(去官网下载编译下,很简单)
 *  组件通信能力
 *  [example_shared_memory.cpp文件](https://github.com/NingLeixueR/middleware/blob/master/example/example_shared_memory.cpp)为测试示例中测试了此模块

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
