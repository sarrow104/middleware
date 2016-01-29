#include "../include/connect_key_socket.h"

namespace middleware {

  boost::bimap<SOCKET, key_ip_port>  connect_key_socket::m_socket_key;

} //namespace middleware 