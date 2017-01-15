///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/compress_zlip/compress_zlip.hpp"

#pragma comment(lib, "zlib.lib")

compress_zlib::compress_zlib(){}

bool compress_zlib::compress(Bytef* sourcebuff, uLongf sourcelen, Bytef* targetbuff, uLongf& targetlen)
{
	return (::compress((Bytef*)targetbuff, &targetlen, (const Bytef*)sourcebuff, sourcelen) == Z_OK) ? true : false;
}

bool compress_zlib::uncompress(Bytef* sourcebuff, uLongf sourcelen, Bytef* targetbuff, uLongf& targetlen)
{
	return (::uncompress(targetbuff, &targetlen, sourcebuff, sourcelen) == Z_OK) ? true : false;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */