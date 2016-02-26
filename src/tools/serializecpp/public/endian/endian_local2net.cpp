///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/serializecpp/public/endian/endian_local2net.hpp"

namespace middleware {
	namespace tools {

		bool endian_local2net::m_trans = true;
		endian_local2net* endian_local2net::m_this = nullptr;

		endian_local2net& gendian_local2net = endian_local2net::get_single();

	}  //namespace tools
}  //namespace middleware