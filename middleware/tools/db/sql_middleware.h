///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SQL_MIDDLEWARE_H
#define SQL_MIDDLEWARE_H

#include "sql_middleware_const.h"

namespace middleware{
	namespace tools{

		/** 排序方式 */
		extern char* g_field_sort[E_SORT_SIZE];
		/** tab字段 */
		extern char* g_field[E_FIELD_SIZE];

		template <typename T_BINARY>
		struct SLECT_BINARY_DATA
		{
			uint32_t m_id;
			uint32_t  m_groupid;
			time_t m_update_time;
			T_BINARY m_binarydata;
		};

		class sql_middleware
		{

		public:
			typedef uint32_t dbtype_u32_key;
			typedef std::map<dbtype_u32_key,MYSQL> dbtype_map_mysql;
		private:
			bool _select(
				dbtype_u32_key akey,			//key
				std::string& aitabname,			//tabname
				dbtype_u32_key aid,					//sqlid
				dbtype_u32_key aigroupid,				//group id
				MYSQL_ROW& aibinary,			//binary data
				uint32_t& aisize			//T_BINARY所指的数组大小
				);

			bool _insert(
				dbtype_u32_key akey,
				std::string& aitabname,
				dbtype_u32_key aid, 
				dbtype_u32_key aigroupid,
				void* aibinary,
				uint32_t aisize);
			bool _select(
				dbtype_u32_key akey,			//key
				std::string aitabname,			//tabname
				ENUM_SQLFIELD aiwhere,			//以哪个字段作为排序依据
				ENUM_SORT aisort,				//排序方式
				uint32_t aisize,				//选取条目数
				std::vector<MYSQL_ROW>& aidata,
				std::vector<uint32_t>& aibinarybytes);

			bool _updata(
				dbtype_u32_key akey,			//key
				std::string aitabname,			//tabname
				uint32_t aid,
				void* apvoid,
				uint32_t asize);

			void _str2id(const char* ap,uint32_t& aid){ aid = atoi(ap); }

			void _str2uptime(const char* ap,time_t& auptime){auptime = atoi(ap);}

			void _str2groupid(const char* ap,uint32_t& agroupid){agroupid = atoi(ap);}

			template <typename T_BINARY>
			void _memcpybinary(const char* ap,T_BINARY* apbinary,uint32_t lbinarysize);

			template <typename T_BINARY>
			void _memcpybinary(T_BINARY& aioutdata,const char* aiindata,uint32_t aiindatasize);

			template <typename T_BINARY>
			void _copy2ret(SLECT_BINARY_DATA<T_BINARY>& aioutdata,MYSQL_ROW aiindata,uint32_t aiindatasize);

		public:
			static MYSQL* key2db(dbtype_u32_key akey);

			static bool create_connect(dbtype_u32_key akey, std::string& aitabname ,connect_db_arg& aicarg);

			/** 插入 */
			//template <typedef T_BINARY>
			template <typename T_BINARY>
			bool insert(dbtype_u32_key akey, std::string& aitabname,uint32_t aid, std::vector<T_BINARY>& aibinary);

			template <typename T_BINARY>
			bool insert(
				dbtype_u32_key akey, 
				std::string& aitabname,
				dbtype_u32_key aid,
				dbtype_u32_key aigroupid,
				T_BINARY* aibinary,
				uint32_t aisize);

			/** 查询 */
			template <typename T_BINARY>
			bool select(
				dbtype_u32_key akey,			//key
				std::string& aitabname,			//tabname
				dbtype_u32_key aid,					//sqlid
				dbtype_u32_key aigroupid,
				SLECT_BINARY_DATA<T_BINARY>& aibinary,			//binary data
				uint32_t& aisize					//T_BINARY所指的数组大小
				);

			/** 按照条件查询 */
			template <typename T_BINARY>
			bool select(
				dbtype_u32_key akey,			//key
				std::string aitabname,			//tabname
				ENUM_SQLFIELD aiwhere,			//以哪个字段作为排序依据
				ENUM_SORT aisort,				//排序方式
				uint32_t aisize,				//选取条目数
				std::vector<SLECT_BINARY_DATA<T_BINARY> >& aibinary	//binary data 指针
				);



			template <typename T_BINARY>
			bool updata( 
				dbtype_u32_key akey,			//key
				std::string aitabname,			//tabname
				SLECT_BINARY_DATA<T_BINARY>& apdata
				);

			bool deletes(
				dbtype_u32_key akey,			//key
				std::string aitabname,			//tabname
				dbtype_u32_key aid,
				dbtype_u32_key aigroupid
				);

			bool deletes(
				dbtype_u32_key akey,			//key
				std::string aitabname,			//tabname
				ENUM_SQLFIELD aiwhere,			//以哪个字段作为依据
				dbtype_u32_key aigroupid,
				std::vector<uint32_t> aidarr
				);

