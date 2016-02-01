///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)
#ifndef CHECK_CRC_H
#define CHECK_CRC_H

#include <boost/crc.hpp>

#include <iostream>

namespace middleware{
/**
 *  crc相关
 */
 class check_crc
 {
 	boost::crc_32_type m_result;		/** crc 校验 */
 public:
	/**
	 *  检查crc
	 */
 	bool operator()( uint32_t aicrc_values, const char* aibuf , size_t aisize)
 	{

 		m_result.reset();
 		m_result.process_bytes(  aibuf  , aisize );
 		/**	std::cout << "crc:[" << aicrc_values <<"]["<< m_result.checksum() << "]" << std::endl; */
 		return ( aicrc_values ==  m_result.checksum() ? true : false );
 	}
	
	/**
	 *  生成
	 */
 	uint32_t operator()(  const char* aibuf , size_t aisize)
 	{
 		m_result.reset();
 		m_result.process_bytes(  aibuf  , aisize );
 		return  m_result.checksum();
 	}
 };

} //namespace middleware
#endif //CHECK_CRC_H
