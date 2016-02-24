///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef CSV_CONFIG_H
#define CSV_CONFIG_H

#include "middleware/tools/incidental/csv/csv_key_values.h"

#include <cstdint>
#include <cstdio>

namespace middleware {
	namespace tools {
		class fkey_config;

		fkey_config* init_key_config();

		const char* get_data(fkey_config* ap, uint32_t akey1, uint32_t akey2, uint32_t akey3);
		/* ¶ÁÈ¡ÎÄ¼þ */
		void push_read(fkey_config* ap, const char* apfile, uint32_t aikey);

		void get_data(fkey_config* ap, uint32_t akey1, uint32_t akey2, uint32_t akey3, uint32_t& airet);

		void get_data(fkey_config* ap, uint32_t akey1, uint32_t akey2, uint32_t akey3, uint16_t& airet);

	}  //namespace tools
}  //namespace middleware 

#endif  //CSV_CONFIG_H
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */