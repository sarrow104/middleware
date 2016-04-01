#ifndef SQL_MIDDLEWARE_H

#include "sql_middleware_const.h"


class sql_middleware
{
public:
	typedef uint32_t dbtype_u32_key;
	typedef std::map<dbtype_u32_key,MYSQL> dbtype_map_mysql;

	static bool create_connect(dbtype_u32_key akey, std::string aitabname ,connect_db_arg& aicarg);


	/** 插入 */
	bool insert(dbtype_u32_key akey, std::string aitabname,uint32_t aid, char* aibinary,uint32_t aisize);

	bool select(dbtype_u32_key akey, std::string aitabname,uint32_t aid, char* aibinary,uint32_t& aisize);

	/** 检查数据库是否存在,不存在则尝试创建 */
	static bool check_db(MYSQL *mysql,const char *db_name); 
	/** 检查数据表是否存在,不存在则尝试创建 */
	static bool check_tab(MYSQL* mysql,const char *tabname);  
private:
	static dbtype_map_mysql m_sql;

};






#endif //SQL_MIDDLEWARE_H