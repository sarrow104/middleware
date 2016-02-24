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
      static std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* > m_promap;
      protocol_base<T_PHP1, T_PHP2>()
      {}
      //protocol_base<T_PHP1, T_PHP2>(const protocol_base&);

    protected:
      unpack_head_process<T_PHP1>* m_premote2local;
      pack_head_process<T_PHP2>* m_plocal2remote;

      virtual bool task(uint32_t aikey) = 0;
      virtual void serialization() = 0;
      virtual void unserialization() = 0;
      virtual tools::protocol_base<spack_head::protocol_head, spack_head::protocol_head>* new_own() = 0;
    public:
      protocol_base<T_PHP1,T_PHP2>(uint32_t aiprotocolnum):
        m_premote2local(nullptr),
        m_plocal2remote(nullptr)
      {
        auto itor = m_promap.find(aiprotocolnum);
        if ( itor != m_promap.end())
        {
          itor->second = this;
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


      static std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* >* new_protocol_base_map( uint32_t aimax, unpack_head_process<T_PHP1>* apremote2local, pack_head_process<T_PHP2>* aplocal2remote)
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
            itor->second = itor->second->new_own();
          }
        }
        else
        {
          return nullptr;
        }

        for (auto itor = m_promap.begin(); itor != m_promap.end();++itor)
        {
          itor->second->set_pack_head_process(apremote2local, aplocal2remote);
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
        bool liret = task(aikey);
        if (liret)
        {
          serialization();
        }
        return liret;
      }



    };

  
    template <typename T_PHP1, typename T_PHP2>
    std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* > protocol_base<T_PHP1, T_PHP2>::m_promap;

  } //namespace tools
} //namespace middleware


#endif //PROTOCOL_BASE_HPP
 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
