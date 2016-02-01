SOCKET IO
==========================================



####简单介绍
------------------------------------------
* 用于不同pc间两个模块间通信,模块引用[`looparray`](https://github.com/NingLeixueR/middleware/tree/master/src/loop_array)模块
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下,很简单)
* [`example_socket_io.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_socket_io.cpp)为测试示例
* 组件通信能力
* 鉴于boost asio实现服务器间大规模的连接,所以目前版本使用socket send recv实现,未考虑多连接,虽然也支持多连接,但希望数量上有所控制,之后会完善socket select的实现渐进的增长可稍微有效运用线程,asio可能是同pc间两个模块的终点,目前这么设计



####以下是数据传递示例
* 首先想要使用SOCKET IO传输数据需要先定义两个回调
  * recv回调,回在接收到数据时回调
  ```cpp
  /* recv call back */
  auto rcb = [](const char* ap,uint32_t aplen)
	{
		cout << *( (uint32_t*)ap) << endl;
		aplen = 0;
		return true;
	};
  ```
  * send失败回调,在发送失败后回调
  ```cpp
  /* send failure call back*/
	auto sfcb = [](const char* ap, uint32_t aplen)
	{
		cout << *((uint32_t*)ap) << endl;
		aplen = 0;
		return true;
	};
  ```
* 现在可以定义socket io对象了
  * server
  ```cpp
  /*
  参数1 端口号
  参数2 接收回调
  参数3 缓冲区大小
  参数4 单条数据最大长度
  参数5 发送失败回调
  */
  gateway_socket_server_con lserver( 13140, rcb, 10240, 1024, sfcb);
  ```
  * client
  ```cpp
  /*
  参数1 接收回调
  参数2 缓冲区大小
  参数3 单条数据最大长度
  */
  gateway_socket_client_con lclient(rcb, 10240, 1024);
  ```
* socket io 对象中client对象需要建立连接
  ```cpp
  /*
  参数1 key
  参数2 ip地址
  参数3 端口号
  参数4 发送失败的回调
  */
  lclient.create_con( 0 ,"127.0.0.1", 13140, sfcb);
  ```
* 发送数据
  ```cpp
  /*
  参数1 key
  参数2 ip地址
  参数3 端口号
  参数4 发送失败的回调
  */
  char lbuf[] = "我爱你";
  lclient.send( 0,lbuf, sizeof( lbuf ) );
  lserver.send( 0, lbuf, sizeof( lbuf ) );
  ```
* 关闭连接
  * server
  ```cpp
  /* 关闭所有连接 */
  lserver.close();
  ```
  * client
  ```cpp
  /* 关闭指定key对应的连接 */
  lserver.close(0);
  ```
####最后
* 如果您有更好的建议,可以通过邮件方式联系我,希望与您共同进步
