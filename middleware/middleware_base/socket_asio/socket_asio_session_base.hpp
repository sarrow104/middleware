///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_ASIO_SESSION_BASE_H
#define SOCKET_ASIO_SESSION_BASE_H


#include "middleware/middleware_base/socket_asio/socket_asio_tcp_io_service_pool.hpp"
#include "middleware/middleware_base/socket_asio/socket_session_mapping.hpp"
#include "middleware/tools/segmentation_pack/segmentation_pack.hpp"
#include "middleware/middleware_base/socket_asio/close_socket_session.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"
#include "middleware/tools/incidental/pack_head.hpp"
#include "middleware/tools/incidental/check_crc.hpp"
#include "middleware/middleware_base/middleware_base.hpp"

#include <cstdint>
#include <array>
#include <unordered_set>

#ifdef _MSC_VER
# ifndef snprintf
#  define snprintf _snprintf
# endif //snprintf
#endif //_MSC_VER

namespace middleware {

  //typedef boost::asio::ip::tcp::socket  boosttype_tcp_socket;
  typedef boost::asio::ip::tcp   boosttype_tcp;
  typedef boosttype_tcp::socket  boosttype_tcp_socket;

  class middleware_base;

  class socket_asio_session_base
  {
  protected:
    class tools_help
    {
    public:

      std::vector< std::unordered_set<uint32_t> > m_server;  /* 服务器之间保持的连接 */

      session_mapping<socket_asio_session_base*>  m_sessionidmapp;
      std::vector<middleware_base*> m_middlewarearr;
      std::vector<middleware_base*> m_extern_middlewarearr;
      std::vector<close_socket_session* > m_closesocket_help;
      std::vector<check_crc* > m_check_crc;
      std::vector< protocol_head > m_php;
      uint32_t m_everyoncemaxsize;    /* 单条最大数据 */
      uint32_t m_timeout;
      bool m_s2s;             /* 服务器与服务器的连接断开是否通知上层 */
      bool m_s2c;             /* 服务器与客户端的连接断开是否通知上层 */

      tools_help()
      {}

      void init(uint32_t aisize)
      {
        m_middlewarearr.resize(aisize);
        m_extern_middlewarearr.resize(aisize);
        m_closesocket_help.resize(aisize);
        m_check_crc.resize(aisize);
        m_server.resize(aisize);
        m_php.resize(aisize);
      }
    };

    /** socket 状态 */
    enum
    {
      SOCKET_OPEN_TYPE,
      SOCKET_CLOSE_TYPE,
    };


    boost::mutex m_lock;
    uint32_t m_close_socket_type;
    uint32_t m_closesocket_count;             /** socket关闭的次数 */

    boosttype_tcp_socket m_tcp_socket;        /** tcp socket 连接 */

    time_t  m_begin;                          /** 连接开始时间 */
    IP_ADDRESS_TYPE m_ipadress;               /** 客户端ip地址 */

    boost::asio::deadline_timer m_timer;      /** 定时器 */

    const uint32_t m_sessionid;               /** session 唯一id*/
    const uint8_t m_groupid;                  /** 此session所在的组id */                

    char* mp_data1;
    char* mp_data2;

    static tools_help m_tools;

    static const boost::asio::socket_base::linger mo_option;

    static uint32_t m_tempadd;

    static uint32_t m_heartbeat_num;        /* 心跳协议号 */

    //static err_log* m_errlog;

    uint8_t m_settimer;

