#include "sql_middleware.h"
#include <ctime>



sql_middleware::dbtype_map_mysql sql_middleware::m_sql;

bool sql_middleware::create_connect(dbtype_u32_key akey, std::string aitabname ,connect_db_arg& aicarg)
{
	std::pair<std::map<dbtype_u32_key,MYSQL>::iterator,bool> litor_insert = m_sql.insert( std::make_pair(akey,MYSQL()) );
	if( litor_insert.second )
	{
		MYSQL lmysql;

		if(!mysql_init(&lmysql))
		{  
			goto RETURN_FALSE;
		}

		if(!mysql_real_connect(&lmysql,aicarg.m_ip.c_str(),aicarg.m_account.c_str(),aicarg.m_passworld.c_str(),NULL,aicarg.m_port,NULL,0))  
		{     
			goto RETURN_FALSE;
		}   

		if( !sql_middleware::check_db(&lmysql,aicarg.m_dbname.c_str()) )
		{  
			goto CLOSE_MYSQL;
		}

		if( !sql_middleware::check_tab(&lmysql,aitabname.c_str()))
		{ 
			goto CLOSE_MYSQL;
		}


		

		litor_insert.first->second = lmysql;
		return true;
		
CLOSE_MYSQL:
		mysql_close(&lmysql); 
RETURN_FALSE:
		m_sql.erase(litor_insert.first);
		return false;
	}
	else
	{
		return false;
	}
}



bool sql_middleware::check_db(MYSQL *mysql,const char *db_name)  
{  
	MYSQL_ROW row = NULL;  
	MYSQL_RES *res = NULL;  

	res = mysql_list_dbs(mysql,NULL);  
	if(res)  
	{  
		while((row = mysql_fetch_row(res))!=NULL)  
		{  
			printf("db is %s\n",row[0]);  
			if(strcmp(row[0],db_name)==0)  
			{  
				printf("find db %s\n",db_name);  
				break;  
			}  
		}  
		//mysql_list_dbs会分配内存，需要使用mysql_free_result释放  
		mysql_free_result(res);  
	}  
	if(!row)  //没有这个数据库，则建立  
	{  
		char buf[128]={0};  
		strcpy(buf,"CREATE DATABASE ");  
		strcat(buf,db_name);  
#ifdef DEBUG  
		printf("%s\n",buf);  
#endif  
		if(mysql_query(mysql,buf)){  
			return false;
		}  
	}  

	mysql_select_db(mysql,db_name);
	return true;  
} 



bool sql_middleware::check_tab(MYSQL* mysql,const char *tabname)  
{  
	if(tabname == NULL)  
		return 0;  
	MYSQL_ROW row=NULL;  
	MYSQL_RES *res = NULL;  
	res = mysql_list_tables(mysql,NULL);  
	if(res)  
	{  
		while((row = mysql_fetch_row(res))!=NULL)  
		{  
			printf("tables is %s\n",row[0]);  
			if(strcmp(row[0],tabname) == 0)  
			{  
				printf("find the table !\n");  
				break;  
			}  
		}  
		mysql_free_result(res);  
	}  
	if(!row) //create table  
	{  
		char qbuf[2048]={0};  
		//"DROP TABLE IF EXISTS `%s`;"
		snprintf(qbuf,sizeof(qbuf),
			"CREATE TABLE `%s` ("
			"`id` int(11) NOT NULL,"
			"`val` blob NOT NULL,"
			"`update_time` int(11) NOT NULL,"
			"PRIMARY KEY  (`id`)"
			") ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;",tabname,tabname);
		if(mysql_query(mysql,qbuf)){  
			return false;

		}  
	}  
	return true;  
}  



MYSQL* sql_middleware::key2db(dbtype_u32_key akey)
{
	dbtype_map_mysql::iterator litor_find = m_sql.find(akey);
	if( litor_find == m_sql.end())
	{
		return NULL;
	}
	else
	{
		return &litor_find->second;
	}
}

/** 插入 */
bool sql_middleware::insert(dbtype_u32_key akey, std::string aitabname,uint32_t aid, char* aibinary,uint32_t aisize)
{
	MYSQL* lmysql = key2db( akey);
	char ltempbuf[1024];
	char lsqlbuf[4096];
	mysql_real_escape_string(lmysql,ltempbuf,aibinary,aisize);
	uint32_t lsize = 
		snprintf
		(
		lsqlbuf, 
		sizeof(lsqlbuf), 
		"INSERT INTO %s(id, val, update_time) VALUE (%d,'%s',%d); ", 
		aitabname.c_str() , 
		aid, 
		ltempbuf,
		time(NULL)
		);
	
	return (mysql_real_query(lmysql, lsqlbuf, lsize) == 0)? true : false;
}

bool sql_middleware::select(dbtype_u32_key akey, std::string aitabname,uint32_t aid, char* aibinary,uint32_t& aisize)
{
	MYSQL* lmysql = key2db( akey);

	char lsqlbuf[4096];
	uint32_t lsize = snprintf(lsqlbuf, sizeof(lsqlbuf), "SELECT val FROM %s WHERE id = %d", aitabname.c_str(),aid);
	
	if( mysql_real_query(lmysql, lsqlbuf, lsize) != 0 )
	{
		return false;
	}

	MYSQL_RES * pRes = mysql_store_result(lmysql);
	MYSQL_ROW stRow = mysql_fetch_row(pRes);
	if( pRes != NULL )
	{
		unsigned long* lengths = mysql_fetch_lengths(pRes);
		memcpy( aibinary, stRow[0], *lengths);
		aisize = *lengths;
		return true;
	}
	else
	{
		return false;
	}

}


enum
{
	E_GET_MAX,
	E_GET_MIN,
	E_GET_RAND,
	E_SIZE,
};
char* glbuf[E_SIZE] = { "ASC","DESC","RAND()"};

template <uint32_t BINART_SIZE>
bool sql_middleware::select_id(
	dbtype_u32_key akey, 
	std::string aitabname,  
	vector<SLECT_BINARY_DATA<BINART_SIZE>>& aivec,
	uint32_t aigetsize,
	uint32_t order
	)
{
	MYSQL* lmysql = key2db( akey);

	char lsqlbuf[4096];
	uint32_t lsize = snprintf(
		lsqlbuf, 
		sizeof(lsqlbuf),
		"SELECT val FROM %s ORDER BY id %s LIMIT %d;", 
		aitabname.c_str(),
		glbuf[E_GET_MAX],
		aigetsize
	);

	if( mysql_real_query(lmysql, lsqlbuf, lsize) != 0 )
	{
		return false;
	}

	MYSQL_RES* pRes = mysql_store_result(lmysql);
	MYSQL_ROW stRow = mysql_fetch_row(pRes);
	if( pRes != NULL )
	{
		unsigned long* lengths = mysql_fetch_lengths(pRes);
		memcpy( aibinary, stRow[0], *lengths);
		aisize = *lengths;
		return true;
	}
	else
	{
		return false;
	}
}

template <uint32_t BINART_SIZE>
bool sql_middleware::select_update_time(
	dbtype_u32_key akey, 
	std::string aitabname,  
	vector<SLECT_BINARY_DATA<BINART_SIZE>>& aivec,
	uint32_t aigetsize,
	uint32_t order
	)
{
	return true;
}
