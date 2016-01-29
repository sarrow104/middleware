#include "../include/socket_head.h"
#include <iostream>

namespace middleware {

  int g_send(SOCKET aisocket, const char* aiSendMessage, int aiSendMessageLen)
  {
    return send(aisocket, aiSendMessage, aiSendMessageLen, 0);
  }

  int g_recv(SOCKET aisocket, char* aiRecvMessage, int aiRecvMessageLen)
  {
    return recv(aisocket, aiRecvMessage, aiRecvMessageLen, 0);
  }
}