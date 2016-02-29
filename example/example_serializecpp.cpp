///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/serializecpp/binary/serializecpp.hpp"
#include "middleware/tools/serializecpp/binary/unserializecpp.hpp"
#include "middleware/tools/serializecpp/xml/serializecpp_xml.hpp"
#include "middleware/tools/serializecpp/xml/unserializecpp_xml.hpp"
#include "middleware/tools/serializecpp/json/serializecpp_json.hpp"
#include "middleware/tools/serializecpp/json/unserializecpp_json.hpp"

#include <iostream>
#include <map>

using namespace std;

/**
 *  测试基础类型
 */
void test_1( )
{
  middleware::tools::serializecpp_buffer lsbuf(10240);
  cout<< "************基础类型序列化与反序列化测试*************"<< endl;
  /** 序列化*/
  int linum = 1023;
  middleware::tools::serializecpp::push( lsbuf, "", linum );
  uint16_t lsnum = 123;
  middleware::tools::serializecpp::push( lsbuf,"", lsnum );
  char lcnum = 'a';
  middleware::tools::serializecpp::push( lsbuf,"", lcnum );

  /** 反序列化 */
  middleware::tools::serializecpp_buffer lsbuf2(lsbuf.get_buffer(),lsbuf.get_uselen());

  int linum2 = 0;
  middleware::tools::unserializecpp::pop( lsbuf2,"", linum2 );

  cout<< "linum=["<<linum<<"],"<<"linum2=["<< linum2<<"]"<< endl;

  uint16_t lsnum2 = 0;
  middleware::tools::unserializecpp::pop( lsbuf2,"", lsnum2 );
  cout<< "lsnum=["<<lsnum<<"],"<<"lsnum2=["<< lsnum2<<"]"<< endl;

  char lcnum2 = 0;
  middleware::tools::unserializecpp::pop( lsbuf2,"", lcnum2 );
  cout<< "lcnum=["<<lcnum<<"],"<<"lcnum2=["<< lcnum2<<"]"<< endl;

}

 /**
  *  测试结构类型(没有指针的那种结构体)
  */
void test_2()
{
  cout<< "************结构类型(没有指针的那种结构体)序列化与反序列化测试*************"<< endl;
  middleware::tools::serializecpp_buffer lsbuf(10240);
  struct test_struct
  {
    char arg1[32];
    int  arg2;
    char arg3;
    void endian()
    {
      middleware::tools::gendian_local2net.endian(arg1,32);
      middleware::tools::gendian_local2net.endian(arg2);
      middleware::tools::gendian_local2net.endian(arg3);
    }
  };

  test_struct lstruct;
  memcpy( lstruct.arg1,"my name is libo",sizeof("my name is libo")+1 );
  lstruct.arg2 = 15;
  lstruct.arg3 = 'x';
  middleware::tools::serializecpp::push( lsbuf,"", lstruct );
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
  middleware::tools::serializecpp::push( lsbuf, "",larray , 3 );


  /** 反序列化 */
  middleware::tools::serializecpp_buffer lsbuf2(lsbuf.get_buffer(),lsbuf.get_uselen());

  test_struct lstruct2;
  middleware::tools::unserializecpp::pop( lsbuf2,"", lstruct2 );
  cout<< "lstruct.arg1=["<<lstruct.arg1<<"],"<<"lstruct2.arg1=["<< lstruct2.arg1<<"]"<< endl;
  cout<< "lstruct.arg2=["<<lstruct.arg2<<"],"<<"lstruct2.arg2=["<< lstruct2.arg2<<"]"<< endl;
  cout<< "lstruct.arg3=["<<lstruct.arg3<<"],"<<"lstruct2.arg3=["<< lstruct2.arg3<<"]"<< endl;

  test_struct larray2[3];
  middleware::tools::unserializecpp::pop( lsbuf2,"", larray2, 3 );
  for( uint32_t i=0;i<3;++i)
  {
    cout<< "larray["<< i <<"].arg1=["<<larray[i].arg1<<"],"<<"larray2["<< i <<"].arg1=["<< larray2[i].arg1<<"]"<< endl;
    cout<< "larray["<< i <<"].arg2=["<<larray[i].arg2<<"],"<<"larray2["<< i <<"].arg2=["<< larray2[i].arg2<<"]"<< endl;
    cout<< "larray["<< i <<"].arg3=["<<larray[i].arg3<<"],"<<"larray2["<< i <<"].arg3=["<< larray2[i].arg3<<"]"<< endl;

  }
}


/*
 * 测试 数组、字符串、std::string、std::vector、不支持嵌套解析容器
 */
