#ifndef SERIALIZACPP_FUN_HPP
#define SERIALIZACPP_FUN_HPP


namespace middleware {
  namespace tools {
    
    int Binary2Cstr(unsigned char *pSrc, int nSrcLen, unsigned char *pDst, int nDstMaxLen);

    int Cstr2Binary(unsigned char* szSrc, unsigned char* pDst, int nDstMaxLen);
  
  }
}

#endif //SERIALIZACPP_FUN_HPP
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
