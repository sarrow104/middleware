日志
==========================================



####简单介绍
------------------------------------------
* 用于写日志,模块引用[`looparray`](https://github.com/NingLeixueR/middleware/tree/master/src/loop_array)模块
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下,很简单)
* [`example_logsys.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_logsys.cpp)为测试示例


####以下是简单针对各个小用法的例子
------------------------------------------
* 获取logsys对象
```cpp
   int i = 0;
   middleware::tools::logsys* lp = middleware::tools::logsys::get_examples( i, "庄心妍");
```
* 写日志
```cpp
   lp->write( LOG_LEVE_WARN, "一人 独自唱情歌 却越唱越难过." );
   lp->write( LOG_LEVE_WARN, "承诺 我已说出口." );
   lp->write( LOG_LEVE_WARN, "沉默 把时间偷走 ." );
   lp->write( LOG_LEVE_WARN, "回忆 它在我左右 ." );
   /** 使用宏,方便排版 */
   LOG_ERROR(LOG_SOCKET_IO_ID, LOG_SOCKET_IO_STR,"看星星%d颗%d颗连成线", 1, 2 );
```
* 刷新文件流
```cpp
   lp->flush();
```

####最后
------------------------------------------
如果您有更好的建议,可以通过邮件方式联系我
希望与您共同进步
