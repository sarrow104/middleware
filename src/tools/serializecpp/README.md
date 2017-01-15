类序列化与反序列化
==========================================



####简单介绍
------------------------------------------
* 以二进制形式将c++中基础类型、类等数据成员序列化与反序列化
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下,很简单)
* [`example_serializecpp.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_serializecpp.cpp)为测试示例


####以下是简单针对各个小用法的例子
------------------------------------------
* 首先定义serializecpp_buffer类,有两种方式
```cpp
  //主要针对于序列化
  middleware::tools::mgt_serializecpp lsbuf(10240);
  //针对反序列化
  middleware::tools::mgt_serializecpp lsbufpop;
```
* 设置序列化格式
```cpp
  //目前支持三种序列化格式
  enum
  {
    SERIALIZE_TYPE_BINARY,                              // 二进制
    SERIALIZE_TYPE_JSON,                                //  JSON
    SERIALIZE_TYPE_XML,                                 //  XML
  };
  //设置序列化格式为二进制
  lsbuf.reset(middleware::SERIALIZE_TYPE_BINARY);
```
* 以下是几种简单类型的演示
```cpp
  int linum = 1023;
  lsbuf.push(linum);
  uint16_t lsnum = 123;
  lsbuf.push(lsnum);
  char lcnum = 'a';
  lsbuf.push(lcnum);
```
* 然后您可以获取序列化的buffer与其长度
```cpp
  char* ap = (char*)lsbuf.get_buffer();
  uint32_t aplen = lsbuf.get_uselen();
```
* 接着就是解析了,想知道解析的结果么,您可以自己试试
```cpp
  lsbufpop.reset(middleware::SERIALIZE_TYPE_BINARY, (char*)lsbuf.get_buffer(), lsbuf.get_uselen());
  int linum2 = 0;
  lsbufpop.pop( linum2 );
  uint16_t lsnum2 = 0;
  lsbufpop.pop(lsnum2);
  char lcnum2 = 0;
  lsbufpop.pop(lcnum2);
```
* 最后让我来介绍一下如何使用自定的类类型
```cpp
  /* 首先是定一个自定义的类型，需要继承自serializecpp_class*/
  struct test_struct
 {
   char arg1[32];
   int  arg2;
   char arg3;
   /** 结构数组时候会被调用 */
   void endian()
   {
     //middleware::tools::gendian_local2net.endian(arg1,32);
     middleware::tools::gendian_local2net.endian(arg2);
     //middleware::tools::gendian_local2net.endian(arg3);
   }
   bool pop(middleware::tools::mgt_serializecpp& lp, const char* apkey = "")
   {
     lp.pop(arg1,32, (string(apkey)+"arg1").c_str());
     lp.pop(arg2, (string(apkey) + "arg2").c_str());
     lp.pop(arg3, (string(apkey) + "arg3").c_str());
     return true;
   }
   bool push(middleware::tools::mgt_serializecpp& lp, const char* apkey = "")
   {
     lp.push(arg1, 32, (string(apkey) + "arg1").c_str());
     lp.push(arg2, (string(apkey) + "arg2").c_str());
     lp.push(arg3, (string(apkey) + "arg3").c_str());
     return true;
   }
 };
  //让我们来序列化他吧
  test_struct lstruct;
  memcpy( lstruct.arg1,"I love WangAiCai,But her not love me!",sizeof("I love WangAiCai,But her not love me!")+1 );
  lstruct.arg2 = 1234;
  lstruct.arg3 = 'x';
  lsbuf.push_struct( lstruct );
  /** 数组 */
  test_struct larray[3];
  std::string ltempstr = "z";
  for( uint32_t i = 0;i<3;++i)
  {
    ltempstr += 'z';
    memcpy( larray[i].arg1,ltempstr.c_str(),ltempstr.length()+1 );
    larray[i].arg2 = i;
    larray[i].arg3 = 'a'+i;
  }
	lsbuf.push(larray,3);
  //现在我们来反序列化出这个类
  middleware::tools::mgt_serializecpp lsbufpop;
	lsbufpop.reset(middleware::SERIALIZE_TYPE_BINARY, (char*)lsbuf.get_buffer(), lsbuf.get_uselen());
  test_struct lstruct2;
	lsbufpop.pop_struct(lstruct2);
  /** 输出结果比较下 */
  cout<< "lstruct.arg1=["<<lstruct.arg1<<"],"<<"lstruct2.arg1=["<< lstruct2.arg1<<"]"<< endl;
  cout<< "lstruct.arg2=["<<lstruct.arg2<<"],"<<"lstruct2.arg2=["<< lstruct2.arg2<<"]"<< endl;
  cout<< "lstruct.arg3=["<<lstruct.arg3<<"],"<<"lstruct2.arg3=["<< lstruct2.arg3<<"]"<< endl;
```
*  更多的演示就不再一一进行了,包括json,xml,基本用法与binary差不多,想了解更多参见[`example_serializecpp.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_serializecpp.cpp)

####最后
------------------------------------------
如果您有更好的建议,可以通过邮件方式联系我
希望与您共同进步
