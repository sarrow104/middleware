

#include "sql_middleware.h"






int main()
{
	connect_db_arg larg;
	larg.m_account = "root";
	larg.m_ip = "127.0.0.1";
	larg.m_dbname = "lbtest";
	larg.m_passworld = "123456";
	larg.m_port = 3306;
	
	std::string ltabname = "tab123";
	sql_middleware lsm;
	lsm.create_connect(0,ltabname,larg);

	char ch[4];
	ch[0] = 1;
	ch[1] = 2;
	ch[2] = 3;
	ch[3] = 4;

	lsm.insert(0,ltabname,0,ch,sizeof(ch));
	lsm.insert(1,ltabname,0,ch,sizeof(ch));
	lsm.insert(2,ltabname,0,ch,sizeof(ch));
	lsm.insert(3,ltabname,0,ch,sizeof(ch));
	lsm.insert(4,ltabname,0,ch,sizeof(ch));
	lsm.insert(5,ltabname,0,ch,sizeof(ch));

	char  buff[1024];
	uint32_t lsize = 1024;
	lsm.select(0,ltabname,0,buff,lsize);

	char  buff[1024];
	lsm.select_id(0,ltabname,)

	return 0;
}