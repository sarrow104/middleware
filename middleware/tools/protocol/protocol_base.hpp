///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef PROTOCOL_BASE_HPP
#define PROTOCOL_BASE_HPP

#include "middleware/middleware_base/middleware_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

#include <cstdint>
#include <unordered_map>
#include <vector>


namespace middleware {
  namespace tools {
  
    template <typename T_PHP>
    class protocol_base
    {
			typedef unpack_head_process<T_PHP>		type_uhp;
			typedef pack_head_process<T_PHP>			type_php;
			typedef protocol_base<T_PHP> type_own_base;
			typedef std::unordered_map<uint32_t, protocol_base<T_PHP>* > type_map;
      //static std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* > m_promap;
      protocol_base<T_PHP>()
      {}
      //protocol_base<T_PHP1, T_PHP2>(const protocol_base&);
			void set_pack_head_process(type_uhp* apremote2local, type_php* aplocal2remote)
			{
				m_premote2local = apremote2local;
				m_plocal2remote = aplocal2remote;
			}
    protected:
			
			type_uhp* m_premote2local;
			type_php* m_plocal2remote;
      uint32_t m_protocol_num;

			

      virtual uint32_t task(uint32_t aikey) = 0;
      virtual void serialization() = 0;
      virtual void unserialization() = 0;
      virtual type_own_base* new_own() = 0;
      
    public:
      protocol_base<T_PHP>(uint32_t aiprotocolnum):
        m_premote2local(nullptr),
        m_plocal2remote(nullptr),
        m_protocol_num(aiprotocolnum)
      {}

      uint32_t protocol_num()
      {
        return m_protocol_num;
      }

      


      static type_map* new_protocol_base_map(
				type_map& apromap,
				type_uhp* apremote2local,
				type_php* aplocal2remote
        )
      {
				type_map* lret
      = new type_map();

				type_own_base* lptemp;

				for (auto itor = apromap.begin(); itor != apromap.end(); ++itor)
        {
					lptemp = itor->second->new_own();
					lptemp->set_pack_head_process(apremote2local, aplocal2remote);

          lret->insert( std::make_pair( itor->first, lptemp) );
        }
        return lret;
      }

      /**
      *  回调,服务器没有aikey 可以用bind随便绑定一个值
      */
      uint32_t run_task(uint32_t aikey)
      {
        unserialization();
        uint32_t liret = task(aikey);
        if (liret)
        {
          serialization();
        }
        return liret;
      }



    };

    typedef protocol_base<spack_head::protocol_head>  protocol_server_base;
    typedef protocol_base<cpack_head::protocol_head>  protocol_client_base;
  
  } //namespace tools
} //namespace middleware


#endif //PROTOCOL_BASE_HPP
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
