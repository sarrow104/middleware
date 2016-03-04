///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)


#include "middleware/tools/serializecpp/mgt_serializecpp.hpp"

#include <iostream>
#include <map>

using namespace std;

/**
 *  测试基础类型
 */
void test_1( )
{
  middleware::tools::mgt_serializecpp lsbuf(1024);
  lsbuf.reset(middleware::SERIALIZE_TYPE_BINARY);
  cout<< "************基础类型序列化与反序列化测试*************"<< endl;
  /** 序列化*/
  int linum = 1023;
  lsbuf.push(linum);
  uint16_t lsnum = 123;
  lsbuf.push(lsnum);
  char lcnum = 'a';
  lsbuf.push(lcnum);
  bool lbool = true;
  lsbuf.push(lbool);

  /** 反序列化 */
  middleware::tools::mgt_serializecpp lsbufpop;
  lsbufpop.reset(middleware::SERIALIZE_TYPE_BINARY, (char*)lsbuf.get_buffer(), lsbuf.get_uselen());

  int linum2 = 0;
  lsbufpop.pop( linum2 );

  cout<< "linum=["<<linum<<"],"<<"linum2=["<< linum2<<"]"<< endl;

  uint16_t lsnum2 = 0;
  lsbufpop.pop(lsnum2);
  cout<< "lsnum=["<<lsnum<<"],"<<"lsnum2=["<< lsnum2<<"]"<< endl;

  char lcnum2 = 0;
  lsbufpop.pop(lcnum2);
  cout<< "lcnum=["<<lcnum<<"],"<<"lcnum2=["<< lcnum2<<"]"<< endl;
  
  bool lbool2 = true;
  lsbufpop.pop(lbool2);
  cout << "lbool=[" << lbool << "]," << "lbool2=[" << lbool2 << "]" << endl;
}

 /**
  *  测试结构类型(没有指针的那种结构体)
  */
void test_2()
{
  cout<< "************结构类型序列化与反序列化测试*************"<< endl;
  middleware::tools::mgt_serializecpp lsbuf(1024);
  lsbuf.reset(middleware::SERIALIZE_TYPE_BINARY);
  struct test_struct
  {
    char arg1[32];
    int  arg2;
    char arg3;
    int* arg4;
    test_struct* arg5;
    test_struct():
      arg4(nullptr),
      arg5(nullptr)
  {}

  ~test_struct()
  {
    if(arg4 != nullptr)
    {
      delete arg4;
    }
    if (arg5 != nullptr)
    {
      delete arg5;
    }
  }


  POP_FUN_ARG(
    POP_ARG_ARRAY(arg1, "arg1"),
    POP_ARG(arg2, "arg2"),
    POP_ARG(arg3, "arg3"),
    POP_ARG_PTR(arg4, "arg4"),
    POP_ARG_STRUCT_PTR(arg5, "arg5")
    )
 
    PUSH_FUN_ARG(
      PUSH_ARG_ARRAY(arg1, "arg1"),
      PUSH_ARG(arg2, "arg2"),
      PUSH_ARG(arg3, "arg3"),
      PUSH_ARG_PTR(arg4, "arg4"),
      PUSH_ARG_STRUCT_PTR(arg5, "arg5")
      )
   
  };

  struct test_struct2
  {
    char arg1[32];
    int  arg2;
    char arg3;
  
    /** 结构数组时候会被调用(如果结构体中有指针 那么不应该定义此函数) */
    ENDIAN_FUN(ENDIAN_ARG_ARRAY(arg1), ENDIAN_ARG(arg2), ENDIAN_ARG(arg3))
    
    POP_FUN_ARG(
      POP_ARG_ARRAY(arg1, "arg1"),
      POP_ARG(arg2, "arg2"),
      POP_ARG(arg3, "arg3")
      )

      PUSH_FUN_ARG(
        PUSH_ARG_ARRAY(arg1, "arg1"),
        PUSH_ARG(arg2, "arg2"),
        PUSH_ARG(arg3, "arg3")
        )
  };


  test_struct lstruct;
  memcpy( lstruct.arg1,"my name is libo",sizeof("my name is libo")+1 );
  lstruct.arg2 = 15;
  lstruct.arg3 = 'x';
  lstruct.arg4 = nullptr;
  lstruct.arg5 = new test_struct(lstruct);
  lstruct.arg5->arg5 = nullptr;
  lsbuf.push_struct( lstruct );
  /** 数组 */
  test_struct2 larray[3];
  std::string ltempstr = "z";
  for( uint32_t i = 0;i<3;++i)
  {
    ltempstr += 'z';
    memcpy( larray[i].arg1,ltempstr.c_str(),ltempstr.length()+1 );
    larray[i].arg2 = i;
    larray[i].arg3 = 'a'+i;
  }
  lsbuf.push(larray,3);


  /** 反序列化 */
  middleware::tools::mgt_serializecpp lsbufpop;
  lsbufpop.reset(middleware::SERIALIZE_TYPE_BINARY, (char*)lsbuf.get_buffer(), lsbuf.get_uselen());

  test_struct lstruct2;
  lsbufpop.pop_struct(lstruct2);
  cout<< "lstruct.arg1=["<<lstruct.arg1<<"],"<<"lstruct2.arg1=["<< lstruct2.arg1<<"]"<< endl;
  cout<< "lstruct.arg2=["<<lstruct.arg2<<"],"<<"lstruct2.arg2=["<< lstruct2.arg2<<"]"<< endl;
  cout<< "lstruct.arg3=["<<lstruct.arg3<<"],"<<"lstruct2.arg3=["<< lstruct2.arg3<<"]"<< endl;

  test_struct2 larray2[3];
  lsbufpop.pop(larray2,3);
  for( uint32_t i=0;i<3;++i)
  {
    cout<< "larray["<< i <<"].arg1=["<<larray[i].arg1<<"],"<<"larray2["<< i <<"].arg1=["<< larray2[i].arg1<<"]"<< endl;
    cout<< "larray["<< i <<"].arg2=["<<larray[i].arg2<<"],"<<"larray2["<< i <<"].arg2=["<< larray2[i].arg2<<"]"<< endl;
    cout<< "larray["<< i <<"].arg3=["<<larray[i].arg3<<"],"<<"larray2["<< i <<"].arg3=["<< larray2[i].arg3<<"]"<< endl;
  }
}

