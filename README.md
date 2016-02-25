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
  * [循环数组](https://github.com/NingLeixueR/middleware/tree/master/src/middleware_base/loop_array)---用于线程间交互
  * [共享内存](https://github.com/NingLeixueR/middleware/tree/master/src/middleware_base/shared_memory)---用于进程间交互
  * [socket io](https://github.com/NingLeixueR/middleware/blob/master/src/middleware_base/socket_io)---用于pc间模块通信
  * [socket asio](https://github.com/NingLeixueR/middleware/blob/master/src/middleware_base/socket_asio)---用于服务器与客户端交互

* **工具模块**
  * [序列化与反序列化](https://github.com/NingLeixueR/middleware/tree/master/src/tools/serializecpp)
  * [线程池](https://github.com/NingLeixueR/middleware/tree/master/src/tools/threadpool)
  * [日志](https://github.com/NingLeixueR/middleware/tree/master/src/tools/logsys)
  * [tcp分包](https://github.com/NingLeixueR/middleware/tree/master/src/tools/segmentation_pack)

#####支持接口[README.md](https://github.com/NingLeixueR/middleware/tree/master/src/middleware_base)
* c++
* c



#####最后
* 如果您有更好的建议,可以通过邮件方式联系我,希望与您共同进步
* 感谢@sarrow104,帮忙写cmake与指点代码及意见