    bool get_str( std::string& aistr, uint32_t aiport, uint32_t aigroupid)
    {
      char ch[128];
      if (snprintf(ch, 128, "port%dgroup%d", aiport, aigroupid) > 0)
      {
        aistr = ch;
        return true;
      }
      else
      {
        return false;
      }
    }

public:
    socket_asio_session_base(const socket_asio_arg& aiarg, uint32_t aigroupid, boost::asio::io_service& aiio_service) :
      m_tcp_socket(aiio_service),
      m_timer(aiio_service),
      m_closesocket_count(0),
      m_close_socket_type(SOCKET_OPEN_TYPE),
      m_sessionid(socket_asio_session_base::m_tempadd/*[ aiarg.m_type ]*/++),
      m_groupid(aigroupid),
      m_settimer(SET_TIMER__LOGIC_CONTROL::SET_TIMER_OPEN)
    {
      m_tools.init(aiarg.get_thread_maxsize());

      //m_errlog = aiarg.m_errlog;
      //middleware
      if (get_middleware() == NULL)
      {
        //
        std::string lstr;
        if (get_str(lstr, aiarg.m_port, aigroupid))
        {
          get_middleware() =
            new middleware::middleware_la_server(
              lstr.c_str(),
              aiarg.m_loopbuffermaxsize,
              aiarg.m_everyoncemaxsize,
              socket_asio_session_base::middleware_callback,
              aiarg.m_activ);
          get_extern_middleware() =
            new middleware::middleware_la_client(
              lstr.c_str(),
              aiarg.m_extern_loopbuffermaxsize,
              aiarg.m_extern_everyoncemaxsize,
              (aiarg.m_callbackfun[aigroupid]),
              aiarg.m_extern_activ);
          aiarg.m_middlewarearr[aigroupid] = get_extern_middleware();
        }
        else
        {
          //错误
        }
        
      }



      mp_data1 = new char[aiarg.m_everyoncemaxsize];
      mp_data2 = new char[aiarg.m_everyoncemaxsize];
      mp_data1 += server_head::END_POS;
      mp_data2 += server_head::END_POS;


      m_heartbeat_num = aiarg.m_heartbeat_num;

      //超时时间
      m_tools.m_timeout = aiarg.m_timeout;

      //最大单条接收数据
      m_tools.m_everyoncemaxsize = aiarg.m_everyoncemaxsize;

      m_tools.m_s2c = aiarg.m_s2c;
      m_tools.m_s2s = aiarg.m_s2s;



      if (get_closesocket_help() == NULL)
      {
        get_closesocket_help() = new close_socket_session();
      }


      if (get_check_crc() == NULL)
      {
        get_check_crc() = new check_crc();
      }
    }


    middleware_base*& get_middleware()
    { 
      return m_tools.m_middlewarearr[m_groupid]; 
    }

    middleware_base*& get_extern_middleware()
    { 
      return m_tools.m_extern_middlewarearr[m_groupid]; 
    }

    close_socket_session*& get_closesocket_help()
    { 
      return m_tools.m_closesocket_help[m_groupid]; 
    }

    check_crc*& get_check_crc()
    { 
      return m_tools.m_check_crc[m_groupid]; 
    }

    std::unordered_set<uint32_t>& get_server_sessionid_map()
    { 
      return m_tools.m_server[m_groupid]; 
    }

    bool get_s2c()
    { 
      return m_tools.m_s2c; 
    }

    bool get_s2s()
    { 
      return m_tools.m_s2s; 
    }

    static session_mapping<socket_asio_session_base*>& get_sessionidmapp( )
    { 
      return m_tools.m_sessionidmapp;
    }

    protocol_head& get_php()
    { 
      return m_tools.m_php[m_groupid]; 
    }

    uint8_t get_groupid()
    { 
      return m_groupid; 
    }

    ~socket_asio_session_base()
    {
      delete &m_tcp_socket;
    }

    boosttype_tcp_socket& socket()
    {
      return m_tcp_socket;
    }


    /** 子类可以补充reset */
    virtual bool reset_complementary() = 0;
    /** 子类可以补充close */
    virtual bool close_complementary() = 0;
    /** 子类可以补充handle_read */
    virtual bool handle_read_complementary(const boost::system::error_code& error, size_t bytes_transferred) = 0;
    /** 子类可以补充middleware_callback */
    virtual bool middleware_callback_complementary(protocol_head& m_php2, uint32_t aibyte) = 0;

    bool reset(unsigned long aiip)
    {
      boost::mutex::scoped_lock llock(m_lock);
      m_begin = time(NULL);
      m_ipadress = aiip;
      m_close_socket_type = SOCKET_OPEN_TYPE;
      m_settimer = (SET_TIMER__LOGIC_CONTROL::SET_TIMER_OPEN);
      reset_complementary();
      return true;
    }
    

