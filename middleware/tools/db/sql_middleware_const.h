#ifndef SQL_MIDDLEWARE_CONST_H

#include <iostream>
#include <string>
#include <cstdint>
#include <list>
#include <map>

#if WIN32
# include<winsock2.h>
# define snprintf  sprintf_s
#endif //WIN32

#include <mysql.h>

struct connect_db_arg
{
	uint32_t		m_port;				/** 端口 */
	std::string		m_ip;				/** ip */
	std::string     m_account;			/** 账号 */
	std::string     m_passworld;		/** 密码 */
	std::string     m_dbname;			/** 数据库名称 */
};





#endif //SQL_MIDDLEWARE_CONST_H