void test_3()
{
  cout<< "**(数组 字符串  std::string std::vector  不支持嵌套解析容器)序列化与反序列化测试**"<< endl;
  middleware::tools::serializecpp_buffer lsbuf(10240);
  char lcarray[13] = {
    'a','b','c','d','e',
    'f','g','h','i','j',
    'k','l','m'
  };
  middleware::tools::serializecpp::push( lsbuf, "",lcarray , 13 );
  const char* lcstr = "helloworld";
  middleware::tools::serializecpp::push( lsbuf,"", lcstr , sizeof("helloworld") );


  std::string lstr ="my open code!";
  middleware::tools::serializecpp::push( lsbuf,"", lstr );

  std::vector<int> lvec(10,38);
  middleware::tools::serializecpp::push( lsbuf,"", lvec );

  std::set<int> lset;
  for(uint32_t i = 0; i < 20 ;++i)
  {
    lset.insert( rand() );
  }
  middleware::tools::serializecpp::push_set( lsbuf,"", lset );

  std::map<int,int> lmap;
  for(uint32_t i = 0; i < 20 ;++i)
  {
    lmap.insert( std::make_pair( rand(),rand()) );
  }
  middleware::tools::serializecpp::push_map( lsbuf,"", lmap );

  std::unordered_set<int> lunset;
  for(uint32_t i = 0; i < 20 ;++i)
  {
    lunset.insert( rand() );
  }
  middleware::tools::serializecpp::push_set( lsbuf,"", lunset );

  /** 反序列化 */
  middleware::tools::serializecpp_buffer lsbuf2(lsbuf.get_buffer(),lsbuf.get_uselen());

  char lcarray2[13];
  middleware::tools::unserializecpp::pop( lsbuf2, "",lcarray2, 13 );
  cout<<"lcarray[13]";
  cout<< ( ( memcmp( lcarray, lcarray2,13 ) == 0) ? "==" : "!=" );
  cout<<"lcarray2[13]"<<endl;

  char lcstr2[128];
  middleware::tools::unserializecpp::pop( lsbuf2, "",lcstr2, 128 );
  cout<<"lcstr";
  cout<< ( ( strcmp( lcarray, lcarray2) == 0) ? "==" : "!=");
  cout<<"lcstr2"<<endl;

  std::string lstr2;
  middleware::tools::unserializecpp::pop( lsbuf2,"", lstr2 );
  cout<<"lstr";
  cout<< ( ( lstr == lstr2) ? "==" : "!=");
  cout<<"lstr2"<<endl;

  std::vector<int> lvec2;
  middleware::tools::unserializecpp::pop( lsbuf2,"", lvec2 );
  cout<<"lvec";
  cout<< ( ( lvec == lvec2) ? "==" : "!=");
  cout<<"lvec2"<<endl;

  std::set<int> lset2;
  middleware::tools::unserializecpp::pop( lsbuf2,"", lset2 );
  cout<<"lset";
  cout<< ( ( lset == lset2) ? "==" : "!=");
  cout<<"lset2"<<endl;

  std::map<int,int> lmap2;
  middleware::tools::unserializecpp::pop( lsbuf2,"", lmap2 );
  cout<<"lmap";
  cout<< ( ( lmap == lmap2) ? "==" : "!=");
  cout<<"lmap2"<<endl;

  std::unordered_set<int> lunset2;
  middleware::tools::unserializecpp::pop( lsbuf2,"", lunset2 );
  cout<<"lunset";
  cout<< ( ( lunset == lunset2) ? "==" : "!=");
  cout<<"lunset2"<<endl;
}



/** json */
void test_5()
{
  middleware::tools::serializecpp_jsonbuffer lsbuf;
  uint32_t luint32_t = 12345;
  middleware::tools::serializecpp_json::push(lsbuf,"uint32_t",luint32_t);
  //std::vector<uint32_t> lvec = {1,2,3,4,5,6,7,8,9,0};
  uint32_t linitarr[] = { 1,2,3,4,5,6,7,8,9,0 };
  std::vector<uint32_t> lvec(linitarr, &(linitarr[sizeof(linitarr) / sizeof(uint32_t)]));
  middleware::tools::serializecpp_json::push(lsbuf, "vector", lvec);
  std::string lstr = "helloworld";
  middleware::tools::serializecpp_json::push(lsbuf, "string", lstr);
  //std::set<uint32_t> lset = {1,2,3,4,5,6,7,8,9,0,20,19,18,17};
  std::set<uint32_t> lset(linitarr, &(linitarr[sizeof(linitarr) / sizeof(uint32_t)]));
  middleware::tools::serializecpp_json::push_set(lsbuf, "set", lset);
  std::map<int, int> lmap;
  for (int i = 1; i < 10; ++i)
  {
    lmap.insert(std::make_pair(i,-i));
  }
  middleware::tools::serializecpp_json::push_map(lsbuf, "map", lmap);


  middleware::tools::serializecpp_jsonbuffer lsbufpop;
  lsbufpop.reset(lsbuf.get_buffer(), lsbuf.get_uselen());
  uint32_t luint32_pop;
  middleware::tools::unserializecpp_json::pop(lsbufpop, "uint32_t", luint32_pop);
  std::vector<uint32_t> lvecpop;
  middleware::tools::unserializecpp_json::pop(lsbufpop, "vector", lvecpop);
  std::string lstrpop;
  middleware::tools::unserializecpp_json::pop(lsbufpop, "string", lstrpop);
  std::set<uint32_t> lsetpop;
  middleware::tools::unserializecpp_json::pop_set(lsbufpop, "set", lsetpop);
  std::map<int, int> lmappop;
  middleware::tools::unserializecpp_json::pop_map(lsbufpop, "map", lmappop);
}