    void close(uint32_t aicount, bool aisend_protocol)
    {
      m_lock.lock();
      if (((aicount == m_closesocket_count) && (m_close_socket_type == SOCKET_OPEN_TYPE)) && m_tcp_socket.is_open() /*|| (aicount == 0xFFFFFFFF) */)
      {
        m_timer.cancel();
        m_close_socket_type = SOCKET_CLOSE_TYPE;
        //if( m_tcp_socket.is_open() )
        //{
        //m_tcp_socket.set_option( mo_option );
        //}
        m_tcp_socket.close();
        close_complementary();
        ++m_closesocket_count;

        if (aisend_protocol)
        {
          /* 通知协议 此函数一定是单线程串行执行的   所以用静态变量*/
          static char lbuf1[protocol_head::END_POS] = { 0 };
          static char lbuf2[protocol_head::END_POS] = { 0 };
          static char* lp1 = lbuf1;
          static char* lp2 = lbuf2;

          static protocol_head lphp;

          std::swap(lp1, lp2);
          lphp.reset(lp1, protocol_head::END_POS);
          lphp.set_data_byte(0);
          //lphp.get_protocol_num() = 0;
          lphp.get_protocol_num() = 9999;
          lphp.get_sessionid() = m_sessionid;
          get_middleware()->send(lp1, protocol_head::END_POS);
        }


      }
      m_lock.unlock();
    }
    
    void handle_timer(const boost::system::error_code& error)
    {
      if (!error && m_settimer != SET_TIMER__LOGIC_CONTROL::SET_TIMER_CLOSE)
      {
        push_close();
        //write_log(m_errlog, m_ipadress, THROW__TIME_OUT, m_sessionid, m_groupid, "", 0);
        //single_callback_module_fun()->writeipsessionidnummsglog(  m_ipadress , THROW__TIME_OUT ,m_sessionid , m_groupid, "" , 0 );
      }
    }

    /* 重置定时器 */
    void reset_timer()
    {
      m_timer.expires_from_now(boost::posix_time::seconds(m_tools/*[ m_type ]*/.m_timeout));
      m_timer.async_wait(boost::bind(&socket_asio_session_base::handle_timer, this, boost::asio::placeholders::error));
    }


    inline void start()
    {
      if (m_settimer == SET_TIMER__LOGIC_CONTROL::SET_TIMER_OPEN)
      {
        reset_timer();
      }

      std::swap(mp_data1, mp_data2);

      m_tcp_socket.async_read_some(
        boost::asio::buffer(mp_data1, m_tools.m_everyoncemaxsize),
        boost::bind(&socket_asio_session_base::handle_read,
          this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred)
        );
    }

    enum CONNECT_TYPE
    {
      CONNECT_S2S,
      CONNECT_S2C,
    };

    void push_close()
    {
      /* 派生类关闭前期事件 */
      std::unordered_set<uint32_t>& lus = get_server_sessionid_map();
      /* 是否需要通知协议层连接断开 */
      bool lret;
      if (lus.find(m_sessionid) != lus.end())
      {
        lret = push_close(CONNECT_TYPE::CONNECT_S2S);
        get_server_sessionid_map().erase(m_sessionid);
      }
      else
      {
        lret = push_close(CONNECT_TYPE::CONNECT_S2C);
      }

      get_closesocket_help()->push_close(this, m_closesocket_count, lret);
    }

    bool push_close(CONNECT_TYPE aiconnecttype)
    {
      if ((CONNECT_S2S == aiconnecttype && get_s2s()) || (CONNECT_S2C == aiconnecttype && get_s2c()))
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
      if (!error)
      {
        m_timer.cancel();
        handle_read_complementary(error, bytes_transferred);
      }
      else
      {
        push_close();
        //write_log(m_errlog, m_ipadress, THROW__BOOST_HANDLE_READ, m_sessionid, m_groupid, "", 0);
        //single_callback_module_fun()->writeipsessionidnummsglog(  m_ipadress , THROW__BOOST_HANDLE_READ , m_sessionid ,m_groupid, "" , 0 );
      }

    }