			bool deletes(
				dbtype_u32_key akey,			//key
				std::string aitabname,			//tabname
				ENUM_SQLFIELD aiwhere,			//以哪个字段作为依据
				dbtype_u32_key aigroupid,
				dbtype_u32_key abegid,
				dbtype_u32_key aendid
				);


			/** 检查数据库是否存在,不存在则尝试创建 */
			static bool check_db(MYSQL *mysql,const char *db_name); 
			/** 检查数据表是否存在,不存在则尝试创建 */
			static bool check_tab(MYSQL* mysql,const char *tabname);  
		private:
			static dbtype_map_mysql m_sql;

		};

		template <typename T_BINARY>
		bool sql_middleware::insert(dbtype_u32_key akey, std::string& aitabname,uint32_t aid, std::vector<T_BINARY>& aibinary)
		{
			return _insert( akey, aitabname, aid, aibinary.data(), aibinary.size()*sizeof(T_BINARY));
		}


		template <typename T_BINARY>
		void sql_middleware::_memcpybinary(const char* ap,T_BINARY* apbinary,uint32_t lbinarysize)
		{
			memset( apbinary, 0x0,sizeof(T_BINARY));
			memcpy( apbinary, ap,lbinarysize);
		}

		template <typename T_BINARY>
		void sql_middleware::_memcpybinary(T_BINARY& aioutdata,const char* aiindata,uint32_t aiindatasize)
		{

			uint32_t lbinarysize = (aiindatasize >= sizeof(T_BINARY) ? sizeof(T_BINARY) : aiindatasize);
			_memcpybinary(aiindata,&aioutdata,lbinarysize);
		}

		template <typename T_BINARY>
		void sql_middleware::_copy2ret(SLECT_BINARY_DATA<T_BINARY>& aioutdata,MYSQL_ROW aiindata,uint32_t aiindatasize)
		{
			_str2id(aiindata[E_SQLID],aioutdata.m_id);
			_str2uptime(aiindata[E_SQLTIME],aioutdata.m_update_time);
			_str2groupid(aiindata[E_SQLGROUPID],aioutdata.m_groupid);
			_memcpybinary(aioutdata.m_binarydata,aiindata[E_SQLBINARY],aiindatasize);
		}

		template <typename T_BINARY>
		bool sql_middleware::insert(
			dbtype_u32_key akey, 
			std::string& aitabname,
			dbtype_u32_key aid,
			dbtype_u32_key aigroupid,
			T_BINARY* aibinary,
			uint32_t aisize)
		{
			return _insert( akey, aitabname, aid, aigroupid, aibinary, aisize*sizeof(T_BINARY));
		}


		template <typename T_BINARY>
		bool sql_middleware::select(
			dbtype_u32_key akey,			//key
			std::string& aitabname,			//tabname
			dbtype_u32_key aid,					//sqlid
			dbtype_u32_key aigroupid,
			SLECT_BINARY_DATA<T_BINARY>& aibinary,			//binary data
			uint32_t& aisize					//T_BINARY所指的数组大小
			)
		{
			MYSQL_ROW lvoid;
			if( _select(akey,aitabname,aid,aigroupid,lvoid,aisize) )
			{
				_copy2ret(aibinary,lvoid,aisize);
				return true;
			}
			else
			{
				DBG_OUT("SQL FAIL = [_select() == false]\n");
				return false;
			}
		}


		/** 按照条件查询 */
		template <typename T_BINARY>
		bool sql_middleware::select(
			dbtype_u32_key akey,			//key
			std::string aitabname,			//tabname
			ENUM_SQLFIELD aiwhere,			//以哪个字段作为排序依据
			ENUM_SORT aisort,				//排序方式
			uint32_t aisize,				//选取条目数
			std::vector<SLECT_BINARY_DATA<T_BINARY> >& aibinary	//binary data 指针
			)
		{
			std::vector<MYSQL_ROW> ldata;
			std::vector<uint32_t> lbinarybytes;
			bool lret = _select(akey,aitabname,aiwhere,aisort,aisize,ldata,lbinarybytes);
			if(!lret || ldata.empty()||lbinarybytes.empty())
			{
				DBG_OUT("SQL FAIL = [_select()]\n");
				return false;
			}
			else
			{
				uint32_t lsize = ldata.size();
				aibinary.resize(lsize);
				uint32_t lbinarysize = 0;
				for (uint32_t i = 0;i<lsize;++i)
				{
					_copy2ret(aibinary[i],ldata[i],lbinarybytes[i]);
				}
				return true;
			}

		}

		template <typename T_BINARY>
		bool sql_middleware::updata( 
			dbtype_u32_key akey,			//key
			std::string aitabname,			//tabname
			SLECT_BINARY_DATA<T_BINARY>& apdata
			)
		{	
			return _updata(akey,aitabname,apdata.m_id,&apdata.m_binarydata,sizeof(T_BINARY));
		}

	} //namespace tools
} //namespace middleware
#endif //SQL_MIDDLEWARE_H