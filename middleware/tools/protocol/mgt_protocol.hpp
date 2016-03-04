///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef MGT_PROTOCOL_HPP
#define MGT_PROTOCOL_HPP

#include "middleware/tools/protocol/protocol_base.hpp"


namespace middleware {
  namespace tools {

    template <typename T_PHP1, typename T_PHP2>
    class mgt_protocol
    {
      std::vector< std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>*>* > m_promap_arr;
      std::vector< unpack_head_process<T_PHP1>* > m_premote2local_arr;
      std::vector< pack_head_process<T_PHP2>* > m_plocal2remote_arr;
      std::vector< middleware_base*> m_middle_arr;
    public:
      mgt_protocol(
        std::unordered_map<uint32_t, protocol_base<T_PHP1, T_PHP2>* >& apromap, 
        uint32_t aimaxthreadnum, 
        uint32_t aieverybytes)
      {
        m_promap_arr.resize(aimaxthreadnum);
        m_premote2local_arr.resize(aimaxthreadnum);
        m_plocal2remote_arr.resize(aimaxthreadnum);
        for (uint32_t i = 0; i < aimaxthreadnum; ++i)
        {
          m_premote2local_arr[i] = new unpack_head_process<T_PHP1>();
      m_plocal2remote_arr[i] = new pack_head_process<T_PHP2>(aieverybytes);
        }

        for (uint32_t i = 0; i < aimaxthreadnum; ++i)
        {

          m_promap_arr[i] =
            protocol_base<T_PHP1, T_PHP2>::new_protocol_base_map(
              apromap,
              m_premote2local_arr[i],
              m_plocal2remote_arr[i]
              );
        }
      }

      bool run_task(uint32_t aipos, uint32_t aikey, const char* ap, uint32_t aplen)
      {
        m_premote2local_arr[aipos]->reset(ap, aplen);
        T_PHP1* lphp = m_premote2local_arr[aipos]->get_head();
        uint32_t lprotocolnum = lphp->get_protocol_num();
        auto ltempfind = m_promap_arr[aipos]->find(lprotocolnum);

        if (ltempfind->first)
        {
          /**  write log */
          return true;
        }
        else
        {
          T_PHP2* lphp2 = m_plocal2remote_arr[aipos]->get_head();
          m_plocal2remote_arr[aipos]->set_pack_head(lphp);
          lphp2->get_error() = ltempfind->second->run_task(aikey);
          m_middle_arr[aipos]->send(lphp2->get_buffer(), lphp2->get_buffer_len());
        }
        return true;
      }


    };


    typedef mgt_protocol<spack_head::protocol_head, spack_head::protocol_head>  mgt_server_protocol;
    typedef mgt_protocol<cpack_head::protocol_head, cpack_head::protocol_head>  mgt_client_protocol;
    /** 服务器协议 map */
    typedef std::unordered_map<uint32_t, protocol_base<spack_head::protocol_head, spack_head::protocol_head>* >   type_server_protocol_map;
    /** 客户端协议 map */
    typedef std::unordered_map<uint32_t, protocol_base<cpack_head::protocol_head, cpack_head::protocol_head>* >   type_client_protocol_map;

  /**
   * 创建一个服务器
   * Parameter 1  配置类型
   * Parameter 2  基本配置文档
   * Parameter 3  此服务器对应的一组协议
   ****/

    middleware_asio_server* create_server_protocol_mgt(
      uint32_t aiconfigtype,
      const char* aiconfigpath,
      type_server_protocol_map& apromap
      );


    middleware_asio_client* create_client_protocol_mgt(
      uint32_t aiconfigtype,
      const char* aiconfigpath,
      type_client_protocol_map& apromap
      );

   void connect_server(
      middleware_asio_client* ap,
      uint32_t aiconfigtype,
      const char* aiconfigpath, 
      boost::function<bool(const char*, uint32_t)> aisendfailure
      );


  } // namespace tools
}//namespace middleware
#endif //MGT_PROTOCOL_HPP

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
