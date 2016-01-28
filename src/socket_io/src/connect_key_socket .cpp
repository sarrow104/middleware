#include "connect_key_socket.h"

boost::bimap<SOCKET,connect_key_socket::key_ip_port>  connect_key_socket::m_socket_key;