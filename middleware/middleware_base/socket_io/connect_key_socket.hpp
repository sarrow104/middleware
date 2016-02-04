///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef CONNECT_KEY_SOCKET_H
#define CONNECT_KEY_SOCKET_H
#include "middleware/middleware_base/socket_io/lpthread.hpp"
#include "middleware/middleware_base/socket_io/socket_head.hpp"
#include "middleware/tools/threadpool/threadpool.hpp"

#include <boost/function.hpp>
#include <boost/bimap.hpp>

namespace middleware {
  /**
   * connect_key_socket的辅助类
   */
  struct key_ip_port
  {
    uint32_t m_key;
    std::string m_ip;
    uint32_t m_port;
    bool m_socket_stat;
    /** 发送失败的回调 */
    boost::function<bool(const char*, uint32_t)> m_sendfailure_callback;

    key_ip_port(
      uint32_t aikey,
      const char* aiip,
      uint32_t aiport,
      const boost::function<bool(const char*, uint32_t)>& aisendfailure_callback
      ) :
      m_key(aikey),
      m_ip(aiip),
      m_port(aiport),
      m_socket_stat(true),
      m_sendfailure_callback(aisendfailure_callback)
    {}

    key_ip_port(uint32_t aikey) :
      m_key(aikey)
    {}

    bool operator<(const key_ip_port& r)const
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

  /**
   * 负责维护 socket client连接
   */
  class connect_key_socket
  {
    SOCKET m_socket;
    static boost::bimap<SOCKET, key_ip_port>  m_socket_key;
    boost::function<bool(SOCKET)> m_recvfun;
	/**
	 *  初始化socket
	 */
	void initsocket()
	{
#ifdef _MSC_VER
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			WSACleanup();
			return;
		}
#endif
		return;
	}

	/**
	 * 创建连接
	 */
	SOCKET create_con(uint32_t aikey, const char* aiserverip, int32_t aiserverport)
	{
		SOCKET lsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		/** 构建服务器地址信息 */
		struct sockaddr_in laddr;
		laddr.sin_family = AF_INET;
		laddr.sin_port = htons(aiserverport);
		laddr.sin_addr.s_addr = inet_addr(aiserverip);
		/** 连接服务器 */
		while (connect(lsock, (struct sockaddr *)&laddr, sizeof(laddr)) == SOCKET_ERROR)
		{
			//ErrlogWrite(G_LOGMSG_SOCKET,ERROR_SOCKET_INIT_NUM,"connect()");
			closesocket(lsock);
#ifdef WIN32
			WSACleanup();
#endif
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
		return lsock;
	}

  public:
    connect_key_socket(boost::function<bool(SOCKET)> airecv) :
      m_recvfun(airecv)
    {
	  CREATE_LOG(LOG_SOCKET_IO_ID, LOG_SOCKET_IO_STR)
      initsocket();
    }

	/**
	 * 进一步确认创建连接
	 */
	bool send_key(uint32_t aikey, const char* aiserverip, int32_t aiserverport, boost::function<bool(const char*, uint32_t)> aisendfailure)
	{
		m_socket = create_con(aikey, aiserverip, aiserverport);
		char lsendkey[32] = { 0 };
		char lrecvkey[32] = { 0 };
		size_t lsendlen = strlen(lsendkey) + 1;
		if (g_send(m_socket, (const char*)&aikey, sizeof(uint32_t)) > 0)
		{
			size_t lrecvlen = g_recv(m_socket, lrecvkey, 32);
			if (lrecvlen < sizeof(uint32_t))
			{
				/** 失败 */
				closehandle(m_socket);
				LOG_ERROR(LOG_SOCKET_IO_ID, "send_key()失败,lrecvlen=[%d] < sizeof(uint32_t)", lrecvlen );
				return false;
			}
			else
			{
				if (*((uint32_t*)(lrecvkey)) != aikey)
				{
					LOG_ERROR(LOG_SOCKET_IO_ID, "send_key()失败,lrecvkey=[%d] < aikey=[%d]", *((uint32_t*)(lrecvkey)),aikey );
					/** 失败 */
					closehandle(m_socket);
					return false;
				}
			}
		}
		key_ip_port lkey_ip_port(aikey, aiserverip, aiserverport, aisendfailure);
		auto itor = m_socket_key.right.find(lkey_ip_port);
		if (itor != m_socket_key.right.end())
		{
			m_socket_key.right.erase(itor);
		}
		(m_socket_key.insert(boost::bimap<SOCKET, key_ip_port>::value_type(m_socket, lkey_ip_port))).second;

		//boost::thread(boost::bind(&connect_key_socket::recv, this, m_socket));
		 middleware::tools::threadpool::asyn_thread( boost::bind(&connect_key_socket::recv, this, m_socket) );
		return true;
	}

  void recv(SOCKET aisocket)
  {
    m_recvfun(aisocket);
  }

	 /*
	  *  通过key获取socket
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
	 *  通过socket获取key
	 */
    bool get_key(uint32_t& aikey, SOCKET aisocket)
    {
      auto itor = m_socket_key.left.find(aisocket);
      if (itor != m_socket_key.left.end())
      {
        aikey = itor->second.m_key;
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
    void closehandle(SOCKET aisocket)
    {
      closesocket(m_socket);
    }

	/*
	 *  通过key关闭socket
	 */
    bool closekey(uint32_t key)
    {
      auto itor = m_socket_key.right.find(key);
      if (itor == m_socket_key.right.end())
      {
        return false;
      }
      closehandle(itor->second);
      m_socket_key.right.erase(itor);
      return true;
    }

    /*
	 *  重连
	 */
    bool reconnect(SOCKET aisocket)
    {
      /* 移除 */
      auto itor = m_socket_key.left.find(aisocket);
      if (itor == m_socket_key.left.end())
      {
        return false;
      }

      if (itor->second.get_stat())
      {
        (const_cast<key_ip_port*>(&itor->second))->set_stat(false);
        closehandle(aisocket);
        boost::thread(
          boost::bind(
            &connect_key_socket::send_key,
            this,
            itor->second.m_key,
            itor->second.m_ip.c_str(),
            itor->second.m_port,
            boost::ref(itor->second.m_sendfailure_callback)
            )
          );
      }
      return true;
    }
  };

} //namespace middleware
#endif //CONNECT_KEY_SOCKET_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
