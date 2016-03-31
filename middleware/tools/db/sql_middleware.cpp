#include "sql_middleware.h"




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

		if( sql_middleware::check_db(&lmysql,aicarg.m_dbname.c_str()) )
		{  
			goto CLOSE_MYSQL;
		}

		if( sql_middleware::check_tab(&lmysql,aitabname.c_str()))
		{ 
			goto CLOSE_MYSQL;
		}


		if(mysql_select_db(&lmysql,aicarg.m_dbname.c_str())) //return 0 is success ,!0 is err  
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
		char qbuf[128]={0};  

		snprintf(qbuf,sizeof(qbuf),
			"DROP TABLE IF EXISTS `%s`;"
			"CREATE TABLE `tab1` ("
			"`id` int(11) NOT NULL,"
			"`val` blob NOT NULL,"
			"PRIMARY KEY  (`key`)"
			") ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;",tabname);
		if(mysql_query(mysql,qbuf)){  
			return false;
		}  
	}  
	return true;  
}  





/** 插入 */
bool sql_middleware::insert(dbtype_u32_key akey, std::string aitabname,uint32_t aid, char* aibinary,uint32_t aisize)
{
	dbtype_map_mysql::iterator litor_find = m_sql.find(akey);
	if( litor_find == m_sql.end())
	{
		return false;
	}
	char ltempbuf[1024];
	char lsqlbuf[4096];
	mysql_real_escape_string(&litor_find->second,ltempbuf,aibinary,aisize);
	uint32_t lsize = snprintf(lsqlbuf, sizeof(lsqlbuf), "INSERT INTO %s(id, val) VALUE (%d,'%s'); ", aitabname.c_str() , aid, ltempbuf );
	
	return (mysql_real_query(&litor_find->second, lsqlbuf, lsize) == 0)? true : false;
}

bool sql_middleware::select(dbtype_u32_key akey, std::string aitabname,uint32_t aid, char* aibinary,uint32_t& aisize)
{
	dbtype_map_mysql::iterator litor_find = m_sql.find(akey);
	if( litor_find == m_sql.end())
	{
		return false;
	}

	char lsqlbuf[4096];
	uint32_t lsize = snprintf(lsqlbuf, sizeof(lsqlbuf), "SELECT val FROM %s WHERE id = %d", aitabname.c_str(),aid);
	
	if( mysql_real_query(&litor_find->second, lsqlbuf, lsize) != 0 )
	{
		return false;
	}

	MYSQL_RES * pRes = mysql_store_result(&litor_find->second);
	MYSQL_ROW stRow = mysql_fetch_row(pRes);
	if( pRes != NULL )
	{
		unsigned long* lengths = mysql_fetch_lengths(pRes);
		std::string ldata;

		memcpy( aibinary, stRow[0], *lengths);
		aisize = *lengths;
		return true;
	}
	else
	{
		return false;
	}

}