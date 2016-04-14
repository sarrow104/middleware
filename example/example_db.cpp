///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "sql_middleware.h"

using namespace middleware::tools;

/** 测试的结构 */
struct binary_test
{
	char m_chardata[4];
	uint32_t m_uint32;
};

void test_connect(std::string& atabname, sql_middleware& alsm)
{
	connect_db_arg larg;
	larg.m_account = "root";
	larg.m_ip = "127.0.0.1";
	larg.m_dbname = "lbtest";
	larg.m_passworld = "123456";
	larg.m_port = 3306;

	
	alsm.create_connect(0,atabname,larg);
}


void test_insert(std::string& atabname,sql_middleware& alsm)
{
	binary_test lbt;
	lbt.m_chardata[0] = 1;
	lbt.m_chardata[1] = 2;
	lbt.m_chardata[2] = 3;
	lbt.m_chardata[3] = 4;
	lbt.m_uint32 = 5;
	alsm.insert(0,atabname,1,1,&lbt,1);
	alsm.insert(0,atabname,2,1,&lbt,1);
	alsm.insert(0,atabname,3,1,&lbt,1);
	alsm.insert(0,atabname,4,1,&lbt,1);
	alsm.insert(0,atabname,5,1,&lbt,1);
	alsm.insert(0,atabname,6,1,&lbt,1);
	alsm.insert(0,atabname,4294967295,1,&lbt,1);
}

void test_select(std::string& atabname,sql_middleware& alsm)
{
	/** 按照id查找某一个 */
	SLECT_BINARY_DATA<binary_test> lvecbt2;
	uint32_t lsize;
	alsm.select(0,atabname,3,1,lvecbt2,lsize);

	/** 按照规则查找一组 */
	std::vector<SLECT_BINARY_DATA<binary_test> > lvecarr;
	alsm.select(0,
		atabname,
		E_SQLID,		/** 按照谁查找 */
		E_SORT_MAX,		/** 查找规则 */
		3,				/** 复合规则的前n个 */
		lvecarr);
}


void test_updata(std::string& atabname,sql_middleware& alsm)
{
	/** 更新一条数据 */
	SLECT_BINARY_DATA<binary_test> lvecbt3;
	lvecbt3.m_binarydata.m_chardata[0] = '1';
	lvecbt3.m_binarydata.m_chardata[1] = '9';
	lvecbt3.m_binarydata.m_chardata[2] = '8';
	lvecbt3.m_binarydata.m_chardata[3] = '9';
	lvecbt3.m_binarydata.m_uint32 = 1989;
	lvecbt3.m_id = 3;
	lvecbt3.m_update_time = 0;
	alsm.updata(0,atabname,lvecbt3);

	std::vector<SLECT_BINARY_DATA<binary_test> > lvecarr;
	alsm.select(0,
		atabname,
		E_SQLID,
		E_SORT_MAX,
		4,lvecarr);
}


void test_deletes(std::string& atabname,sql_middleware& alsm)
{
	alsm.deletes(0,atabname,4294967295,1);
	
	uint32_t arr[] = {1,3,4};
	std::vector<uint32_t > lvecarr(arr,arr+sizeof(arr)/sizeof(uint32_t));
	alsm.deletes(0,atabname,E_SQLID,1,lvecarr);


	alsm.deletes(0,atabname,E_SQLID,1,TAB_MIN_VALUES,TAB_MAX_VALUES);
}

int main()
{
	
	sql_middleware lsm;
	std::string ltabname("tab123456");
	test_connect(ltabname,lsm);
	test_insert(ltabname,lsm);
	test_select(ltabname,lsm);
	test_updata(ltabname,lsm);
	test_deletes(ltabname,lsm);
	return 0;
}