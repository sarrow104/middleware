#include "sql_middleware.h"
#include <ctime>


/** 排序方式 */
char* g_field_sort[E_SORT_SIZE] = { "ASC","DESC","RAND()"};
/** tab字段 */
char* g_field[E_FIELD_SIZE] = {"s_id","s_val","s_uptime"};

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
			DBG_OUT("db is = [%s]\n",row[0]);  
			if(strcmp(row[0],db_name)==0)  
			{  
				DBG_OUT("find db = [%s]\n",db_name);  
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
		DBG_OUT("SQL STR = [%s]\n",buf);  
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
			DBG_OUT("tab is = [%s]\n",row[0]);  
			if(strcmp(row[0],tabname) == 0)  
			{  
				DBG_OUT("find tab = [%s]\n",tabname);  
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
			"`%s` int(11) NOT NULL,"
			"`%s` blob NOT NULL,"
			"`%s` int(11) NOT NULL,"
			"PRIMARY KEY  (`%s`)"
			") ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;",
			tabname,
			g_field[E_SQLID],
			g_field[E_SQLBINARY],
			g_field[E_SQLTIME],
			g_field[E_SQLID]
			);

		DBG_OUT("SQL STR = [%s]\n",qbuf); 

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
bool sql_middleware::_insert(dbtype_u32_key akey, std::string aitabname,uint32_t aid, void* aibinary,uint32_t aisize)
{
	MYSQL* lmysql = key2db( akey);
	char ltempbuf[1024];
	char lsqlbuf[4096];
	mysql_real_escape_string(lmysql,ltempbuf,(const char*)aibinary,aisize);
	uint32_t lsize = 
		snprintf
		(
		lsqlbuf, 
		sizeof(lsqlbuf), 
		"INSERT INTO %s(%s, %s, %s) VALUE (%d,'%s',%d); ", 
		aitabname.c_str(), 
		g_field[E_SQLID],
		g_field[E_SQLBINARY],
		g_field[E_SQLTIME],
		aid, 
		ltempbuf,
		time(NULL)
		);

	DBG_OUT("SQL STR = [%s]\n",lsqlbuf); 

	return (mysql_real_query(lmysql, lsqlbuf, lsize) == 0)? true : false;
}

bool sql_middleware::_select(
	dbtype_u32_key akey,			//key
	std::string& aitabname,			//tabname
	uint32_t aid,					//sqlid
	MYSQL_ROW& aibinary,			//binary data
	uint32_t& aisize					//T_BINARY所指的数组大小
	)
{
	MYSQL* lmysql = key2db( akey);

	char lsqlbuf[4096];
	uint32_t lsize = snprintf(
		lsqlbuf, 
		sizeof(lsqlbuf), 
		"SELECT %s, %s, %s FROM %s WHERE %s = %d",
		g_field[E_SQLID],
		g_field[E_SQLBINARY],
		g_field[E_SQLTIME],
		aitabname.c_str(),
		g_field[E_SQLID],
		aid);

	DBG_OUT("SQL STR = [%s]\n",lsqlbuf); 

	if( mysql_real_query(lmysql, lsqlbuf, lsize) != 0 )
	{
		DBG_OUT("SQL FAIL = [mysql_real_query]\n");
		return false;
	}

	MYSQL_RES* pRes = mysql_store_result(lmysql);
	if( pRes != NULL )
	{
		aibinary = mysql_fetch_row(pRes);
		//unsigned long* lengths = mysql_fetch_lengths(pRes);
		aisize = mysql_fetch_lengths(pRes)[E_SQLBINARY];
		
		return true;
	}
	else
	{
		DBG_OUT("SQL FAIL = [mysql_store_result(lmysql) == NULL]\n");
		return false;
	}

}

bool sql_middleware::_select(
	dbtype_u32_key akey,			//key
	std::string aitabname,			//tabname
	ENUM_SQLFIELD aiwhere,			//以哪个字段作为排序依据
	ENUM_SORT aisort,				//排序方式
	uint32_t aisize,				//选取条目数
	std::vector<MYSQL_ROW>& aidata,
	std::vector<uint32_t>& aibinarybytes
	)
{
	MYSQL* lmysql = key2db( akey);

	char lsqlbuf[4096];
	uint32_t lsize = snprintf(
		lsqlbuf, 
		sizeof(lsqlbuf),
		"SELECT s_id,s_val,s_uptime FROM %s ORDER BY %s %s LIMIT %d;", 
		aitabname.c_str(),
		g_field[aiwhere],
		g_field_sort[aisort],
		aisize
		);

	DBG_OUT("SQL STR = [%s]\n",lsqlbuf); 

	if( mysql_real_query(lmysql, lsqlbuf, lsize) != 0 )
	{
		return false;
	}


	MYSQL_RES* pRes = mysql_store_result(lmysql);//集合
	if( pRes )
	{
		uint32_t liTableRow = (uint32_t)mysql_num_rows(pRes);//行  
		uint32_t liTableCol = (uint32_t)mysql_num_fields(pRes);//列  
		aidata.resize(liTableRow);
		aibinarybytes.resize(liTableRow);
		for(uint32_t i=0; i<liTableRow; i++)
		{
			aidata[i] = mysql_fetch_row(pRes);
			aibinarybytes[i] = mysql_fetch_lengths(pRes)[E_SQLBINARY];
			//for(uint32_t j=0; j<liTableCol; j++)
			//{
			//	DBG_OUT("SQL STR = [%s]\n",stRow[j]); 
			//}
			//DBG_OUT("\n");
		}
		return true;
	}
	else
	{
		return false;
	}
}


