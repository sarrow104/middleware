///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/incidental/csv/readconfig.hpp"

namespace middleware {
	namespace tools {

		std::ifstream tabkey_config::m_readfile;

		boost::mutex tabkey_config::m_lock;

		char table::m_segmentation = CSV_SEGMENTATION_CHAR;

	}  //namespace tools
}  //namespace middleware 
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */