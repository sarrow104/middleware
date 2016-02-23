#ifndef UNSERIALIZECPP_JSON_HPP
#define UNSERIALIZECPP_JSON_HPP

#include "middleware/tools/serializecpp/json/serializecpp_jsonbuffer.hpp"

#include <cstdint>

#ifndef POP_JSONPOP_BASE_TYPE
#define POP_JSONPOP_BASE_TYPE(TYPE)																																								\
		 static void pop(serializecpp_jsonbuffer& asj, const char* aikey, TYPE& aivalues)											\
		 {																																																		\
			  asj.get<TYPE>(aikey,aivalues);																																		\
		 }
#endif //POP_JSONBASE_TYPE


namespace middleware {
	namespace tools {

		int Cstr2Binary(unsigned char* szSrc, unsigned char* pDst, int nDstMaxLen)
		{
			if (szSrc == NULL)
			{
				return 0;
			}
			int iLen = strlen((char *)szSrc);
			if (iLen <= 0 || iLen % 2 != 0 || pDst == NULL || nDstMaxLen < iLen / 2)
			{
				return 0;
			}

			iLen /= 2;
			strupr((char *)szSrc);
			for (int i = 0; i<iLen; i++)
			{
				int iVal = 0;
				unsigned char *pSrcTemp = szSrc + i * 2;
				sscanf((char *)pSrcTemp, "%02x", &iVal);
				pDst[i] = (unsigned char)iVal;
			}

			return iLen;
		}


		/**
		 *  反序列化
		 */
		class unserializecpp_json
		{
		public:
			/**
			 * 自定义类型
			 */
			template <typename T_DATA>
			static void pop(serializecpp_jsonbuffer& asj, const char* aikey, T_DATA& aivalues)
			{
				aivalues.pop(asj, aikey, aivalues);
			}

			POP_JSONPOP_BASE_TYPE(uint8_t)
			POP_JSONPOP_BASE_TYPE(uint16_t)
			POP_JSONPOP_BASE_TYPE(uint32_t)
			POP_JSONPOP_BASE_TYPE(uint64_t)
			POP_JSONPOP_BASE_TYPE(float)
			POP_JSONPOP_BASE_TYPE(double)

			/**
			 *  原始数组
			 */
			template <typename T_DATA>
			static void pop(serializecpp_jsonbuffer& asj, const char* aikey, T_DATA* aivaluesarr, uint32_t& aivaluesarrsize)
			{
				boost::property_tree::ptree& lret = asj.get_child(aikey);
				std::string ldatastr;
				asj.get<std::string>("data", ldatastr);
				Cstr2Binary(ldatastr.c_str(), (unsigned char*)aivaluesarr, sizeof(T_DATA)*aivaluesarrsize)
				asj.get<std::uint32_t>("size", aivaluesarrsize);
			}

			/**
			 * vector 数组
			 */
			template <typename T_DATA>
			static void pop(serializecpp_jsonbuffer& asj, const char* aikey, std::vector<T_DATA>& aivaluesarr)
			{
				boost::property_tree::ptree& lret = asj.get_child(aikey);
				uint32_t lisize = lret.get<std::uint32_t>("size");
				aivaluesarr.resize(lisize);
				std::string ldatastr = lret.get<std::string>("data");
				Cstr2Binary((unsigned char*)ldatastr.c_str(), (unsigned char*)aivaluesarr.data(), sizeof(T_DATA)*lisize);
			}

			/**
			 * string
			 */
			static void pop(serializecpp_jsonbuffer& asj, const char* aikey, std::string& aivaluesarr)
			{
				boost::property_tree::ptree& lret = asj.get_child(aikey);
				uint32_t lisize = lret.get<std::uint32_t>("size");
				aivaluesarr.resize(lisize);
				std::string ldatastr = lret.get<std::string>("data");
				Cstr2Binary((unsigned char*)ldatastr.c_str(), (unsigned char*)aivaluesarr.data(), lisize);
			}

			/**
			 *  关联容器中的set类
			 */
			template <typename T_DATA>
			static void pop_set(serializecpp_jsonbuffer& asj, const char* aikey, T_DATA& aivaluesarr)
			{
				std::vector<T_DATA::value_type> lvec;
				unserializecpp_json::pop(asj, aikey, lvec);
				aivaluesarr.insert(lvec.begin(), lvec.end());
			}
			template <typename T_DATA>
			static void pop_map(serializecpp_jsonbuffer& asj, const char* aikey, T_DATA& aivaluesarr)
			{
				boost::property_tree::ptree& lret = asj.get_child(aikey);
				std::string lkey;
				asj.get<std::string>("key", lkey);
				std::string lval;
				asj.get<std::string>("val", lval);

				char larrkey[1024];
				char larrval[1024];
				Cstr2Binary((unsigned char*)lkey.c_str(), (unsigned char*)larrkey, sizeof(T_DATA)*lkey.length());
				Cstr2Binary((unsigned char*)lkey.c_str(), (unsigned char*)larrval, sizeof(T_DATA)*lkey.length());




				asj.get<std::uint32_t>("size", aivaluesarrsize);
			}

		};
	} //namespace tools
}// namespace middleware


#endif //UNSERIALIZECPP_JSON_HPP

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */