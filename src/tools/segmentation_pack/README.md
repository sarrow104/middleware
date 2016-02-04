TCP分包
==========================================



####简单介绍
------------------------------------------
* 用于tcp沾包分包
* 其中需要引用到 [`c++ boost库`](http://www.boost.org/)(去官网下载编译下,很简单)
* [`example_segmentation_pack.cpp文件`](https://github.com/NingLeixueR/middleware/blob/master/example/example_segmentation_pack.cpp)为测试示例


####以下是简单针对各个小用法的例子
------------------------------------------
* 定义回调,回调可以绑定一个对象的指针
```cpp
  class abc
  {
  public:
    void print( const char* ap)
    {
      std::cout << &(ap[sizeof(uint32_t)]) << std::endl;
    }
  };
  /** 这样我们会将abc对象指针 回调的同时传递给回调函数,
      我们这么做仅仅是为了不需要每次绑定abc对象构造一个function对象 */
  auto fun = [](abc* ap, char* apstr, uint32_t apstrlen)
  {
    ap->print( apstr );
    return true;
  };
```
* 定义segmentation_pack对象与回调对象
```cpp
  middleware::tools::segmentation_pack<abc*> lsegmentation(fun);
  abc la;
```
* 构造测试数据
```cpp
  /** | len(sizeof(uint16_t)) | data(len) |  */
  char buff[1024000];
  char bufftemp[16];
  uint32_t j = 0;
  for (uint32_t i = 0; i <102400;++i)
  {
    #if defined(_MSC_VER)
      itoa(i, bufftemp, 10);
    #else
      sprintf(bufftemp, "%d", i);
    #endif
   *(uint32_t*)&(buff[j]) = (uint32_t)(strlen(bufftemp) + sizeof(uint32_t) +1);
   j += sizeof(uint32_t);
   memcpy(&(buff[j]), bufftemp, strlen(bufftemp)+1);
   j += strlen(bufftemp) + 1;
}
```
* 随机分包
```cpp
  uint32_t ltemp;
  for (int i = 0; i < int(j); )
  {
    ltemp = rand() % 100;
    if (ltemp + i > j)
    {
      ltemp = rand() % (j - i);
    }
    if (ltemp == 0)
    {
      continue;
    }
    lsegmentation.segmentation(&la, 0, &(buff[i]), ltemp);
    i += ltemp;
  }
```

####最后
------------------------------------------
如果您有更好的建议,可以通过邮件方式联系我
希望与您共同进步