    void set_php(char* ap, uint32_t alen)
    {
      get_php().reset(
        ap,
        alen
        );
    }

    /* 检查协议号  如果是心跳协议直接退出 */
    bool check_heartbeat_fun()
    {
      return (get_php().get_protocol_num() == m_heartbeat_num) ? true : false;
    }

    bool check_crc_fun()
    {
#ifdef  CHECK_CRC
      if (get_php().check_crc())
      {
#endif
        get_php().set_data_byte(get_php().get_protocol_body_len());
        get_php().get_ip() = m_ipadress;
        get_php().get_groupid() = m_groupid;
        get_php().get_sessionid() = m_sessionid;
        get_php().get_closesocket_count() = m_closesocket_count;
        //      get_php().get_sessiontype() = m_type;
        get_php().get_settimer() = m_settimer;
        get_php().get_server_error() = 0;
        //  get_php().set_mgic_num();
        return true;
#ifdef  CHECK_CRC
      }
      else
      {
        return false;
      }
#endif
    }




    void session_send(uint32_t aiclosesocket_count, char* ap, uint32_t aplen)
    {
      m_lock.lock();


      if (socket().is_open() && m_closesocket_count == aiclosesocket_count)
      {
        socket().async_send(
          boost::asio::buffer(ap, aplen),
          boost::bind(
            &socket_asio_session_base::handle_write,
            this,
            boost::asio::placeholders::error
            )
          );
      }
      m_lock.unlock();
    }

    static bool middleware_callback(const char* ap, uint32_t aplen)
    {
      //uint8_t lgroupid = GET_GROUPID( ap );
      protocol_head m_php2;
      m_php2.reset(ap, aplen);



      if (aplen >= m_php2.get_protocol_head_len())
      {




        uint32_t lsessionid = m_php2.get_sessionid();
        //uint32_t lgroupid = m_php2.get_groupid();
        //  uint32_t lsessiontype = m_php2.get_sessiontype();

        session_mapping<socket_asio_session_base*>& lsmsasb = get_sessionidmapp( /*lsessiontype*/);

        socket_asio_session_base* lthis = lsmsasb.find(lsessionid);




        if (lthis == NULL)
        {
          //write_log(m_errlog, THROW__SESSION_NOTFIND);
          //  single_callback_module_fun()->writenumlog( THROW__SESSION_NOTFIND );
          return true;
        }

        if (m_php2.get_closesocket_count() != lthis->m_closesocket_count)
        {
          return true;
        }


        if (m_php2.get_server_error() != 0)
        {
          if (m_php2.get_server_error() == (uint32_t)(-1))
          {
            /* 如果lg lf server登录成功会触发 */
            lthis->get_server_sessionid_map().insert(lsessionid);
            lthis->session_send(m_php2.get_closesocket_count(), m_php2.get_protocol_buffer(), m_php2.get_protocol_buffer_len());

          }
          else
          {
            if (m_php2.get_error() != 0)
            {
              lthis->session_send(m_php2.get_closesocket_count(), m_php2.get_protocol_buffer(), m_php2.get_protocol_buffer_len());
            }
            lthis->push_close();
          }
        }
        else
        {
          lthis->session_send(m_php2.get_closesocket_count(), m_php2.get_protocol_buffer(), m_php2.get_protocol_buffer_len());
        }

        lthis->middleware_callback_complementary(m_php2, aplen);



      }


      return true;
    }


  

    /*发送后的回调 */
    void handle_write(const boost::system::error_code& error)
    {
      //extern_close();
      if (error)
      {
        //write_log(m_errlog, THROW__SESSION_NOTFIND);
        //single_callback_module_fun()->writenumlog( THROW__SESSION_NOTFIND );
      }
    }

  };




} //namespace middleware
#endif //SOCKET_ASIO_SESSION_BASE_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
