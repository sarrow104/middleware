///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/loop_array/loop_array_middleware.hpp"

namespace middleware {

  std::map<std::string, std::pair<module_communicate*, module_communicate*> > middleware_looparray::m_module_communicate_tab;
  boost::mutex middleware_looparray::m_lock;

} //namespace middleware

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
