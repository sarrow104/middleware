///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_SESSION_MAPPING
#define SOCKET_SESSION_MAPPING

#include <array>

namespace middleware {

	template <typename T>
	class session_mapping
	{
		std::vector<T> m_sessionidmapp;
		uint32_t m_max_session_count;
	public:
		session_mapping():
			m_max_session_count( 0 )
		{}
		T find( uint32_t aisessionid )
		{
			if( aisessionid < m_max_session_count  )
			{
				return m_sessionidmapp[ aisessionid ];
			}
			else
			{
				return NULL;
			}
		}

		std::vector<T>& get_sessionmapp( uint32_t aisize)
		{
			m_sessionidmapp.resize( aisize );
			m_max_session_count = aisize;
			return m_sessionidmapp;
		}
	};



} //namespace middleware
#endif //SOCKET_SESSION_MAPPING

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */