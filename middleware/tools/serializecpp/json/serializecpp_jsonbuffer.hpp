#ifndef SERIALIZECPP_JSON_BUFFER_HPP
#define SERIALIZECPP_JSON_BUFFER_HPP

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace middleware {
	namespace tools {
	
		enum SER_BUFF
		{
			E_BINARY,
			E_JSON,
			E_CSV,
		};


		struct serializecpp_jsonbuffer
		{
			/** 获取已使用长度*/
			inline void get_data(std::stringstream& st)
			{
				boost::property_tree::write_json(st, m_root);
			}

			serializecpp_jsonbuffer()
			{}

			~serializecpp_jsonbuffer()
			{
			}


			inline void add_node(const char* aikey,boost::property_tree::ptree& apt)
			{
				m_root.push_back(make_pair(aikey, apt));
			}

			template <typename T>
			inline void add_single(const char* aikey, const T& aivalues)
			{
				m_root.put<T>(aikey, aivalues);
			}
			
			inline void clear()
			{
				m_root.clear();
			}

			inline void reset()
			{
				clear();
			}

			/** unserializecpp */
			inline void reset(const char* ap, uint32_t aplen)
			{
				boost::property_tree::read_json<boost::property_tree::ptree>(std::stringstream(ap), m_root);
			}

			template <typename T>
			void get(const char* apkey, T& airet)
			{
				airet = m_root.get<T>(apkey);
			}
			//boost::property_tree::ptree
			template <typename T>
			void get(boost::property_tree::ptree& aptree,const char* apkey, T& aivalues)
			{
				aivalues = aptree.get<T>(aikey);
			}

			template <typename T>
			void get(const char* apkey1, const char* apkey2, T& aivalues)
			{
				aivalues = m_root.get<T>(apkey1).get<T>(apkey2);
			}

			boost::property_tree::ptree& get_child( const char* aikey )
			{
				return m_root.get_child(aikey);
			}

		private:
			boost::property_tree::ptree m_root;
		};


	} //namespace middleware
} //namespace tools

#endif //SERIALIZECPP_JSON_BUFFER_HPP
