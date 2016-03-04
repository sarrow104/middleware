#ifndef SEGMENTATION_PACK_H
#define SEGMENTATION_PACK_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
  typedef uint32_t (segm_callbackfun)(char* apstr, uint32_t apstrlen);
  void* init_segmentation_pack(segm_callbackfun apfun);

#ifndef D_IP_ADDRESS_TYPE
  typedef unsigned long IP_ADDRESS_TYPE;
#endif //D_IP_ADDRESS_TYPE

  uint32_t segmentation(void* ap, IP_ADDRESS_TYPE aip, char* aidata, uint32_t aidatalen);
#ifdef __cplusplus
}
#endif //__cplusplus



#endif //SEGMENTATION_PACK_H
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
