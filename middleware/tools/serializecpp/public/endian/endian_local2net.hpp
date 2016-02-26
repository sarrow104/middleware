///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef ENDIAN_LOCAL2NET_HPP
#define ENDIAN_LOCAL2NET_HPP

#include <cstdint>
#include <algorithm>
#include <string>

namespace middleware {
	namespace tools {
		
		class endian_local2net
		{
			static bool m_trans;
			static endian_local2net* m_this;
			endian_local2net()
			{
				uint16_t lshort = 1;
				m_trans = ( ((uint8_t*)(&lshort))[0] == 1 );
			}

			template <typename T_DATA>
			endian_local2net& operator()(T_DATA& aidata)
			{
				/** void endian() **/
				aidata.endian();
				return *this;
			}

			/** สýื้ */
			template <typename T_DATA>
			endian_local2net& operator()(T_DATA* aidata, uint32_t aplen)
			{
				for (uint32_t i = 0; i < aplen;++i)
				{
					(*this)(aidata[i]);
				}
				return *this;
			}

			endian_local2net& operator()(float& aidata)
			{
				return *this;
			}

			endian_local2net& operator()(double& aidata)
			{
				return *this;
			}

			endian_local2net& operator()(std::string& aidata)
			{
				return *this;
			}

			endian_local2net& operator()(uint8_t& aidata)
			{
				return *this;
			}

			endian_local2net& operator()(int8_t& aidata)
			{
				return *this;
			}

			endian_local2net& operator()(char& aidata)
			{
				return *this;
			}

			endian_local2net& operator()(uint16_t& aidata)
			{
				aidata = ((aidata & 0x00ff) << 8) | ((aidata & 0xff00) >> 8);
				return *this;
			}

			endian_local2net& operator()(uint32_t& aidata)
			{
				aidata = ((aidata & 0x000000ff) << 24) | \
						((aidata & 0x0000ff00) << 8) | \
						((aidata & 0x00ff0000) >> 8) | \
						((aidata & 0xff000000) >> 24);
				return *this;
			}
			endian_local2net& operator()(uint64_t& aidata)
			{
					uint32_t& l_0 = ((uint32_t*)(&aidata))[0];
					uint32_t& l_1 = ((uint32_t*)(&aidata))[1];
					this->operator()(l_0);
					this->operator()(l_1);
					std::swap(l_0, l_1);
				return *this;
			}

			endian_local2net& operator()(int16_t& aidata)
			{
				aidata = ((aidata & 0x00ff) << 8) | ((aidata & 0xff00) >> 8);
				return *this;
			}

			endian_local2net& operator()(int32_t& aidata)
			{
				aidata = ((aidata & 0x000000ff) << 24) | \
					((aidata & 0x0000ff00) << 8) | \
					((aidata & 0x00ff0000) >> 8) | \
					((aidata & 0xff000000) >> 24);
				return *this;
			}

			endian_local2net& operator()(int64_t& aidata)
			{
				uint32_t& l_0 = ((uint32_t*)(&aidata))[0];
				uint32_t& l_1 = ((uint32_t*)(&aidata))[1];
				this->operator()(l_0);
				this->operator()(l_1);
				std::swap(l_0, l_1);
				return *this;
			}

		public:
			static endian_local2net& get_single()
			{
				if (m_this == nullptr)
				{
					return *(m_this = new endian_local2net());
				}
				else
				{
					return *m_this;
				}
			}

			static void release_single()
			{
				delete m_this;
				m_this = nullptr;
			}

			template <typename T_DATA>
			endian_local2net& endian(T_DATA& aidata)
			{
				if (m_trans)
				{
					(*m_this)(aidata);
				}
				return *m_this;
			}

			template <typename T_DATA>
			endian_local2net& endian(T_DATA* aidata, uint32_t aplen)
			{
				if (m_trans)
				{
					(*m_this)(aidata, aplen);
				}
				return *m_this;
			}

			bool get_trans()
			{
				return m_trans;
			}

		};


		extern endian_local2net& gendian_local2net;

	}  //namespace tools
} //namespace middleware

#endif //ENDIAN_LOCAL2NET_HPP
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
