SOCKET asio
==========================================



####简单介绍
------------------------------------------
* 用于服务器客户端(c/s)间两个模块间通信
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下)
* [`example_socket_asio.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_socket_asio.cpp)为测试示例
* 组件通信能力



####以下是数据传递示例
* 首先客户端想要使用SOCKET ASIO,需要定义SOCKET ASIO对象
  * client对象与server对象
     * client对象传输数据需要先定义两个回调
     ```cpp
     /** recv call back */
     bool rcb(bool aisclient, uint32_t aikey, const char* ap, uint32_t aplen)
     {
	      middleware::unpack_head_process<middleware::cpack_head::protocol_head> luhp;
	      luhp.reset(ap, aplen);
	      char ch[sizeof("hello world")] = { 0 };
	      luhp.pop(ch);
	      std::cout << ch << std::endl;
	      return true;
     };
      /* send failure call back*/
    	auto sfcb = [](const char* ap, uint32_t aplen)
    	{
    		cout << *((uint32_t*)ap) << endl;
    		aplen = 0;
    		return true;
    	};
    ```
    * server对象构造前需要一个回调和初始化一个socket_asio_arg对象
    ```cpp
    /** recv call back */
    boost::function<bool(uint32_t,const char*, uint32_t)> apfun = [](uint32_t ainum,const char* ap, uint32_t aplen) {
      middleware::unpack_head_process<middleware::spack_head::protocol_head> luhp;
      luhp.reset( ap, aplen);
      char ch[sizeof("hello world")] = { 0 };
      luhp.pop(ch);
      std::cout << ch << std::endl;
      middleware::asio_server().send(ainum,ap, aplen);
      return true;
    };
    '''
    '''cpp
    /** 初始化一个socket_asio_arg对象 */
    std::vector<boost::function<bool(const char*, uint32_t)> > ltemp(5);
	  for (uint32_t i = 0; i < 5; ++i)
	  {
		  ltemp[i] = boost::bind( apfun, i, _1, _2);
	  }
	  middleware::socket_asio_arg larg(5, ltemp);
	  larg.m_activ = false;
	  larg.m_extern_activ = false;
	  larg.m_everyoncemaxsize = 1024;
	  larg.m_extern_everyoncemaxsize = 1024;
	  larg.m_extern_loopbuffermaxsize = 10240;
	  larg.m_loopbuffermaxsize = 10240;
	  larg.m_heartbeat_num = 32;
	  larg.m_persecond_recvdatatbyte_num = 1024;
	  larg.m_port = 13140;
	  larg.m_recvpack_maxsize = 1024;
	  larg.m_timeout = 10240;
	  larg.m_s2c = true;
	  larg.m_s2s = true;
	  larg.m_session_num = 10240;
    ```
  * 现在可以定义socket ASIO对象了
    * server
    ```cpp
    middleware::middleware_asio_server lser(larg);
    ```
    * client
    ```cpp
    middleware::middleware_asio_client lclient(boost::bind(&rcb, false, _1, _2, _3), 10240, 1024);
    /** 连接 */
    lclient.create_connect(0, "127.0.0.1", 13140, sfcb);
    ```
  * 发送数据
  ```cpp
  lphp.push("hello world");
  lphp.set_pack_head();
  lclient.send(0, lphp.get_send_buffer(), lphp.get_send_len());
  ```

####最后
* 如果您有更好的建议,可以通过邮件方式联系我,希望与您共同进步
