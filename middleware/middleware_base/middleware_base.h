///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)


#inclu


namespace middleware {

	class mgt_shared_memory_server;
	class mgt_shared_memory_client;
	/**
	*  中间件
	*  用构造函数
	*  区分
	*  循环数组,
	*  共享内存,
	*  socket
	*/
	class middleware_base
	{
	public:
		virtual bool send(char* apdata, uint32_t aiwlen) = 0;
	};





}

