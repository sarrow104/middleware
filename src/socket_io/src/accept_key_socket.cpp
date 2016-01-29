#include "../include/accept_key_socket.h"

namespace middleware {

  boost::bimap<SOCKET, key_stat>  accept_key_socket::m_socket_key;

} //namespace middleware 