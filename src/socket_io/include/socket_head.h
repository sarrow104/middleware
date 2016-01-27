#ifndef SOCKET_HEAD_H
#define SOCKET_HEAD_H

#ifdef WIN32

#include "winsock2.h"
#pragma  comment(lib,"WS2_32.LIB")

#else

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCKET int
#define closesocket close
#include <cstring>
#include <cstdlib>
#define _itoa_s(a,b,c) sprintf(b, "%d", a);
#define WSAGetLastError() 0

#endif //WIN32


#define DATA_SIZE     (1024)
#define TIME_OUT_VALUES   (6000)

#define GATEWAY_LIST  (1024)

#define GATEWAY_SOCKET_CLIENT   (false)
#define GATEWAY_SOCKET_SERVER   (true)


int g_send(SOCKET aisocket , const char* aiSendMessage,int aiSendMessageLen);

int g_recv(SOCKET aisocket , char* aiRecvMessage,int aiRecvMessageLen);







#endif
