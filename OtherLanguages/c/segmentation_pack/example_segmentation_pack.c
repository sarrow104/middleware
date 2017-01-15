#include "OtherLanguages/c/segmentation_pack/segmentation_pack.h"

#include <stdio.h>



uint32_t fun(char* ap,uint32_t aplen)
{
  printf("%s\n", &(ap[sizeof(uint32_t)]));
  return 0;
}

int main()
{
  void* lp = init_segmentation_pack(&fun);
  char buff[1024000];
  char bufftemp[16];

  uint32_t j = 0;
  for (uint32_t i = 0; i <102400; ++i)
  {
    sprintf(bufftemp, "%d", i);
    *(uint32_t*)&(buff[j]) = (uint32_t)(strlen(bufftemp) + sizeof(uint32_t) + 1);
    j += sizeof(uint32_t);

    memcpy(&(buff[j]), bufftemp, strlen(bufftemp) + 1);
    j += strlen(bufftemp) + 1;

  }

  uint32_t ltemp;
  for (uint32_t i = 0; i < j; )
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
    segmentation(lp,0, &(buff[i]), ltemp);
    i += ltemp;
  }
  getchar();
  return 0;
}
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
