通信模块接口
==========================================


####简单介绍
------------------------------------------
*  提供简单的接口供其他语言调用,目前支持c/c++,将来会为更多的语言提供接口.
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下)



####支持接口
------------------------------------------
* c接口-[`example_middleware_base.c文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_middleware_base.c)为测试示例
* c++接口-[`example_middleware_base.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_middleware_base.cpp)为测试示例



####接口函数介绍
*  c接口
  * 初始化函数
  ```c
  /***** 一对一 *****/
  /* 初始化共享内存服务器 */
  void* init_middleware_sm_server(
    const char* aismname,
    uint64_t  ai_client_byte_sum,
    uint64_t ai_server_byte_sum,
    uint32_t aieveryonemaxsize,
    callbackfun logic_fun);
    /* 初始化共享内存客户端 */
    void* init_middleware_sm_client(
      const char* aismname,
      uint64_t  ai_client_byte_sum,
      uint64_t ai_server_byte_sum,
      uint32_t aieveryonemaxsize,
      callbackfun logic_fun);
    /* 初始化循环数组服务器 */
    void* init_middleware_la_server(
      const char* ainame,
      uint32_t apbuffersize,
      uint32_t aieverymaxsize,
      callbackfun aireadfun,
      bool apstartthread)
    /*初始化循环数组客户端*/
    void* init_middleware_la_client(
      const char* ainame,
      uint32_t apbuffersize,
      uint32_t aieverymaxsize,
      callbackfun aireadfun,
      bool apstartthread)
    /***** 一对多*****/
    /*初始化简单socket服务器*/
    void* init_middleware_soio_server(
      uint32_t aiport,
      multi_recv_fun logic_recv_callback,
      uint32_t aimaxsize,
      uint32_t aievery_once_max_size,
      sendfailure_fun aisendfailure
      )
    /*初始化简单socket客户端*/
    void* init_middleware_soio_client(
      multi_recv_fun logic_recv_callback,
      uint32_t aimaxsize,
      uint32_t aievery_once_max_size
      )
    ```
  * 发送与关闭
  ```c
  /***** 一对一 *****/
  bool send_middleware(void* ap, char* apdata, uint32_t aiwlen);
  bool close_middleware(void* ap);
  /***** 一对多*****/
  bool create_connect_client(
    void* ap,
    uint32_t aikey,
    const char* aiserverip,
    uint32_t aiserverport,
    sendfailure_fun aisendfailure);//客户端的连接
  bool send_middleware_soio(void* ap, uint32_t aikey, char* apdata, uint32_t aiwlen);
  bool close_middleware_soio(void* ap, uint32_t aikey);
  ```
* cpp接口
  * 基类及虚方法
   ```cpp
    /* 一对一基类 */
    class middleware_base:
      public middleware_type  /* 区分类型的基类 */
      {
      public:
        virtual bool send( const char* apdata, uint32_t aiwlen) = 0;//发送
        virtual bool close() = 0;//关闭
      };
    /* 一对多基类 */
    class socket_middleware_base:
      public middleware_type  /* 区分类型的基类 */
      {
      public:
        virtual bool send(uint32_t aikey, const char* apdata, uint32_t aiwlen) = 0;//发送
        virtual bool close(uint32_t aikey) = 0;//关闭
        /** client 需要实现*/
        bool create_connect(
          uint32_t aikey,
          std::string aiserverip,
          uint32_t aiserverport,
          boost::function<bool(const char*, uint32_t)> aisendfailure
        )
        {
          return false;
        }
      };
    ```
  * 以下是各个类
  ```cpp
  /* 一对一 */
  /* 共享内存服务器 */
  class middleware_sm_server;
  /* 共享内存客户端 */
  class middleware_sm_client;
  /* 循环数组服务器 */
  class middleware_la_server;
  /* 环数组客户端 */
  class middleware_la_client;
  /* 一对多 */
  /* 简单socket服务器 */
  class middleware_soio_server;
  /* 简单socket客户端 */
  class middleware_soio_client;
  ```
