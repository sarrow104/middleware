///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef XMLJSON_SERIALIZECPP_HPP
#define XMLJSON_SERIALIZECPP_HPP

#include "middleware/tools/serializecpp/json/serializecpp_jsonbuffer.hpp"
#include "middleware/tools/serializecpp/xml/serializecpp_xmlbuffer.hpp"
#include "middleware/tools/serializecpp/public/serializecpp_fun.hpp"


#ifndef PUSH_BASE_TYPE
#define PUSH_BASE_TYPE(TYPE)																																				\
		static void push(T_STAND& asj, const char* aikey, const TYPE& aivalues)							\
		{																																																		\
			asj.add_single(aikey, aivalues);																																	\
		}
#endif //PUSH_BASE_TYPE

namespace middleware {
	namespace tools {

		template <typename T_STAND>
		class serialize_stand
		{
		public:
			/**
			* 自定义类型
			*/
			template <typename T_DATA>
			static void push(T_STAND& asj, const char* aikey, const T_DATA& aivalues)
			{
				aivalues.push(asj, aikey);
			}

			/** 基础类型 */
			PUSH_BASE_TYPE(uint8_t)
			PUSH_BASE_TYPE(uint16_t)
			PUSH_BASE_TYPE(uint32_t)
			PUSH_BASE_TYPE(uint64_t)
			PUSH_BASE_TYPE(double)
			PUSH_BASE_TYPE(float)

		 /**
			*  原始数组
			*/
			template <typename T_DATA>
			static void push(T_STAND& asj, const char* aikey, T_DATA* aivaluesarr, uint32_t aivaluesarrsize)
			{
				boost::property_tree::ptree lptree;
				lptree.put<uint32_t>("size", aivaluesarrsize);
				char chbuf[1024];
				int lplen = Binary2Cstr((unsigned char*)(aivaluesarr), aivaluesarrsize*sizeof(T_DATA), (unsigned char*)chbuf, 1024);
				lptree.put<std::string>("data", chbuf);
				asj.add_node(aikey, lptree);
			}

			/**
			* vector数组
			*/
			template <typename T_DATA>
			static void push(T_STAND& asj, const char* aikey, vector<T_DATA>& aivaluesarr)
			{
				serialize_stand<T_STAND>::push(asj, aikey, aivaluesarr.data(), aivaluesarr.size());
			}

			/**
			* std::string
			*/
			static void push(T_STAND& asj, const char* aikey, std::string& aivaluesarr)
			{
				serialize_stand<T_STAND>::push(asj, aikey, aivaluesarr.c_str(), aivaluesarr.size());
			}

			/**
			*  关联容器中的set类
			*/
			template <typename T_DATA>
			static void push_set(T_STAND& asj, const char* aikey, T_DATA& aivaluesarr)
			{
				std::vector<T_DATA::value_type> lvec;
				for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
				{
					lvec.push_back(*itor);
				}
				serialize_stand<T_STAND>::push(asj, aikey, lvec.data(), aivaluesarr.size());
			}

			template <typename T_DATA>
			static void push_map(T_STAND& asj, const char* aikey, T_DATA& aivaluesarr)
			{
				std::vector<T_DATA::key_type> lfirstvec;
				std::vector<T_DATA::mapped_type> lsecondvec;
				for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
				{
					lfirstvec.push_back(itor->first);
					lsecondvec.push_back(itor->second);
				}

				boost::property_tree::ptree lptree;
				uint32_t lisize = aivaluesarr.size();
				lptree.put<uint32_t>("size", lisize);
				char lchbuf[1024] = { 0 };
				Binary2Cstr((unsigned char*)(lfirstvec.data()), lisize*sizeof(T_DATA::key_type), (unsigned char*)lchbuf, 1024);
				lptree.put<std::string>("key", (char*)lchbuf);

				Binary2Cstr((unsigned char*)(lsecondvec.data()), lisize*sizeof(T_DATA::mapped_type), (unsigned char*)lchbuf, 1024);
				lptree.put<std::string>("val", (char*)lchbuf);
				asj.add_node(aikey, lptree);
			}
		};

	} //namespace tools
} //namespace middleware 

#endif //XMLJSON_SERIALIZECPP_HPP
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */