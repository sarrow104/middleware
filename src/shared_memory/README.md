共享内存,同pc间两进程通信
==========================================
####简单介绍
------------------------------------------
 *  用于同pc间两进程通信，
 *  也是中间件的基础部件，
 *  如果两个模块进程中有一个崩溃了,数据不会丢失,另一个进程会在共享内存被存满后阻塞,这样我们重启即可继续处理我们的数据了
 *  其中需要引用到 [c++ boost库](http://www.boost.org/)(去官网下载编译下,很简单)
 *  组件通信能力
 *  [main.cpp文件](https://github.com/NingLeixueR/shared_memory/blob/4b030c7e7a046eb4d92eadb0b9e9006b23aecd97/src/main.cpp)为测试示例中测试了此模块

####以下是简单针对各个小用法的例子
------------------------------------------
* 想要使用共享内存,作为某个模块间通信的桥梁,那么首先我们需要定义middleware_sms对象或者middleware_smc对象
```cpp
  /* 我们需要为定义middleware_sms对象做点准备 */
  /* 定义一个匿名函数做为测试的回调函数吧 */
  auto funservercallback = []( char* ap ,uint32_t& aplen)
  {
  	cout << ap << endl;
  	aplen = 0;
  	return true;
  };
  /*
  定义对象,
  第1个参数是共享内存的名字
  第2个参数你接收所使用的共享内存大小,
  第3个参数你发送所使用的共享内存大小,
  第4个参数是单条数据最大长度
  第5个参数是定义的回调函数
  */
  middleware_sms  abc( "kk" , 1024 , 1024 , 128, funservercallback );
  /*至此我们定义了一个,server端的共享内存,那么我们怎么定义client端的共享内存呢 */
  /* boost::function<bool ( char* ,uint32_t& )> */
  auto funclientcallback = []( char* ap ,uint32_t& aplen)
  {
	  cout << ap << endl;
	  aplen = 0;
	  return true;
  };
  /*
  定义对象参数同上middleware_sms对象
  */
   middleware_smc  abc( "kk" , 1024 , 1024 ,128, funclientcallback );
```

* 有了对象我们可以相互通信了,middleware_sms对象和middleware_smc对象,调用send方法即可
```cpp
  abc.send( "我爱北京天爱门" , sizeof("我爱北京天爱门") );
```

####题外话
------------------------------------------
简单的示例做完了,您只需要把他用在正确的模块间通信,
可能示例确实有些简单,那么等以后的日子里,
我会将更多的东西融入到一个项目中,他更加复杂与灵活的用法将会得以体现


####最后
------------------------------------------
如果您有更好的建议,可以通过邮件联系我
希望与您共同进步
