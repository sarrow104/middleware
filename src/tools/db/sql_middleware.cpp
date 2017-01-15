///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/db/sql_middleware.h"
#include <ctime>
#include <string.h>

namespace middleware{
  namespace tools{

    const char* _select_limt_str()
    {
      static char lsqlbuf[4096];
      static uint32_t lsize = snprintf(
        lsqlbuf, 
        sizeof(lsqlbuf),
        "SELECT %s, %s, %s, %s FROM %s ORDER BY %s",
        g_field[E_SQLID],
        g_field[E_SQLGROUPID],
        g_field[E_SQLBINARY],
        g_field[E_SQLTIME],
        "%s",
        "%s %s LIMIT %d;"
        );
      return lsqlbuf;
    }

    const char* _insert_str()
    {
      static char ltempbuf[256];
      static uint32_t lsize = 
        snprintf
        (
          ltempbuf, 
          sizeof(ltempbuf), 
          "INSERT INTO %s(%s, %s, %s, %s) VALUE %s; ", 
          "%s",
          g_field[E_SQLID],
          g_field[E_SQLGROUPID],
          g_field[E_SQLBINARY],
          g_field[E_SQLTIME],
          "(%d,%d,'%s',%d)"
        );
      return ltempbuf;
    }

    const char* _select_str()
    {
      static char lsqlbuf[256];
      static uint32_t lsize = snprintf(
        lsqlbuf, 
        sizeof(lsqlbuf), 
        "SELECT %s, %s, %s, %s FROM %s WHERE %s",
        g_field[E_SQLID],
        g_field[E_SQLGROUPID],
        g_field[E_SQLBINARY],
        g_field[E_SQLTIME],
        "%s",
        "%s = %d AND %s = %d"
        );
      return lsqlbuf;
    }


    const char* _updata_str()
    {
      static char lsqlbuf[4096];
      static uint32_t lsize = snprintf(
        lsqlbuf, 
        sizeof(lsqlbuf),
        "UPDATE %s SET %s = '%s', %s = '%s' WHERE %s = '%s'",
        "%s",
        g_field[E_SQLBINARY],
        "%s",
        g_field[E_SQLTIME],
        "%d",
        g_field[E_SQLID],
        "%d"
        );
      return lsqlbuf;
    }

    const char* _deletes_str()
    {
      static char lsqlbuf[256];
      static uint32_t lsize = 
        snprintf
        (
          lsqlbuf, 
          sizeof(lsqlbuf), 
          "DELETE FROM %s WHERE %s = %s AND %s = %s;", 
          "%s", 
          g_field[E_SQLID],
          "%d",
          g_field[E_SQLGROUPID],
          "%d"
        );
      return lsqlbuf;
    }

    const char* _deletes_more_str()
    {
      static char lsqlbuf[256];
      static uint32_t lsize = 
        snprintf
        (
          lsqlbuf, 
          sizeof(lsqlbuf), 
          "DELETE FROM %s WHERE %s = %s AND %s in ", 
          "%s", 
          g_field[E_SQLGROUPID],
          "%d"    
        );
      return lsqlbuf;
    }

    const char* _deletes_blank_str()
    {
      static char lsqlbuf[256];
      static uint32_t lsize = 
        snprintf
        (
          lsqlbuf, 
          sizeof(lsqlbuf), 
          "DELETE FROM %s WHERE %s = %s AND %s > %s AND %s < %s;", 
          "%s", 
          g_field[E_SQLGROUPID],
          "%d",
          "%s",
          "%d",
          "%s",
          "%d"
        );
      return lsqlbuf;
    }

    /** 排序方式 */
    char* g_field_sort[E_SORT_SIZE] = { "ASC","DESC","RAND()"};
    /** tab字段 */
    char* g_field[E_FIELD_SIZE] = {"s_id","s_groupid","s_val","s_uptime"};

    sql_middleware::dbtype_map_mysql sql_middleware::m_sql;

    bool sql_middleware::create_connect(dbtype_u32_key akey, std::string& aitabname ,connect_db_arg& aicarg)
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
                 "`%s` tinyint NOT NULL,"
                 "`%s` blob NOT NULL,"
                 "`%s` int(11) NOT NULL,"
                 "PRIMARY KEY  (`%s`)"
                 ") ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;",
                 tabname,
                 g_field[E_SQLID],
                 g_field[E_SQLGROUPID],
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
    bool sql_middleware::_insert(
      dbtype_u32_key akey, 
      std::string& aitabname,
      dbtype_u32_key aid,
      dbtype_u32_key aigroupid, 
      void* aibinary,
      uint32_t aisize
      )
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
          _insert_str(),
          aitabname.c_str(),
          aid, 
          aigroupid,
          ltempbuf,
          time(NULL)
        );

      DBG_OUT("SQL STR = [%s]\n",lsqlbuf); 

