
#include "middleware/middleware_base/middleware_base.hpp"


#include <cstdint>
#include <unordered_map>
#include <vector>


namespace middleware {
	namespace tools {
	

		template <typename T_PHP1, typename T_PHP2>
		class protocol_base
		{
			static std::unordered_map<uint32_t, protocol_base* > m_promap;
			protocol_base()
			{}
			protocol_base(const protocol_base&);

		protected:
			unpack_head_process<T_PHP1>* m_premote2local;
			pack_head_process<T_PHP2>* m_plocal2remote;

			virtual bool task(uint32_t aikey) = 0;
			virtual void serialization() = 0;
			virtual const char* unserialization() = 0;
			virtual protocol_base* new_own() = 0;
		public:
			protocol_base(uin32_t aiprotocolnum):
				m_premote2local(nullptr),
				m_plocal2remote(nullptr)
			{
				auto itor = m_promap.find(aiprotocolnum);
				if ( itor != m_promap.end())
				{
					itor->sesond->m_premote2local
				}
				else
				{
					m_promap.insert( std::make_pair(aiprotocolnum,this) );
				}
			}

			void set_pack_head_process(unpack_head_process<T_PHP1>* apremote2local, pack_head_process<T_PHP2>* aplocal2remote)
			{
				m_premote2local = apremote2local;
				m_plocal2remote = aplocal2remote;
			}


			static protocol_base<T_PHP1, T_PHP2>* new_protocol_base_map( uint32_t aimax, unpack_head_process<T_PHP1>* apremote2local, pack_head_process<T_PHP2>* aplocal2remote)
			{
				static uint32_t lcount = 1;
				std::unordered_map<uint32_t, protocol_base* >* lret = nullptr;
				if (lcount == 1)
				{
					lret = new std::unordered_map<uint32_t, protocol_base* >();
				}
				else if(lcount <= aimax)
				{
					for (auto itor = m_promap.begin(); itor != m_promap.end(); ++itor)
					{
						itor->sesond = itor->sesond->new_own();
					}
				}
				else
				{
					return nullptr;
				}

				for (auto itor = m_promap.begin(); itor != m_promap.end();++itor)
				{
					itor->sesond->set_pack_head_process(apremote2local, aplocal2remote);
				}

				if (lret != nullptr)
				{
					*lret = m_promap;
				}
				else if (lcount == aimax)
				{
					lret = &m_promap;
				}
				else
				{
					lret = new std::unordered_map<uint32_t, protocol_base* >();
					*lret = m_promap;
				}

				++lcount;
				return lret;
			}

			/**
			*  回调,服务器没有aikey 可以用bind随便绑定一个值
			*/
			bool run_task(uint32_t aikey)
			{
				unserialization();
				task();
				serialization();
			}



		};

		template <typename T_PHP1, typename T_PHP2>
		class mgt_protocol
		{
			std::vector< std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>*>* > m_promap_arr;
			std::vector< unpack_head_process<T_PHP1>* > m_premote2local_arr;
			std::vector< pack_head_process<T_PHP2>* > m_plocal2remote_arr;
		public:
			mgt_protocol( uint32_t aimaxthreadnum, uint32_t aieverybytes, std::vector<>)
			{
				m_promap_arr.resize(aimaxthreadnum);
				m_premote2local_arr.resize(aimaxthreadnum);
				m_plocal2remote_arr.resize(aimaxthreadnum);
				for (uint32_t i = 0; i < aimaxthreadnum; ++i)
				{
					m_premote2local_arr[i] = new unpack_head_process<T_PHP1>();
				}

				for (uint32_t i = 0; i < aimaxthreadnum; ++i)
				{
					m_plocal2remote_arr[i] = new pack_head_process<T_PHP2>(aieverybytes);
				}

				for (uint32_t i = 0; i < aimaxthreadnum;++i)
				{
					m_promap_arr[i] = 
						protocol_base<T_PHP1, T_PHP2>::new_protocol_base_map(
							aimaxvalues, 
							m_premote2local_arr[i],
							m_plocal2remote_arr[i]
							);
				}
			}

			void run_task(uint32_t aipos, uint32_t aikey)
			{
				m_premote2local_arr[aipos]->reset(ap, aplen);
				T_PHP1* lphp = m_premote2local_arr[aipos]->get_head();
				uint32_t lprotocolnum = lphp->get_protocol_num();
				auto ltempfind = m_promap_arr[aipos]->find(lprotocolnum);

				if (ltempfind->sesond == m_promap_arr[aipos].end())
				{
					/**  write log */
					return true;
				}
				else
				{
					ltempfind->first.run_task(aikey);
				}


			}

		};



	} //namespace tools
} //namespace middleware


