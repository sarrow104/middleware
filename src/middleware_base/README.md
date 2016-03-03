通信模块接口
==========================================


####简单介绍
------------------------------------------
*  提供简单的接口供其他语言调用,目前支持c/c++,将来会为更多的语言提供接口.
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下)



####支持接口
------------------------------------------
* c++接口-[`example_middleware_base.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_middleware_base.cpp)为测试示例



####接口函数介绍
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
  /* asio 服务器 */
  class middleware_asio_server;
  /* asio 客户端*/
  class middleware_asio_client;
  ```
