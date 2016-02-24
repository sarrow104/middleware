///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/incidental/csv/readconfig.hpp"
#include "middleware/tools/incidental/csv/csv_key_values.h"



void test_csv()
{
	middleware::tools::fkey_config lconfig;
	/** windows path */
	lconfig.push("E:/gitfile/middleware/src/tools/incidental/config.csv", 0);
	std::cout << lconfig[middleware::tools::E_TEST_CONFIG][middleware::tools::CFG_HANG_UDP_PORT][middleware::tools::CFG_LIE_INTERPRETATION] << std::endl;
}


int main()
{
	test_csv();
  return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */