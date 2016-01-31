///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/logsys/logsys.h"


namespace middleware{
  namespace tools{

    boost::mutex logsys::m_creat_lock;
    std::vector<logsys*> logsys::m_log_list;

  } //namespace tools
} //namespace middleware
