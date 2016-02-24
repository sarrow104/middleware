///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/incidental/csv/csv_config.hpp"
#include "middleware/tools/incidental/csv/readconfig.hpp"
#include "middleware/tools/incidental/csv/csv_key_values.h"

namespace middleware {
	namespace tools {
	/* 初始化key config */
	fkey_config* init_key_config()
	{
		return new fkey_config();
	}

	/* 读取文件 */
	void push_read( fkey_config* ap, const char* apfile , uint32_t aikey )
	{
		ap->push( apfile , aikey );
	}


	/* 获取key指定的config文件 */
	tabkey_config* get_config( fkey_config* ap, uint32_t akey )
	{
		return &( (*ap)[akey] );
	}


	/* 获取key对应的指定行 */
	table* get_row( tabkey_config* ap, uint32_t akey )
	{
		return &( (*ap)[akey] );
	}



	/* 获取指定数据 */
	const char*  get_data( table* ap, uint32_t akey )
	{
		return  (*ap)[akey];
	}


	const char* get_data( fkey_config* ap , uint32_t akey1, uint32_t akey2, uint32_t akey3 )
	{
		tabkey_config* lp1 = get_config( ap , akey1 ) ;
		if( lp1 == NULL )
		{
			return NULL;
		}
		else
		{
			table* lp2 = get_row( lp1 , akey2);
			if( lp2 == NULL )
			{
				return NULL;
			}
			else
			{
				return get_data( lp2 , akey3 );
			}
		}
	}


	void get_data( fkey_config* ap , uint32_t akey1, uint32_t akey2, uint32_t akey3 , uint32_t& airet )
	{
		const char* lret = get_data( ap , akey1 , akey2 , akey3 );
		sscanf( lret , "%d" , &airet );
	}
	void get_data( fkey_config* ap , uint32_t akey1, uint32_t akey2, uint32_t akey3 , uint16_t& airet )
	{
		const char* lret = get_data( ap , akey1 , akey2 , akey3 );
		uint32_t linum;
		get_data( ap , akey1 , akey2 , akey3 , linum );
		airet = linum;
	}
	}  //namespace tools
}  //namespace middleware 

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */