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
  
    template <typename T_PHP1, typename T_PHP2>
    class protocol_base
    {
      //static std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* > m_promap;
      protocol_base<T_PHP1, T_PHP2>()
      {}
      //protocol_base<T_PHP1, T_PHP2>(const protocol_base&);

    protected:
      unpack_head_process<T_PHP1>* m_premote2local;
      pack_head_process<T_PHP2>* m_plocal2remote;
      uint32_t m_protocol_num;

      virtual bool task(uint32_t aikey) = 0;
      virtual void serialization() = 0;
      virtual void unserialization() = 0;
      virtual tools::protocol_base<T_PHP1, T_PHP2>* new_own() = 0;
      
    public:
      protocol_base<T_PHP1,T_PHP2>(uint32_t aiprotocolnum):
        m_premote2local(nullptr),
        m_plocal2remote(nullptr),
        m_protocol_num(aiprotocolnum)
      {}

      uint32_t protocol_num()
      {
        return m_protocol_num;
      }

      void set_pack_head_process(unpack_head_process<T_PHP1>* apremote2local, pack_head_process<T_PHP2>* aplocal2remote)
      {
        m_premote2local = apremote2local;
        m_plocal2remote = aplocal2remote;
      }


      static std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* >* new_protocol_base_map(
        std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* >& apromap,
        unpack_head_process<T_PHP1>* apremote2local, 
        pack_head_process<T_PHP2>* aplocal2remote
        )
      {
        std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* >* lret 
      = new std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* >();
    for (auto itor = apromap.begin(); itor != apromap.end(); ++itor)
        {
          lret->insert( std::make_pair( itor->first, itor->second->new_own() ) );
        }
        return lret;
      }

      /**
      *  回调,服务器没有aikey 可以用bind随便绑定一个值
      */
      bool run_task(uint32_t aikey)
      {
        unserialization();
        bool liret = task(aikey);
        if (liret)
        {
          serialization();
        }
        return liret;
      }



    };

		typedef protocol_base<spack_head::protocol_head, spack_head::protocol_head >  protocol_server_base;
		typedef protocol_base<cpack_head::protocol_head, cpack_head::protocol_head >  protocol_client_base;
  
	} //namespace tools
} //namespace middleware


#endif //PROTOCOL_BASE_HPP
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
