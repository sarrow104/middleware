///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include <iostream>
#include "middleware/tools/serializecpp/serializecpp.h"
#include "middleware/tools/serializecpp/unserializecpp.h"
#include "middleware/tools/serializecpp/serializecpp_class.h"
using namespace std;


/* 基础类型 */
void test_1( )
{
  middleware::tools::serializecpp_buffer lsbuf(10240);
  cout<< "************基础类型序列化与反序列化测试*************"<< endl;
  /* 序列化*/
  int linum = 1023;
  middleware::tools::serializecpp::push( &lsbuf, linum );
  uint16_t lsnum = 123;
  middleware::tools::serializecpp::push( &lsbuf, lsnum );
  char lcnum = 'a';
  middleware::tools::serializecpp::push( &lsbuf, lcnum );
  
  /* 反序列化 */
  middleware::tools::serializecpp_buffer lsbuf2(lsbuf.get_buffer(),lsbuf.get_uselen());

  int linum2 = 0;
  middleware::tools::unserializecpp::pop( &lsbuf2, linum2 );
  
  cout<< "linum=["<<linum<<"],"<<"linum2=["<< linum2<<"]"<< endl;

  uint16_t lsnum2 = 0;
  middleware::tools::unserializecpp::pop( &lsbuf2, lsnum2 );
  cout<< "lsnum=["<<lsnum<<"],"<<"lsnum2=["<< lsnum2<<"]"<< endl;

  char lcnum2 = 0;
  middleware::tools::unserializecpp::pop( &lsbuf2, lcnum2 );
  cout<< "lcnum=["<<lcnum<<"],"<<"lcnum2=["<< lcnum2<<"]"<< endl;

}
/* 结构类型(没有指针的那种结构体) */

void test_2()
{
  cout<< "************结构类型(没有指针的那种结构体)序列化与反序列化测试*************"<< endl;
  middleware::tools::serializecpp_buffer lsbuf(10240);
  struct test_struct
  {
    char arg1[32];
    int  arg2;
    char arg3;
  };

  test_struct lstruct;
  memcpy( lstruct.arg1,"my name is libo",sizeof("my name is libo")+1 );
  lstruct.arg2 = 15;
  lstruct.arg3 = 'x';
  middleware::tools::serializecpp::push( &lsbuf, lstruct );
  /* 数组 */
  test_struct larray[3];
  std::string ltempstr = "z";
  for( uint32_t i = 0;i<3;++i)
  {
    ltempstr += 'z';
    memcpy( larray[i].arg1,ltempstr.c_str(),ltempstr.length()+1 );
    larray[i].arg2 = i;
    larray[i].arg3 = 'a'+i;
  }
  middleware::tools::serializecpp::push( &lsbuf, larray , 3 );


  /* 反序列化 */
  middleware::tools::serializecpp_buffer lsbuf2(lsbuf.get_buffer(),lsbuf.get_uselen());

  test_struct lstruct2;
  middleware::tools::unserializecpp::pop( &lsbuf2, lstruct2 );
  cout<< "lstruct.arg1=["<<lstruct.arg1<<"],"<<"lstruct2.arg1=["<< lstruct2.arg1<<"]"<< endl;
  cout<< "lstruct.arg2=["<<lstruct.arg2<<"],"<<"lstruct2.arg2=["<< lstruct2.arg2<<"]"<< endl;
  cout<< "lstruct.arg3=["<<lstruct.arg3<<"],"<<"lstruct2.arg3=["<< lstruct2.arg3<<"]"<< endl;

  test_struct larray2[3];
  middleware::tools::unserializecpp::pop( &lsbuf2, larray2, 3 );
  for( uint32_t i=0;i<3;++i)
  {
    cout<< "larray["<< i <<"].arg1=["<<larray[i].arg1<<"],"<<"larray2["<< i <<"].arg1=["<< larray2[i].arg1<<"]"<< endl;
    cout<< "larray["<< i <<"].arg2=["<<larray[i].arg2<<"],"<<"larray2["<< i <<"].arg2=["<< larray2[i].arg2<<"]"<< endl;
    cout<< "larray["<< i <<"].arg3=["<<larray[i].arg3<<"],"<<"larray2["<< i <<"].arg3=["<< larray2[i].arg3<<"]"<< endl;

  }
}