/**
*  测试结构类型(没有指针的那种结构体)
*/
void test_struct_jsonxml()
{
  cout << "************结构类型序列化与反序列化测试*************" << endl;
  middleware::tools::mgt_serializecpp lsbuf;
  lsbuf.reset(middleware::SERIALIZE_TYPE_JSON);
  struct test_struct
  {
    char arg1[32];
    int  arg2;
    char arg3;
    int* arg4;
    test_struct* arg5;
    test_struct() :
      arg4(nullptr),
      arg5(nullptr)
    {}

    ~test_struct()
    {
      if (arg4 != nullptr)
      {
        delete arg4;
      }
      if (arg5 != nullptr)
      {
        delete arg5;
      }
    }


    POP_FUN_ARG(
      POP_ARG_ARRAY(arg1, "arg1"),
      POP_ARG_PTR(arg4, "arg4"),
      POP_ARG_STRUCT_PTR(arg5, "arg5"),
      POP_ARG(arg2, "arg2"),
      POP_ARG(arg3, "arg3")
      )

      PUSH_FUN_ARG(
        PUSH_ARG_ARRAY(arg1, "arg1"),
        PUSH_ARG_PTR(arg4, "arg4"),
        PUSH_ARG_STRUCT_PTR(arg5, "arg5"),
        PUSH_ARG(arg2, "arg2"),
        PUSH_ARG(arg3, "arg3")
        )

  };

  struct test_struct2
  {
    char arg1[32];
    int  arg2;
    char arg3;

    /** 结构数组时候会被调用(如果结构体中有指针 那么不应该定义此函数) */
    ENDIAN_FUN(ENDIAN_ARG_ARRAY(arg1), ENDIAN_ARG(arg2), ENDIAN_ARG(arg3))

      POP_FUN_ARG(
        POP_ARG_ARRAY(arg1, "arg1"),
        POP_ARG(arg2, "arg2"),
        POP_ARG(arg3, "arg3")
        )

      PUSH_FUN_ARG(
        PUSH_ARG_ARRAY(arg1, "arg1"),
        PUSH_ARG(arg2, "arg2"),
        PUSH_ARG(arg3, "arg3")
        )
  };


  test_struct lstruct;
  memcpy(lstruct.arg1, "my name is libo", sizeof("my name is libo") + 1);
  lstruct.arg2 = 15;
  lstruct.arg3 = 'x';
  lstruct.arg4 = nullptr;
  lstruct.arg5 = new test_struct(lstruct);
  lstruct.arg5->arg5 = nullptr;
  lsbuf.push_struct(lstruct,"struct");
  /** 数组 */
  test_struct2 larray[3];
  std::string ltempstr = "z";
  for (uint32_t i = 0; i<3; ++i)
  {
    ltempstr += 'z';
    memcpy(larray[i].arg1, ltempstr.c_str(), ltempstr.length() + 1);
    larray[i].arg2 = i;
    larray[i].arg3 = 'a' + i;
  }
  lsbuf.push(larray, 3,"structarray");


  /** 反序列化 */
  std::cout << (char*)lsbuf.get_buffer() << std::endl;
  middleware::tools::mgt_serializecpp lsbufpop;
  lsbufpop.reset(middleware::SERIALIZE_TYPE_JSON, (char*)lsbuf.get_buffer(), lsbuf.get_uselen());

  test_struct lstruct2;
  lsbufpop.pop_struct(lstruct2, "struct");
  cout << "lstruct.arg1=[" << lstruct.arg1 << "]," << "lstruct2.arg1=[" << lstruct2.arg1 << "]" << endl;
  cout << "lstruct.arg2=[" << lstruct.arg2 << "]," << "lstruct2.arg2=[" << lstruct2.arg2 << "]" << endl;
  cout << "lstruct.arg3=[" << lstruct.arg3 << "]," << "lstruct2.arg3=[" << lstruct2.arg3 << "]" << endl;

  test_struct2 larray2[3];
  lsbufpop.pop(larray2, 3, "structarray");
  for (uint32_t i = 0; i<3; ++i)
  {
    cout << "larray[" << i << "].arg1=[" << larray[i].arg1 << "]," << "larray2[" << i << "].arg1=[" << larray2[i].arg1 << "]" << endl;
    cout << "larray[" << i << "].arg2=[" << larray[i].arg2 << "]," << "larray2[" << i << "].arg2=[" << larray2[i].arg2 << "]" << endl;
    cout << "larray[" << i << "].arg3=[" << larray[i].arg3 << "]," << "larray2[" << i << "].arg3=[" << larray2[i].arg3 << "]" << endl;
  }
}

