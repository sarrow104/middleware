middleware(中间件)
===========================

####简单介绍
------------------------------------------
>* 方便两个模块间通信(线程间,进程间)
>>* 线程间
>>* 进程间
>>* PC间
>* 其中需要引用到 [c++ boost库](http://www.boost.org/)(去官网下载编译下,很简单)


####以下是现有各个模块,其工程下均有完整的README.md
------------------------------------------
>* 通信模块
>> [循环数组](https://github.com/NingLeixueR/middleware/src/loop_array)(用于线程间交互)
>> [共享内存](https://github.com/NingLeixueR/middleware/src/shared_memory)(用于进程间交互)

>* 现有工具模块
>> [序列化与反序列化](https://github.com/NingLeixueR/middleware/src/tools/serializecpp)
