通信模块c接口
==========================================


####简单介绍
------------------------------------------
*  提供简单的接口供其他语言调用,目前支持c/c++,将来会为更多的语言提供接口.
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下)



####支持接口
------------------------------------------
* c接口-[`example_middleware_base.c文件`](https://github.com/NingLeixueR/middleware/blob/master/OtherLanguages/c/middleware_base/example_middleware_base.c)为测试示例



####接口函数介绍
*  c接口
  * 初始化函数
  ```c
  /***** 一对一 *****/
  /* 初始化共享内存服务器 */
  void* init_middleware_sm_server(
		uint32_t aiconfigtype,
		const char* aiconfigpath,
		callbackfun logic_fun
		);
	void* init_middleware_sm_server2(
		uint32_t aiconfigtype,
		const char* apconfigtxt,
		uint32_t apconfigtxtlen,
		callbackfun logic_fun
		);
    /* 初始化共享内存客户端 */
    void* init_middleware_sm_client(
		uint32_t aiconfigtype,
		const char* aiconfigpath,
		callbackfun logic_fun);
	void* init_middleware_sm_client2(
		int32_t aiconfigtype,
		const char* apconfigtxt,
		uint32_t apconfigtxtlen,
		callbackfun logic_fun);
    /* 初始化循环数组服务器 */
    void* init_middleware_la_server(
		uint32_t aiconfigtype,
		const char* aiconfigpath,
		callbackfun logic_fun);
	void* init_middleware_la_server2(
		uint32_t aiconfigtype,
		const char* apconfigtxt,
		uint32_t apconfigtxtlen,
		callbackfun logic_fun);
    /*初始化循环数组客户端*/
    void* init_middleware_la_client(
   uint32_t aiconfigtype,
   const char* aiconfigpath,
   callbackfun logic_fun);
 void* init_middleware_la_client2(
   uint32_t aiconfigtype,
   const char* apconfigtxt,
   uint32_t apconfigtxtlen,
   callbackfun logic_fun);
    /***** 一对多*****/
    /*初始化简单socket服务器*/
    void* init_middleware_soio_server(
   uint32_t aiconfigtype,
   const char* aiconfigpath,
   multi_recv_fun logic_recv_callback,
   sendfailure_fun aisendfailure
   );
 void* init_middleware_soio_server2(
   uint32_t aiconfigtype,
   const char* apconfigtxt,
   uint32_t apconfigtxtlen,
   multi_recv_fun logic_recv_callback,
   sendfailure_fun aisendfailure
   );
    /*初始化简单socket客户端*/
    void* init_middleware_soio_client(
		uint32_t aiconfigtype,
		const char* aiconfigpath,
    multi_recv_fun logic_recv_callback
    );
	void* init_middleware_soio_client(
		uint32_t aiconfigtype,
		const char* apconfigtxt,
		uint32_t apconfigtxtlen,
		multi_recv_fun logic_recv_callback
		);
    ```
  * 发送与关闭
  ```c
  /***** 一对一 *****/
 bool send_middleware(void* ap, char* apdata, uint32_t aiwlen);
 bool close_middleware_sm(void* ap);
  /***** 一对多*****/
  //建立连接
  bool create_connect(uint32_t aikey,
		void* ap,
		uint32_t aiconfigtype,
		const char* aiconfigpath,
		sendfailure_fun aisendfailure
		);
	bool create_connect2(uint32_t aikey,
		void* ap,
		uint32_t aiconfigtype,
		const char* apconfigtxt,
		uint32_t apconfigtxtlen,
		sendfailure_fun aisendfailure
		);
  bool send_middleware_soio(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen);
  bool close_middleware_soio(void* ap, uint32_t aikey);
  ```