/* 数组 字符串  std::string std::vector  不支持嵌套解析容器 */
void test_3()
{
  cout<< "**(数组 字符串  std::string std::vector  不支持嵌套解析容器)序列化与反序列化测试**"<< endl;
  middleware::tools::serializecpp_buffer lsbuf(10240);
  char lcarray[13] = {
    'a','b','c','d','e',
    'f','g','h','i','j',
    'k','l','m'
  };
  middleware::tools::serializecpp::push( &lsbuf, lcarray , 13 );
  const char* lcstr = "helloworld";
  middleware::tools::serializecpp::push( &lsbuf, lcstr , sizeof("helloworld") );


  std::string lstr ="my open code!";
  middleware::tools::serializecpp::push( &lsbuf, lstr );

  std::vector<int> lvec(10,38);
  middleware::tools::serializecpp::push( &lsbuf, lvec );

  std::set<int> lset;
  for(uint32_t i = 0; i < 20 ;++i)
  {
    lset.insert( rand() );
  }
  middleware::tools::serializecpp::push( &lsbuf, lset );

  std::map<int,int> lmap;
  for(uint32_t i = 0; i < 20 ;++i)
  {
    lmap.insert( std::make_pair( rand(),rand()) );
  }
  middleware::tools::serializecpp::push( &lsbuf, lmap );

  std::unordered_set<int> lunset;
  for(uint32_t i = 0; i < 20 ;++i)
  {
    lunset.insert( rand() );
  }
  middleware::tools::serializecpp::push( &lsbuf, lunset );

  /* 反序列化 */
  middleware::tools::serializecpp_buffer lsbuf2(lsbuf.get_buffer(),lsbuf.get_uselen());

  char lcarray2[13];
  middleware::tools::unserializecpp::pop( &lsbuf2, lcarray2, 13 );

  cout<<"lcarray[13]";
  cout<< ( ( memcmp( lcarray, lcarray2,13 ) == 0) ? "==" : "!=" );
  cout<<"lcarray2[13]"<<endl;
  
  char lcstr2[128];
  middleware::tools::unserializecpp::pop( &lsbuf2, lcstr2, 128 );
  cout<<"lcstr";
  cout<< ( ( strcmp( lcarray, lcarray2) == 0) ? "==" : "!=");
  cout<<"lcstr2"<<endl;

  std::string lstr2;
  middleware::tools::unserializecpp::pop( &lsbuf2, lstr2 );
  cout<<"lstr";
  cout<< ( ( lstr == lstr2) ? "==" : "!=");
  cout<<"lstr2"<<endl;

  std::vector<int> lvec2;
  middleware::tools::unserializecpp::pop( &lsbuf2, lvec2 );
  cout<<"lvec";
  cout<< ( ( lvec == lvec2) ? "==" : "!=");
  cout<<"lvec2"<<endl;

  std::set<int> lset2;
  middleware::tools::unserializecpp::pop( &lsbuf2, lset2 );
  cout<<"lset";
  cout<< ( ( lset == lset2) ? "==" : "!=");
  cout<<"lset2"<<endl;

  std::map<int,int> lmap2;
  middleware::tools::unserializecpp::pop( &lsbuf2, lmap2 );
  cout<<"lmap";
  cout<< ( ( lmap == lmap2) ? "==" : "!=");
  cout<<"lmap2"<<endl;

  std::unordered_set<int> lunset2;
  middleware::tools::unserializecpp::pop( &lsbuf2, lunset2 );
  cout<<"lunset";
  cout<< ( ( lunset == lunset2) ? "==" : "!=");
  cout<<"lunset2"<<endl;
}



/* 自定义类的序列化 */
class test_class1:
  public middleware::tools::serializecpp_class
{
public:
  /* 测试成员 */
  char arg1[1024];
  int  arg2;
  std::vector<int> arg3;
  virtual bool push()
  {
    check( middleware::tools::serializecpp::push( get_serializecpp_buffer(), arg1 ) );
    check( middleware::tools::serializecpp::push( get_serializecpp_buffer(), arg2 ) );
    return middleware::tools::serializecpp::push( get_serializecpp_buffer(), arg3 );
  }


  virtual bool pop()
  {
    check( middleware::tools::unserializecpp::pop( get_serializecpp_buffer(), arg1 ) );
    check( middleware::tools::unserializecpp::pop( get_serializecpp_buffer(), arg2 ) );
    return middleware::tools::unserializecpp::pop( get_serializecpp_buffer(), arg3 );
  }


  bool operator==(test_class1& r)
  {
    return ( memcmp( arg1, r.arg1,1024 ) == 0) && arg2==r.arg2  && arg3==r.arg3;
  }

};



/* 自定义结构体 */
void test_4()
{
  cout<< "************(自定义结构体)序列化与反序列化测试*************"<< endl;
  middleware::tools::serializecpp_buffer lsbuf(10240);
  /* 自定义序列化类 */
  test_class1 abc;
  memcpy( abc.arg1,"I love WangAiCai,But her not love me!",sizeof("I love WangAiCai,But her not love me!")+1 );
  abc.arg2 = 1234;
  int t[] = {1,2,3,4};
  std::copy( abc.arg3.begin(), abc.arg3.end(),t ); 
  ((middleware::tools::serializecpp_class*)(&abc))->push( &lsbuf );


  /* 反序列化 */
  middleware::tools::serializecpp_buffer lsbuf2(lsbuf.get_buffer(),lsbuf.get_uselen());

  test_class1 abc2;
  ((middleware::tools::serializecpp_class*)(&abc2))->pop( &lsbuf2 );

  cout<<"abc";
  cout<< ( ( abc == abc2) ? "==" : "!=");
  cout<<"abc2"<<endl;

}




int main()
{
  test_1();
  test_2();
  test_3();
  test_4();
  return 0;
}