/*
 * 测试 数组、字符串、std::string、std::vector、不支持嵌套解析容器
 */
void test_3()
{
  cout<< "**(数组 字符串  std::string std::vector  不支持嵌套解析容器)序列化与反序列化测试**"<< endl;
  middleware::tools::mgt_serializecpp lsbuf(1024);
  lsbuf.reset(middleware::SERIALIZE_TYPE_BINARY);

  char lcarray[13] = {
    'a','b','c','d','e',
    'f','g','h','i','j',
    'k','l','m'
  };
  lsbuf.push(lcarray,13);
  const char* lcstr = "helloworld";
  lsbuf.push(lcstr, sizeof("helloworld"));

  std::string lstr ="my open code!";
  lsbuf.push(lstr);

  std::vector<int> lvec(10,38);
  lsbuf.push(lvec);

  std::set<int> lset;
  for(uint32_t i = 0; i < 20 ;++i)
  {
    lset.insert( rand() );
  }
  lsbuf.push_set(lset);

  std::map<int,int> lmap;
  for(uint32_t i = 0; i < 20 ;++i)
  {
    lmap.insert( std::make_pair( rand(),rand()) );
  }
  lsbuf.push_map(lmap);

  std::unordered_set<int> lunset;
  for(uint32_t i = 0; i < 20 ;++i)
  {
    lunset.insert( rand() );
  }
  lsbuf.push_set(lunset);

  /** 反序列化 */
  middleware::tools::mgt_serializecpp lsbufpop;
  lsbufpop.reset(middleware::SERIALIZE_TYPE_BINARY, (char*)lsbuf.get_buffer(), lsbuf.get_uselen());

  char lcarray2[13];
  lsbufpop.pop( lcarray2, 13 );
  cout<<"lcarray[13]";
  cout<< ( ( memcmp( lcarray, lcarray2,13 ) == 0) ? "==" : "!=" );
  cout<<"lcarray2[13]"<<endl;

  char lcstr2[128];
 lsbufpop.pop( lcstr2, 128 );
  cout<<"lcstr";
  cout<< ( ( strcmp( lcstr, lcstr2) == 0) ? "==" : "!=");
  cout<<"lcstr2"<<endl;

  std::string lstr2;
  lsbufpop.pop(lstr2);
  cout<<"lstr";
  cout<< ( ( lstr == lstr2) ? "==" : "!=");
  cout<<"lstr2"<<endl;

  std::vector<int> lvec2;
  lsbufpop.pop(lvec2);
  cout<<"lvec";
  cout<< ( ( lvec == lvec2) ? "==" : "!=");
  cout<<"lvec2"<<endl;

  std::set<int> lset2;
  lsbufpop.pop_set(lset2);
  cout<<"lset";
  cout<< ( ( lset == lset2) ? "==" : "!=");
  cout<<"lset2"<<endl;

  std::map<int,int> lmap2;
  lsbufpop.pop_map(lmap2);
  cout<<"lmap";
  cout<< ( ( lmap == lmap2) ? "==" : "!=");
  cout<<"lmap2"<<endl;

  std::unordered_set<int> lunset2;
  lsbufpop.pop_set(lunset2);
  cout<<"lunset";
  cout<< ( ( lunset == lunset2) ? "==" : "!=");
  cout<<"lunset2"<<endl;
}

void test_xml_json_push(middleware::tools::mgt_serializecpp& lsbuf)
{
  uint32_t luint32_t = 12345;
  lsbuf.push(luint32_t, "uint32_t");

  uint32_t linitarr[] = { 1,2,3,4,5,6,7,8,9,0 };
  std::vector<uint32_t> lvec(linitarr, &(linitarr[sizeof(linitarr) / sizeof(uint32_t)]));
  lsbuf.push(lvec, "vector");

  std::string lstr = "helloworld";
  lsbuf.push(lstr, "string");
  //std::set<uint32_t> lset = {1,2,3,4,5,6,7,8,9,0,20,19,18,17};
  std::set<uint32_t> lset(linitarr, &(linitarr[sizeof(linitarr) / sizeof(uint32_t)]));
  lsbuf.push_set(lset, "set");

  std::map<int, int> lmap;
  for (int i = 1; i < 10; ++i)
  {
    lmap.insert(std::make_pair(i, -i));
  }
  lsbuf.push_map(lmap, "map");

  bool lbool = true;
  lsbuf.push(lbool,"bool");

  
}


void test_xml_json_pop(middleware::tools::mgt_serializecpp& lsbufpop)
{
  uint32_t luint32_pop;
  lsbufpop.pop(luint32_pop, "uint32_t");
  std::vector<uint32_t> lvecpop;
  lsbufpop.pop(lvecpop, "vector");
  std::string lstrpop;
  lsbufpop.pop(lstrpop, "string");
  std::set<uint32_t> lsetpop;
  lsbufpop.pop_set(lsetpop, "set");
  std::map<int, int> lmappop;
  lsbufpop.pop_map(lmappop, "map");
  bool lboolpop;
  lsbufpop.push(lboolpop, "bool");
}
/** json */
void test_5()
{
  middleware::tools::mgt_serializecpp lsbuf;
  lsbuf.reset(middleware::SERIALIZE_TYPE_JSON);
  test_xml_json_push(lsbuf);
  middleware::tools::mgt_serializecpp lsbufpop(1024);
  std::cout << lsbuf.get_buffer() << std::endl;
  lsbufpop.reset(middleware::SERIALIZE_TYPE_JSON, (char*)lsbuf.get_buffer(), lsbuf.get_uselen());
  test_xml_json_pop(lsbufpop);
}


/** xml*/
void test_6()
{
  middleware::tools::mgt_serializecpp lsbuf;
  lsbuf.reset(middleware::SERIALIZE_TYPE_XML);
  test_xml_json_push(lsbuf);
  middleware::tools::mgt_serializecpp lsbufpop(1024);
  lsbufpop.reset(middleware::SERIALIZE_TYPE_XML, (char*)lsbuf.get_buffer(), lsbuf.get_uselen());
  test_xml_json_pop(lsbufpop);
}

#include "middleware/tools/serializecpp/public/endian/endian_local2net.hpp"
void test_7()
{
  uint32_t lsize = 1234;
  middleware::tools::gendian_local2net.endian(lsize);
  middleware::tools::gendian_local2net.endian(lsize);
}

void test_8()
{
  middleware::tools::mgt_serializecpp lreadconfig(middleware::SERIALIZE_TYPE_XML,"D:/server/github/middleware/middleware/config/lp_config.xml");
  cout << lreadconfig.pop<std::string>("laname");
  
}

int main()
{
  //test_8();
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

  test_6();
  test_7();
  test_5();
  test_1();
  test_struct_jsonxml();
  test_2();
  test_3();
  return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
