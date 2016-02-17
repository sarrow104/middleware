///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_ASIO_SERVER_ARG_H
#define SOCKET_ASIO_SERVER_ARG_H

#include <cstdint>
#include <boost/function.hpp>

namespace middleware {

  class middleware_base;
  /**
   *  需要设定的参数 
   */
  struct socket_asio_arg
  {
  private:
    uint32_t m_threadmaxsize;                                   /** 线程数 */
    uint32_t m_thread_pos;
		std::vector< boost::function<bool(const char*, uint32_t)> > m_callbackfun;     /** 回调 */
  public:
    uint32_t m_timeout;                                         /** 连接无活动的存活时间 单位秒 */
    uint16_t m_port;                                            /** 端口号 */
    uint32_t m_recvpack_maxsize;                                /** 最大单个包大小 */
    std::vector<middleware_base*> m_middlewarearr;                          /** 返回参数,用于发送 个数与线程数一致 */
    uint32_t m_session_num;                                     /** 也就是最大连接数 */
    uint32_t m_loopbuffermaxsize;                               /** 环形数组缓冲大小 */
    uint32_t m_everyoncemaxsize;                                /** 单条数据最大大小 */
    uint32_t m_extern_loopbuffermaxsize;                        /** 回传的外部中间件的环形数组缓冲大小 */
    uint32_t m_extern_everyoncemaxsize;                         /** 回传的外部中间件的单条数据最大大小 */
    bool m_activ;                                               /** 是否自我保证数据活性 */
    bool m_extern_activ;
    uint32_t m_persecond_recvdatatbyte_num;                     /** 每秒钟接收的字节数 */
    bool m_s2s;                                                 /** 服务器与服务器的连接断开是否通知上层(此处两个服务器分别是:1.自身,2.集群中的某个其他节点(在1中的体现就是不受time out限制)) */
    bool m_s2c;                                                 /** 服务器与客户端的连接断开是否通知上层 */
    uint32_t m_heartbeat_num;                                   /** 心跳协议号,收到后重置time out时间，然后丢弃 */

    socket_asio_arg(uint32_t aithreadsize, boost::function<bool(const char*, uint32_t)> aifun) :
      m_thread_pos(0)
    {
      m_threadmaxsize = aithreadsize;
      m_callbackfun.resize(aithreadsize, aifun);
			m_middlewarearr.resize(aithreadsize);
    }

    uint32_t get_thread_pos()
    {
      return m_thread_pos++;
    }

    ~socket_asio_arg()
    {
    }

    uint32_t get_thread_maxsize()const
    {
      return m_threadmaxsize;
    }

		const boost::function<bool(const char*, uint32_t)>& get_callbackfun(uint32_t ainum)
		{
			if (m_callbackfun.size() > ainum)
			{
				return m_callbackfun[ainum];
			}
			
		}
  };

};  //namespace middleware_base
#endif //SOCKET_ASIO_SERVER_ARG_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
