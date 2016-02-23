#ifndef SERIALIZACPP_XML_HPP
#define SERIALIZACPP_XML_HPP

#include "middleware/tools/serializecpp/xml/serializecpp_xmlbuffer.hpp"

#include <cstdint>


#ifndef PUSH_JSONBASE_TYPE
#define PUSH_JSONBASE_TYPE(TYPE)																																						\
		static void push(serializecpp_jsonbuffer& asj, const char* aikey, const TYPE& aivalues)							\
		{																																																		\
			asj.add_single(aikey, aivalues);																																	\
		}
#endif //PUSH_JSONBASE_TYPE


namespace middleware{
	namespace tools {

		/**
		 * 序列化
		 */
		class serializecpp_json
		{
		public:
			/**
			 * 自定义类型
			 */
			template <typename T_DATA>
			static void push(serializecpp_jsonbuffer& asj,const char* aikey, const T_DATA& aivalues)
			{
				aivalues.push(asj, aikey);
			}



			/** 基础类型 */
			PUSH_JSONBASE_TYPE(uint8_t)
			PUSH_JSONBASE_TYPE(uint16_t)
			PUSH_JSONBASE_TYPE(uint32_t)
			PUSH_JSONBASE_TYPE(uint64_t)
			PUSH_JSONBASE_TYPE(double)
			PUSH_JSONBASE_TYPE(float)


			/**
			 *  原始数组
			 */
			template <typename T_DATA>
			static void push(serializecpp_jsonbuffer& asj, const char* aikey, T_DATA* aivaluesarr, uint32_t aivaluesarrsize)
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
			static void push(serializecpp_jsonbuffer& asj, const char* aikey, vector<T_DATA>& aivaluesarr)
			{
				serializecpp_json::push(asj, aikey, aivaluesarr.data(), aivaluesarr.size());
			}

			/**
			 * std::string
			 */
			static void push(serializecpp_jsonbuffer& asj, const char* aikey, std::string& aivaluesarr)
			{
				serializecpp_json::push(asj, aikey, aivaluesarr.c_str(), aivaluesarr.size());
			}

			/**
			*  关联容器中的set类
			*/
			template <typename T_DATA>
			static void push_set(serializecpp_jsonbuffer& asj, const char* aikey, T_DATA& aivaluesarr)
			{
				std::vector<T_DATA::value_type> lvec;
				for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
				{
					lvec.push_back(*itor);
				}
				serializecpp_json::push(asj, aikey, lvec.data(), aivaluesarr.size());
			}
			template <typename T_DATA>
			static void push_map(serializecpp_jsonbuffer& asj, const char* aikey, T_DATA& aivaluesarr)
			{
				std::vector<T_DATA::value_type::first_type> lfirstvec;
				std::vector<T_DATA::value_type::second_type> lsecondvec;
				for (auto itor = aivaluesarr.begin(); itor != aivaluesarr.end(); ++itor)
				{
					lfirstvec.push_back(itor->first);
					lsecondvec.push_back(itor->second);
				}

				boost::property_tree::ptree lptree;
				lptree.put<uint32_t>("size", aivaluesarrsize);
				char lchfirstbuf[1024];
				char lchsecondbuf[1024];
				Binary2Cstr((unsigned char*)(lfirstvec.data()), aivaluesarrsize*sizeof(T_DATA), lchfirstbuf, 1024);
				Binary2Cstr((unsigned char*)(lsecondvec.data()), aivaluesarrsize*sizeof(T_DATA), lchsecondbuf, 1024);
				std::string lstr(aikey);
				lptree.put<std::string>("key", (char*)lchfirstbuf);
				lptree.put<std::string>("vel", (char*)lchsecondbuf);
				asj.add_node(aikey, lptree);
			}
		};

	} //namespace tools
}  //namespace middleware

#endif //SERIALIZACPP_XML_HPP

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
