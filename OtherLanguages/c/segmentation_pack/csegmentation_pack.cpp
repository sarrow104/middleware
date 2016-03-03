#include "middleware/tools/segmentation_pack/segmentation_pack.hpp"
#include "OtherLanguages/c/segmentation_pack/segmentation_pack.h"

extern "C"
{
	void* init_segmentation_pack(segm_callbackfun apfun)
	{
		static middleware::tools::segmentation_pack<void*> lsegmentation(
			[apfun](void*, char* apstr, uint32_t apstrlen)
			{
				apfun(apstr, apstrlen);
				return true;
			}
		);
		return &lsegmentation;
	}

	uint32_t segmentation(void* ap, IP_ADDRESS_TYPE aip, char* aidata, uint32_t aidatalen)
	{
		return (uint32_t)static_cast<
			middleware::tools::segmentation_pack<void*>*
		>(ap)->segmentation(NULL, aip, aidata, aidatalen);
	}
}