/** xml*/
void test_6()
{
  middleware::tools::serializecpp_xmlbuffer lsbuf;
  uint32_t luint32_t = 12345;
  middleware::tools::serializecpp_xml::push(lsbuf, "uint32_t", luint32_t);
  //std::vector<uint32_t> lvec = {1,2,3,4,5,6,7,8,9,0};
  uint32_t linitarr[] = { 1,2,3,4,5,6,7,8,9,0 };
  std::vector<uint32_t> lvec(linitarr, &(linitarr[sizeof(linitarr) / sizeof(uint32_t)]));
  middleware::tools::serializecpp_xml::push(lsbuf, "vector", lvec);
  std::string lstr = "helloworld";
  middleware::tools::serializecpp_xml::push(lsbuf, "string", lstr);
  //std::set<uint32_t> lset = {1,2,3,4,5,6,7,8,9,0,20,19,18,17};
  std::set<uint32_t> lset(linitarr, &(linitarr[sizeof(linitarr) / sizeof(uint32_t)]));
  middleware::tools::serializecpp_xml::push_set(lsbuf, "set", lset);
  std::map<int, int> lmap;
  for (int i = 1; i < 10; ++i)
  {
    lmap.insert(std::make_pair(i, -i));
  }
  middleware::tools::serializecpp_xml::push_map(lsbuf, "map", lmap);

  middleware::tools::serializecpp_xmlbuffer lsbufpop;
  lsbufpop.reset(lsbuf.get_buffer(), lsbuf.get_uselen());
  uint32_t luint32_pop;
  middleware::tools::unserializecpp_xml::pop(lsbufpop, "uint32_t", luint32_pop);
  std::vector<uint32_t> lvecpop;
  middleware::tools::unserializecpp_xml::pop(lsbufpop, "vector", lvecpop);
  std::string lstrpop;
  middleware::tools::unserializecpp_xml::pop(lsbufpop, "string", lstrpop);
  std::set<uint32_t> lsetpop;
  middleware::tools::unserializecpp_xml::pop_set(lsbufpop, "set", lsetpop);
  std::map<int, int> lmappop;
  middleware::tools::unserializecpp_xml::pop_map(lsbufpop, "map", lmappop);
}

#include "middleware/tools/serializecpp/public/endian/endian_local2net.hpp"
void test_7()
{
  uint32_t lsize = 1234;
  middleware::tools::gendian_local2net.endian(lsize);
  middleware::tools::gendian_local2net.endian(lsize);
}
#include "middleware/tools/serializecpp/mgt_serializecpp.hpp"
int main()
{
  /** 为何三值均不相等 */
  std::cout<< typeid(int8_t).hash_code() <<","<< typeid(uint8_t).hash_code() <<"," << typeid(char).hash_code();
  middleware::tools::mgt_serializecpp ltemp(1024);
  middleware::tools::mgt_serializecpp luntemp;
  ltemp.reset(0);
  int linum = 1023;
  int linumpop = 0;

  ltemp.push(linum);
  luntemp.reset(0, (char*)ltemp.get_buffer(), ltemp.get_uselen());
  linumpop = 0;
  luntemp.pop(linumpop);

  ltemp.reset(1);
  ltemp.push(linum,"num");
  luntemp.reset(1, (char*)ltemp.get_buffer(), ltemp.get_uselen());
  linumpop = 0;
  luntemp.pop(linumpop,"num");

  ltemp.reset(2);
  ltemp.push(linum, "num");
  luntemp.reset(2, (char*)ltemp.get_buffer(), ltemp.get_uselen());
  linumpop = 0;
  luntemp.pop(linumpop,"num");


  test_7();
  test_6();
  test_5();
  test_1();
  test_2();
  test_3();
  return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
