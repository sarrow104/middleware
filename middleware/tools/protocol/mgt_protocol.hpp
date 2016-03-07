///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef MGT_PROTOCOL_HPP
#define MGT_PROTOCOL_HPP

#include "middleware/tools/protocol/protocol_base.hpp"


namespace middleware {
  namespace tools {

    template <typename T_PHP>
    class mgt_protocol
    {

		protected:
			typedef unpack_head_process<T_PHP>		type_uhp;
			typedef pack_head_process<T_PHP>			type_php;
			typedef protocol_base<T_PHP> type_own_base;
			typedef std::unordered_map<uint32_t, protocol_base<T_PHP>* > type_map;
		private:
			std::vector< type_map* > m_promap_arr;
			std::vector< type_uhp* > m_premote2local_arr;
			std::vector< type_php* > m_plocal2remote_arr;
			std::vector< char* >  m_premote2local_buffer;
			std::vector< char* >  m_plocal2remote_buffer;
			std::vector< middleware_base*> m_middle_arr;
    public:
      mgt_protocol(
				type_map& apromap,
        uint32_t aimaxthreadnum, 
        uint32_t aieverybytes)
      {
        m_promap_arr.resize(aimaxthreadnum);
        m_premote2local_arr.resize(aimaxthreadnum);
        m_plocal2remote_arr.resize(aimaxthreadnum);
				m_premote2local_buffer.resize(aimaxthreadnum);
				m_plocal2remote_buffer.resize(aimaxthreadnum);
        for (uint32_t i = 0; i < aimaxthreadnum; ++i)
        {
          m_premote2local_arr[i] = new type_uhp();
					m_plocal2remote_arr[i] = new type_php(aieverybytes + T_PHP::END_POS);
					m_premote2local_buffer[i] = new char[aieverybytes * 2];
					m_plocal2remote_buffer[i] = new char[aieverybytes * 2];
        }

        for (uint32_t i = 0; i < aimaxthreadnum; ++i)
        {
          m_promap_arr[i] =
            protocol_base<T_PHP>::new_protocol_base_map(
              apromap,
              m_premote2local_arr[i],
              m_plocal2remote_arr[i],
							m_premote2local_buffer[i],
							m_plocal2remote_buffer[i]
              );
        }
      }

      bool run_task(uint32_t aipos, uint32_t aikey, const char* ap, uint32_t aplen)
      {
        m_premote2local_arr[aipos]->reset(ap, aplen);
				T_PHP* lphp;
				protocol_base<T_PHP>* lpb = find_protocol(m_promap_arr[aipos], m_premote2local_arr[aipos], lphp);
				if (lpb == nullptr)
				{
					/** print err log*/
				}
				else
				{
					T_PHP* lphp2 = m_plocal2remote_arr[aipos]->get_head();
					/** 触发逻辑 */
					lphp2->get_error() = lpb->run_task(aikey);
					

					/** 设置serr(server) */
					set_serr(lpb, lphp);
					m_plocal2remote_arr[aipos]->set_pack_head(lphp);
					/** 设置群发队列(server) */
					set_mass_send_arr(lpb, lphp2);

					//m_middle_arr[aipos]->send(lphp2->get_buffer(), lphp2->get_buffer_len());
					/** 回复 */
					send(m_middle_arr[aipos], lphp2);
				}
        return true;
      }

			virtual void set_mass_send_arr(protocol_base<T_PHP>* abp, T_PHP* ap)
			{

			}
			virtual void set_serr(protocol_base<T_PHP>* abp, T_PHP* ap)
			{

			}

			protocol_base<T_PHP>* find_protocol(type_map* apmap , type_uhp* auhp, T_PHP*& aphp)
			{
				
				aphp = auhp->get_head();
				uint32_t lprotocolnum = aphp->get_protocol_num();
				auto lfind = apmap->find(lprotocolnum);
				if (lfind != apmap->end())
				{
					return lfind->second;
				}
				else
				{
					return nullptr;
				}
			}

			void send(middleware_base* amp,T_PHP* aphp)
			{
				amp->send(aphp->get_buffer(), aphp->get_buffer_len());
			}

    };

		class mgt_server_protocol :
			public mgt_protocol<spack_head::protocol_head>
		{
			typedef spack_head::protocol_head type_sph;
		public:
			mgt_server_protocol(
				mgt_protocol<spack_head::protocol_head>::type_map& aimap,
				uint32_t aimaxthreadnum,
				uint32_t aieverybytes):
				mgt_protocol<spack_head::protocol_head>(aimap, aimaxthreadnum, aieverybytes)
			{}

			virtual void set_mass_send_arr(protocol_base<type_sph>* abp, type_sph* ap)
			{
				protocol_server_base* lp = (protocol_server_base*)(abp);
				std::vector< spack_head::session_infor >& larr = lp->get_session_list();
				if (larr.empty())
				{
					ap->set_mass_send_arr();
				}
				else
				{
					ap->set_mass_send_arr(larr);
				}
			}
			virtual void set_serr(protocol_base<type_sph>* abp, type_sph* ap)
			{
				protocol_server_base* lp = (protocol_server_base*)(abp);
				ap->get_server_error() = lp->get_serr();
			}
		};

    typedef mgt_server_protocol  mgt_sprotocol;
    typedef mgt_protocol<cpack_head::protocol_head>  mgt_cprotocol;
    /** 服务器协议 map */
    typedef std::unordered_map<uint32_t, protocol_base<spack_head::protocol_head>* >   type_sprotocol_map;
    /** 客户端协议 map */
    typedef std::unordered_map<uint32_t, protocol_base<cpack_head::protocol_head>* >   type_cprotocol_map;

  /**
   * 创建一个服务器
   * Parameter 1  配置类型
   * Parameter 2  基本配置文档
   * Parameter 3  此服务器对应的一组协议
   ****/

    middleware_asio_server* create_server_protocol_mgt(
      uint32_t aiconfigtype,
      const char* aiconfigpath,
      type_sprotocol_map& apromap
      );


    middleware_asio_client* create_client_protocol_mgt(
      uint32_t aiconfigtype,
      const char* aiconfigpath,
      type_cprotocol_map& apromap
      );

		uint32_t connect_server(
      middleware_asio_client* ap,
      uint32_t aiconfigtype,
      const char* aiconfigpath, 
      boost::function<bool(const char*, uint32_t)> aisendfailure
      );

	 void sendto_server(
		 middleware_asio_client* ap,
		 protocol_base<cpack_head::protocol_head>* apb
		 );


  } // namespace tools
}//namespace middleware
#endif //MGT_PROTOCOL_HPP

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
