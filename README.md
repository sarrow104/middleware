middleware-中间件
------------------------

#####简单介绍
* **方便两个模块间通信**
  * 线程间-循环数组实现
  * 进程间-共享内存实现
  * PC间-简单Socket实现
  * 服务器-asio高并发实现

* **其中需要引用到 [c++ boost库](http://www.boost.org/)(去官网下载编译)**


#####现有各个模块
* **通信模块**
  * [循环数组](https://github.com/NingLeixueR/middleware/tree/master/src/loop_array)(用于线程间交互)
  * [共享内存](https://github.com/NingLeixueR/middleware/tree/master/src/shared_memory)(用于进程间交互)
  * [socket](https://github.com/NingLeixueR/middleware/tree/master/src/socket_io)(用于pc间模块通信)

* **工具模块**
  * [序列化与反序列化](https://github.com/NingLeixueR/middleware/tree/master/src/tools/serializecpp)
