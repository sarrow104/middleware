#ifndef ACCEPT_KEY_SOCKET_H
#define ACCEPT_KEY_SOCKET_H

#include "lpthread.h"
#include "socket_head.h"

#include <bimap.hpp>
#include <iostream>
#include <function.hpp>

  
namespace middleware{

  /*****************************
   ** accept_key_socket内部使用类
   *****************************/
  struct key_stat
  {
    uint32_t m_key;
    bool m_socket_stat;

    key_stat(uint32_t aikey) :
      m_key(aikey),
      m_socket_stat(true)
    {}

    /*
    *  比较函数
    */
    bool operator<(const key_stat& r)const
    {
      return m_key < r.m_key;
    }

    bool get_stat()const
    {
      return m_socket_stat;
    }
    void set_stat(bool aistat)
    {
      m_socket_stat = aistat;
    }
  };

  /*****************************
   ** socket server 辅助函数
   ** 处理有关socket初始化,端口监听,
   *****************************/
  class accept_key_socket
  {

    static boost::bimap<SOCKET, key_stat>  m_socket_key;
    SOCKET m_ssock;
    uint32_t m_port;
    boost::mutex m_lock;
#ifdef WIN32
    int m_len;
#else
    socklen_t m_len;
#endif
    boost::function<bool(SOCKET)> m_recvfun;
    boost::function<bool(const char*, uint32_t)> m_sendfailure;     /*  发送失败回调  */

    /*
     *  accept等待线程
     */
    void run_accept()
    {
      SOCKET l_csock;
      struct sockaddr_in l_caddr;

      while (1)
      {
        l_csock = accept(m_ssock, (struct sockaddr *)&l_caddr, &m_len);
        if (l_csock == INVALID_SOCKET)
        {
          return;
        }
        boost::thread(boost::bind(&accept_key_socket::recv_key, this, l_csock));
      }
    }

    /*
     *  接收key,建立连接
     */
    void recv_key(SOCKET aisocket)
    {
      char lirecv[32];
      int liret;

      liret = g_recv(aisocket, lirecv, 32);
      if (liret <= 0)
      {
        closesocket(aisocket);
        return;
      }

      if (liret < sizeof(uint32_t))
      {
        closesocket(aisocket);
        return;
      }

      uint32_t lkey = *((uint32_t*)(lirecv));


      auto itor = m_socket_key.right.find(lkey);
      if (itor != m_socket_key.right.end())
      {
        closesocket(aisocket);
        return;
      }

      m_socket_key.insert(boost::bimap<SOCKET, key_stat>::value_type(aisocket, lkey));


      /* 回复 */
      if (g_send(aisocket, lirecv, liret) <= 0)
      {
        closesocket(aisocket);
        return;
      }
      boost::thread(boost::bind(&accept_key_socket::recv, this, aisocket));
    }
  public:

    accept_key_socket(uint32_t aiport, boost::function<bool(SOCKET)> aifun, boost::function<bool(const char*, uint32_t)>& aisendfailure) :
      m_port(aiport),
      m_recvfun(aifun),
      m_sendfailure(aisendfailure)
    {
      boost::thread(boost::bind(&accept_key_socket::initsocket, this));
    }

    /*
     *  初始化socket
     */
    void initsocket()
    {
      struct sockaddr_in m_saddr;

#ifdef WIN32
      WSADATA wsaData;
      if (WSAStartup(MAKEWORD(2, 2), &wsaData))
      {
        WSACleanup();
      }
#endif

      m_saddr.sin_family = AF_INET;
      m_saddr.sin_port = htons(m_port);
      m_saddr.sin_addr.s_addr = htonl(INADDR_ANY);
      m_ssock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

      if (m_ssock == INVALID_SOCKET)
      {
        std::cout << WSAGetLastError() << std::endl;
        throw 0;
      }
      if (::bind(m_ssock, (struct sockaddr*)&m_saddr, sizeof(m_saddr)) == SOCKET_ERROR)
      {
        std::cout << WSAGetLastError() << std::endl;
        throw 0;
      }
      m_len = sizeof(sockaddr_in);

      if (listen(m_ssock, 10) == SOCKET_ERROR)
      {
        std::cout << WSAGetLastError() << std::endl;
        throw 0;
      }

      run_accept();
    }

    /*
     *  关闭socket连接
     */
    void close_accept_socket()
    {
      closesocket(m_ssock);
    }

    /*
     * 接收数据
     */
    void recv(SOCKET aisocket)
    {
      m_recvfun(aisocket);
    }

    /*
     * 根据key获取socket
     */
    bool get_socket(uint32_t aikey, SOCKET& aisocket)
    {
      auto itor = m_socket_key.right.find(aikey);
      if (itor != m_socket_key.right.end())
      {
        aisocket = itor->second;
        return true;
      }
      else
      {
        aisocket = 0;
        return false;
      }
    }

    /*
     *  关闭socket
     */
    bool close(SOCKET aisocket, const char* ap, uint32_t aplen)
    {
      /* 移除 */
      auto itor = m_socket_key.left.find(aisocket);
      if (itor == m_socket_key.left.end())
      {
        return false;
      }

      if (itor->second.get_stat())
      {
        (const_cast<key_stat*>(&itor->second))->set_stat(false);
        closesocket(aisocket);
      }

      return m_sendfailure(ap, aplen);
    }

    /*
    *  关闭所有连接
    */
    bool closeallconnect()
    {
      for (auto itor = m_socket_key.left.begin(); itor != m_socket_key.left.end(); ++itor)
      {
        if (itor->second.get_stat())
        {
          (const_cast<key_stat*>(&itor->second))->set_stat(false);
          closesocket(itor->first);
        }
      }
      return true;
    }

  };

} //namespace middleware





#endif
