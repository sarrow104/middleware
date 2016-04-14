#ifndef SQL_MIDDLEWARE_CONST_H
#define SQL_MIDDLEWARE_CONST_H

#include <iostream>
#include <string>
#include <cstdint>
#include <list>
#include <map>
#include <vector>

#if WIN32
# include<winsock2.h>
# define snprintf  sprintf_s
#endif //WIN32

#include <mysql.h>

namespace middleware{
  namespace tools{
    struct connect_db_arg
    {
      uint32_t    m_port;       /** 端口 */
      std::string   m_ip;       /** ip */
      std::string     m_account;      /** 账号 */
      std::string     m_passworld;    /** 密码 */
      std::string     m_dbname;     /** 数据库名称 */
    };


#ifdef _DEBUG
# define DBG_OUT(...)  \
  printf(__VA_ARGS__);
#else
# define DBG_OUT(...)
#endif //DEBUG



    /** 表的字段各个字段 */
    enum ENUM_SQLFIELD
    {
      E_SQLID,      //ID
      E_SQLGROUPID,     //组id
      E_SQLBINARY,    //二进制数据
      E_SQLTIME,      //最后更新时间
      E_FIELD_SIZE,   //字段的数目
    };


    /** 排序的方式 */
    enum ENUM_SORT
    {
      E_SORT_MIN,
      E_SORT_MAX,
      E_SORT_RAND,
      E_SORT_SIZE,
    };

#define TAB_MIN_VALUES  (0x0)
#define TAB_MAX_VALUES  (0x7fffffff)

  } //namespace tools
} //namespace middleware

#endif //SQL_MIDDLEWARE_CONST_H
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
