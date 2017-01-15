///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/tools/compress_zlip/compress_zlip.hpp"
#include <cstdio>
#include <cstdlib>

int main(int argc,char *argv[])
{ 
	compress_zlib lcompress_zlib;
	char ch1[1024] = {0};
	char ch2[1024] = { 0 };
	const char* ch = "abcdefghigklmnopqrst123456";
	int len = sizeof("abcdefghigklmnopqrst123456");
	for (int i = 0; i <= 100;++i)
	{
		ch1[i] = ch[rand() % (len-1)];
	}
	uLongf luLongf;
	lcompress_zlib.compress((Bytef*)ch1, 100, (Bytef*)ch2, luLongf);
	char ch3[1024] = { 0 };
	uLongf luLongf2;
	lcompress_zlib.uncompress((Bytef*)ch2, luLongf, (Bytef*)ch3, luLongf2);

    return(0);
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
