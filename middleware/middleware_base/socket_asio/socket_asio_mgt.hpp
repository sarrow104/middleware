///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_ASIO_MGT
#define SOCKET_ASIO_MGT


#include "middleware/middleware_base/socket_asio/alonelist.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_keep.hpp"

namespace middleware {

  class socket_asio_server_mgt
  {
    socket_asio_server_mgt(const socket_asio_server_mgt&);
  protected:
    struct static_member_keep
    {
      socket_asio_tcp_io_service_pool* m_io_service_pool;     /* boost io_service pool */
      socket_asio_session_base* m_ptemp_session;
      boost::asio::ip::tcp::acceptor* m_acceptor;
      std::vector<socket_asio_server*> m_asion_server_vec;
      uint32_t m_thread_size;
    };
    static socket_asio_server_mgt* m_this;
    static static_member_keep m_smsk;

    static void accept()
    {
      m_smsk.m_ptemp_session = m_smsk.m_asion_server_vec[0]->get_session();
      m_smsk.m_acceptor->async_accept(m_smsk.m_ptemp_session->socket(),
        boost::bind(&socket_asio_server_mgt::handle_accept, m_smsk.m_ptemp_session, boost::asio::placeholders::error));
    }

    static void handle_accept(socket_asio_session_base* new_session, const boost::system::error_code& error)
    {
      if (!error)
      {
        new_session->reset(new_session->socket().remote_endpoint().address().to_v4().to_ulong());
        new_session->start();
      }
      accept();
    }




    socket_asio_server* get_new_socket_asio_server(static_member_keep& aismsok, socket_asio_arg& aiarg)
    {
#if 0
      switch (aiarg.m_type)
      {
      case TYPE_KEEP:
        return new socket_asio_server_keep(aiarg, aismsok.m_io_service_pool);
      case TYPE_SHORT:
        return new socket_asio_server_short(aiarg, aismsok.m_io_service_pool);
      default:
        throw 1;
      }
#endif
      
      return new socket_asio_server_keep(aiarg, aismsok.m_io_service_pool);
    }

    void init_base(static_member_keep& aismsok, socket_asio_arg& aiarg)
    {
      aismsok.m_thread_size = aiarg.get_thread_maxsize();
      aismsok.m_io_service_pool = (new socket_asio_tcp_io_service_pool(aiarg.get_thread_maxsize()));
      aismsok.m_acceptor = new boosttype_tcp::acceptor(
        aismsok.m_io_service_pool->get_io(),
        boosttype_tcp::endpoint(
          boosttype_tcp::v4(),
          aiarg.m_port)
        );

      //set_single_callback_module_fun( aiarg.m_callback_module );

      aismsok.m_asion_server_vec.resize(aiarg.get_thread_maxsize());

      for (uint32_t i = 0; i < aiarg.get_thread_maxsize(); ++i)
      {
        aismsok.m_asion_server_vec[i] = get_new_socket_asio_server(aismsok, aiarg);

      }
    }



    socket_asio_server_mgt()
    {


    }

    void init_(static_member_keep& lsmsok, socket_asio_arg& aiarg)
    {
      if (lsmsok.m_thread_size == 0)
      {
        init_base(lsmsok, aiarg);
      }
    }

  public:

    static socket_asio_server_mgt* init(socket_asio_arg& aiarg)
    {
      static_member_keep& lsmsok = m_smsk;

      if (m_this == NULL)
      {
        m_this = new socket_asio_server_mgt();
      }

      m_this->init_(lsmsok, aiarg);

      return m_this;
    }



    socket_asio_server* get_socket_asio_server(uint32_t ainum, uint32_t aitype)
    {
      static_member_keep& lsmsok = m_smsk;

      if (ainum < lsmsok.m_thread_size)
      {
        return lsmsok.m_asion_server_vec[ainum];
      }
      else
      {
        throw 1;
      }
    }

    static void start(uint32_t aitype)
    {
      accept();
    }
  };

} //namespace middleware 
#endif //SOCKET_ASIO_MGT

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */


