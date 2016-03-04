#ifndef LOG_SYS_H
#define LOG_SYS_H

#include "middleware/tools/logsys/log.h"
#include "OtherLanguages/c/logsys/logdef.h"

#include <inttypes.h>


#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
  /**
  *  创建日志
  */
  uint32_t CLOG(int* LOG_ID, const char* LOG_NAME);

  /**
  *  写日志
  */
  uint32_t WLOG(uint32_t LOG_LEVEL, int* LOG_ID, const char* LOG_NAME, const char* ap);

  /**
  *  像日志刷新数据
  */
  uint32_t WPLUSH(int LOG_ID);
#ifdef __cplusplus
}
#endif //__cplusplus


#endif //LOG_SYS_H

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
