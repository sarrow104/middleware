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
  serializecpp_buffer lsbuf(10240);
  //针对反序列化
  serializecpp_buffer lsbuf2(lsbuf.get_buffer(),lsbuf.get_uselen());
```
* 以下是几种简单类型的演示,您可以使用各种各样的类型去测试其是否支持,不支持嵌套容器类型与某些嵌套的类型
```cpp
  int linum = 1023;
  serializecpp::push( &lsbuf, linum );
  uint16_t lsnum = 123;
  serializecpp::push( &lsbuf, lsnum );
  char lcnum = 'a';
  serializecpp::push( &lsbuf, lcnum );
```
* 然后您可以获取序列化的buffer与其长度
```cpp
  char* ap = lsbuf.get_buffer();
  uint32_t aplen = lsbuf.get_uselen();
```
* 接着就是解析了,想知道解析的结果么,您可以自己试试
```cpp
  int linum2 = 0;
  unserializecpp::pop( &lsbuf2, linum2 );
  uint16_t lsnum2 = 0;
  unserializecpp::pop( &lsbuf2, lsnum2 );
  char lcnum2 = 0;
  unserializecpp::pop( &lsbuf2, lcnum2 );
```
* 最后让我来介绍一下如何使用自定的类类型
```cpp
  /* 首先是定一个自定义的类型，需要继承自serializecpp_class*/
  class test_class1:
  	public serializecpp_class
  {
  public:
    // 测试成员 为了简单赋值我们姑且定义为public
  	char arg1[1024];
  	int  arg2;
  	std::vector<int> arg3;
  	virtual bool push()
  	{
  		check( serializecpp::push( get_serializecpp_buffer(), arg1 ) );
  		check( serializecpp::push( get_serializecpp_buffer(), arg2 ) );
  		return serializecpp::push( get_serializecpp_buffer(), arg3 );
  	}
  	virtual bool pop()
  	{
  		check( unserializecpp::pop( get_serializecpp_buffer(), arg1 ) );
  		check( unserializecpp::pop( get_serializecpp_buffer(), arg2 ) );
  		return unserializecpp::pop( get_serializecpp_buffer(), arg3 );
  	}
        //这个函数主要是我们测试用的
  	bool operator==(test_class1& r)
  	{
  		return ( memcmp( arg1, r.arg1,1024 ) == 0) && arg2==r.arg2  && arg3==r.arg3;
  	}
  };
  //让我们来序列化他吧
  test_class1 abc;
  memcpy( abc.arg1,"I love WangAiCai,But her not love me!",sizeof("I love WangAiCai,But her not love me!")+1 );
  abc.arg2 = 1234;
  int t[] = {1,2,3,4};
  std::copy( abc.arg3.begin(), abc.arg3.end(),t );
  ((serializecpp_class*)(&abc))->push( &lsbuf );
  //现在我们来反序列化出这个类
  serializecpp_buffer lsbuf2(lsbuf.get_buffer(),lsbuf.get_uselen());
  test_class1 abc2;
  ((serializecpp_class*)(&abc2))->pop( &lsbuf2 );
  //测试一下结果如何
  cout<< ( ( abc == abc2) ? "==" : "!=") << endl;
```


####题外话
------------------------------------------
通过各种组合,
您一定会发现简单基础的东西竟然可以拼凑出像瑞士军刀一样锋利的东西.
以后我会将各个小项目融合起来,拼凑出一个简单的分布式服务器,拭目以待吧...


####最后
------------------------------------------
如果您有更好的建议,可以通过邮件方式联系我
希望与您共同进步
