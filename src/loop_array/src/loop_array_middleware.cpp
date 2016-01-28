#include "../include/loop_array_middleware.h"

std::map<std::string, std::pair<module_communicate*, module_communicate*> > middleware_looparray::m_module_communicate_tab;
boost::mutex middleware_looparray::m_lock;