      return (mysql_real_query(lmysql, lsqlbuf, lsize) == 0)? true : false;
    }



    bool sql_middleware::_select(
      dbtype_u32_key akey,      //key
      std::string& aitabname,     //tabname
      dbtype_u32_key aid,         //sqlid
      dbtype_u32_key aigroupid,       //group id
      MYSQL_ROW& aibinary,      //binary data
      uint32_t& aisize          //T_BINARY所指的数组大小
      )
    {
      MYSQL* lmysql = key2db( akey);

      char lsqlbuf[4096];
      uint32_t lsize = snprintf(
        lsqlbuf, 
        sizeof(lsqlbuf), 
        _select_str(),
        aitabname.c_str(),
        g_field[E_SQLID],
        aid,
        g_field[E_SQLGROUPID],
        aigroupid
        );

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
      dbtype_u32_key akey,      //key
      std::string aitabname,      //tabname
      ENUM_SQLFIELD aiwhere,      //以哪个字段作为排序依据
      ENUM_SORT aisort,       //排序方式
      uint32_t aisize,        //选取条目数
      std::vector<MYSQL_ROW>& aidata,
      std::vector<uint32_t>& aibinarybytes
      )
    {
      MYSQL* lmysql = key2db( akey);

      char lsqlbuf[4096];
      uint32_t lsize = snprintf(
        lsqlbuf, 
        sizeof(lsqlbuf),
        _select_limt_str(),
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
          //  DBG_OUT("SQL STR = [%s]\n",stRow[j]); 
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



    bool sql_middleware::_updata(
      dbtype_u32_key akey,      //key
      std::string aitabname,      //tabname
      uint32_t aid,
      void* apvoid,
      uint32_t asize)
    {
      MYSQL* lmysql = key2db( akey);
      char ltempbuf[1024];
      mysql_real_escape_string(lmysql,ltempbuf,(const char*)(apvoid),asize);

      char lsqlbuf[4096];
      uint32_t lsize = snprintf(
        lsqlbuf, 
        sizeof(lsqlbuf),
        _updata_str(),
        aitabname.c_str(),
        ltempbuf,
        (uint32_t)time(NULL),
        aid
        );

      DBG_OUT("SQL STR = [%s]\n",lsqlbuf); 

      return (mysql_real_query(lmysql, lsqlbuf, lsize) != 0);
    }


    bool sql_middleware::deletes(
      dbtype_u32_key akey,      //key
      std::string aitabname,      //tabname
      dbtype_u32_key aid,
      dbtype_u32_key aigroupid
      )
    {
      MYSQL* lmysql = key2db( akey);
      char lsqlbuf[4096];
      uint32_t lsize = 
        snprintf
        (
          lsqlbuf, 
          sizeof(lsqlbuf), 
          _deletes_str(), 
          aitabname.c_str(), 
          aid,
          aigroupid
        );

      DBG_OUT("SQL STR = [%s]\n",lsqlbuf); 
      return (mysql_real_query(lmysql, lsqlbuf, lsize) == 0)? true : false;
    }


    bool sql_middleware::deletes(
      dbtype_u32_key akey,      //key
      std::string aitabname,      //tabname
      ENUM_SQLFIELD aiwhere,      //以哪个字段作为依据
      dbtype_u32_key aigroupid, 
      std::vector<uint32_t> aidarr
      )
    {
      MYSQL* lmysql = key2db( akey);
      char lsqlbuf[4096];
      uint32_t lsize = 
        snprintf
        (
          lsqlbuf, 
          sizeof(lsqlbuf), 
          _deletes_more_str(), 
          aitabname.c_str(), 
          aigroupid
        );

      char ltempbuf[1024] = {0};
      uint32_t lpos = 1;
      ltempbuf[0] = '(';
      for(std::vector<uint32_t>::iterator itor = aidarr.begin(); itor != aidarr.end();++itor)
      {
        lpos += sprintf( &(ltempbuf[lpos]),"%d",*itor);
        ltempbuf[lpos] = ',';
        lpos += 1;
      }
      ltempbuf[lpos-1] = ')';
      ltempbuf[lpos] = ';';

      strcat(lsqlbuf,ltempbuf);
      //char ltempbuf2[2048];
      //sprintf(ltempbuf2,lsqlbuf,ltempbuf);

      DBG_OUT("SQL STR = [%s]\n",lsqlbuf); 
      return (mysql_real_query(lmysql, lsqlbuf, lpos+lsize) == 0)? true : false;
    }

    bool sql_middleware::deletes(
      dbtype_u32_key akey,      //key
      std::string aitabname,      //tabname
      ENUM_SQLFIELD aiwhere,      //以哪个字段作为依据
      dbtype_u32_key aigroupid,
      dbtype_u32_key abegid,
      dbtype_u32_key aendid
      )
    {
      MYSQL* lmysql = key2db( akey);
      char lsqlbuf[4096];
      uint32_t lsize = 
        snprintf
        (
          lsqlbuf, 
          sizeof(lsqlbuf), 
          _deletes_blank_str(), 
          aitabname.c_str(),
          aigroupid,
          g_field[aiwhere],
          abegid,
          g_field[aiwhere],
          aendid
        );

      DBG_OUT("SQL STR = [%s]\n",lsqlbuf); 
      return (mysql_real_query(lmysql, lsqlbuf, lsize) == 0)? true : false;
    }

  } //namespace tools
} //namespace middleware
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
