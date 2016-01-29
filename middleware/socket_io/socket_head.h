///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_HEAD_H
#define SOCKET_HEAD_H

#ifdef _MSC_VER
#
#include "winsock2.h"
#pragma  comment(lib,"WS2_32.LIB")
#
#else
#
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdlib>
#define INVALID_SOCKET    -1
#define SOCKET_ERROR      -1
#define SOCKET            int
#define closesocket       ::close
#define _itoa_s(a,b,c)     sprintf(b, "%d", a);
#define WSAGetLastError()  0
#
#endif //_MSC_VER

namespace middleware {

  /*
   * 包装下 socket send 函数
   */
  int g_send(SOCKET aisocket, const char* aiSendMessage, int aiSendMessageLen);
  /*
   * 包装下 socket recv 函数
   */
  int g_recv(SOCKET aisocket, char* aiRecvMessage, int aiRecvMessageLen);

} //namespace middleware 

#endif
