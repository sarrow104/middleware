

#include "sql_middleware.h"


struct binary_test
{
	char m_chardata[4];
	uint32_t m_uint32;
};


int main()
{
	connect_db_arg larg;
	larg.m_account = "root";
	larg.m_ip = "127.0.0.1";
	larg.m_dbname = "lbtest";
	larg.m_passworld = "123456";
	larg.m_port = 3306;
	
	std::string ltabname = "tab123456";
	sql_middleware lsm;
	lsm.create_connect(0,ltabname,larg);
	
	
	binary_test lbt;
	lbt.m_chardata[0] = 1;
	lbt.m_chardata[1] = 2;
	lbt.m_chardata[2] = 3;
	lbt.m_chardata[3] = 4;
	lbt.m_uint32 = 5;

	lsm.insert(0,ltabname,1,&lbt,1);
	lsm.insert(0,ltabname,2,&lbt,1);
	lsm.insert(0,ltabname,3,&lbt,1);
	lsm.insert(0,ltabname,4,&lbt,1);
	lsm.insert(0,ltabname,5,&lbt,1);
	lsm.insert(0,ltabname,6,&lbt,1);

	SLECT_BINARY_DATA<binary_test> lvecbt2;
	uint32_t lsize;
	lsm.select(0,ltabname,3,lvecbt2,lsize);
	//char  buff[1024];
	//uint32_t lsize = 1024;
	//lsm.select(0,ltabname,0,buff,lsize);
//
	//char  buff[1024];
	//lsm.select_id(0,ltabname,)

	return 0;
}