///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef READCONFIG_KEY_VALUES
#define READCONFIG_KEY_VALUES

namespace middleware {
	namespace tools {

		/* csv 文件对应key */
		enum CONFIG_KEY
		{
			E_TEST_CONFIG,					/* 测试 */
		};


		/* 列 */
		enum E_TEST_LIE
		{
			CFG_LIE_KEY,								/* 行数据的key */
			CFG_LIE_NAME,								/* 名字 */
			CFG_LIE_VALUES,							/* 值 */
			CFG_LIE_INTERPRETATION,			/* 解释 */
		};

		/* 行 */
		enum E_TEST_HANG
		{
			CFG_HANG_TCP_PORT,				/* TCP端口 */
			CFG_HANG_UDP_PORT = 5,				/* UDP端口 */
		};

	}  //namespace tools
}  //namespace middleware 

#endif  //READCONFIG_KEY_VALUES
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */