
#include "middleware/tools/logsys/logsys.hpp"
#include "OtherLanguages/c/logsys/logsys.h"


extern "C"
{
	uint32_t CLOG(int* LOG_ID, const char* LOG_NAME)
	{
		return (uint32_t)middleware::tools::CLOG(*LOG_ID, LOG_NAME);
	}

	uint32_t WLOG(uint32_t LOG_LEVEL, int* LOG_ID, const char* LOG_NAME, const char* ap)
	{
		return (uint32_t)middleware::tools::WLOG(LOG_LEVEL,*LOG_ID, LOG_NAME, ap);
	}

	uint32_t WPLUSH(int LOG_ID)
	{
		return (uint32_t)middleware::tools::WPLUSH(LOG_ID);
	}
}