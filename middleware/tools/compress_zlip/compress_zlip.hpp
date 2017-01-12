///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef CONMPRESS_ZIP_H
#define CONMPRESS_ZIP_H

#include "zlib.h"

class compress_zlib
{
public:
	compress_zlib();
	bool compress(Bytef* sourcebuff, uLongf sourcelen, Bytef* targetbuff, uLongf& targetlen);
	bool uncompress(Bytef* sourcebuff, uLongf sourcelen, Bytef* targetbuff, uLongf& targetlen);
};

#endif //CONMPRESS_ZIP_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */