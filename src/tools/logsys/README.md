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
  int iz = 0;
  CREATE_LOG(iz, "庄心妍")
  int ic = 0;
  CREATE_LOG(ic, "陈奕迅")
```
*  写日志
```cpp
  int iz = 0;
  int i = 0;
  CREATE_LOG(iz, "庄心妍")
  LOG_ERROR(iz,  "一人 独自唱情歌 %s.%d", "却越唱越难过" ,1314521)
  LOG_ERROR(iz, "承诺 %s", "我已说出口")
  LOG_ERROR(iz,  "沉默 %s", "把时间偷走")
  LOG_ERROR(iz,  "回忆 %s", "它在我左右")
  int ic = 0;
  CREATE_LOG(ic, "陈奕迅")
  LOG_ERROR(ic, "想起我不完美.")
  LOG_ERROR(ic, "你会不会逃离我生命的范围.")
  LOG_ERROR(ic, "一个人失眠  .")
  LOG_ERROR(ic, "全世界失眠 .")
  LOG_ERROR(LOG_SOCKET_IO_ID, LOG_SOCKET_IO_STR,"看星星%d颗%d颗连成线", 1, 2 );
```
* 刷新文件流
```cpp
  LOG_PLUSH(iz)
  LOG_PLUSH(ic)
```
* 更多用法（为了避免管理 id 和 name 造成混乱, 我们可以在['logdef.h'](https://github.com/NingLeixueR/middleware/tree/master/middleware/tools/logsys/logdef.h)中定义id和name ）
```cpp
  CREATE_LOG(LOG_SOCKET_IO_ID, LOG_SOCKET_IO_STR)
  /** 那么我们可以在任意代码中写日志了 */
  LOG_ERROR(LOG_SOCKET_IO_ID, "看星星%d颗%s颗连成线.", 1, "两" )
  LOG_PLUSH(LOG_SOCKET_IO_ID)
```

####最后
------------------------------------------
如果您有更好的建议,可以通过邮件方式联系我
希望与您共同进步
