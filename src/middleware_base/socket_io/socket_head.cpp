///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/socket_io/socket_head.hpp"

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

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
