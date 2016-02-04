///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/segmentation_pack/segmentation_pack.hpp"

#include <iostream>
#include <stdlib.h>

int main()
{
	class abc
	{
	public:
		void print( const char* ap)
		{
			std::cout << &(ap[sizeof(uint32_t)]) << std::endl;
		}
	};
	auto fun = [](abc* ap, char* apstr, uint32_t apstrlen)
	{
		ap->print( apstr );
		return true;
	};

	middleware::tools::segmentation_pack<abc*> lsegmentation(fun);
	abc la;
	

	/** 数据格式 = | len(sizeof(uint16_t)) | data(len) |  */
	char buff[1024000];
	char bufftemp[16];
	
	uint32_t j = 0;
	for (uint32_t i = 0; i <102400;++i)
	{
		sprintf(bufftemp, "%d", i);
		*(uint32_t*)&(buff[j]) = (uint32_t)(strlen(bufftemp) + sizeof(uint32_t) +1);
		j += sizeof(uint32_t);

		memcpy(&(buff[j]), bufftemp, strlen(bufftemp)+1);
		j += strlen(bufftemp) + 1;
		
	}

	uint32_t ltemp;
	for (int i = 0; i < int(j); )
	{
		ltemp = rand() % 100;
		if (ltemp + i > j)
		{
			ltemp = rand() % (j - i);
		}
		if (ltemp == 0)
		{
			continue;
		}
		lsegmentation.segmentation(&la, 0, &(buff[i]), ltemp);
		i += ltemp;
	}
	
	while (1)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}
	
	return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
