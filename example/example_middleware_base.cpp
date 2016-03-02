//        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#include "middleware/middleware_base/middleware_base.hpp"
#include "middleware/middleware_base/socket_asio/socket_asio_server_arg.hpp"

#include <iostream>

using std::cout;
using std::endl;


void test_middleware_sm_server()
{
  /** boost::function<bool ( char* ,uint32_t& )> */
  auto funservercallback = [](const char* ap, uint32_t aplen)
  {
    cout << ap << endl;
    return true;
  };
  middleware::middleware_sm_server  abc("kk", 1024, 1024, 128, funservercallback);
  uint32_t lnum = 900;
  char cbuf[128];
  uint32_t litemp;
  while (lnum)
  {
    litemp = sprintf(cbuf, "server2client %d", lnum--);
    abc.send(cbuf, litemp + 1);
  }
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
}

void test_middleware_sm_client()
{
  /** boost::function<bool ( char* ,uint32_t& )> */
  auto funclientcallback = [](const char* ap, uint32_t aplen)
  {
    cout << ap << endl;
    return true;
  };
  middleware::middleware_sm_client  abc("kk", 1024, 1024, 128, funclientcallback);

  uint32_t lnum = 0;
  char cbuf[128];
  uint32_t litemp;
  while (lnum != 900)
  {
    litemp = sprintf(cbuf, "client2server %d", lnum++);
    abc.send(cbuf, litemp + 1);
  }
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
}

void test_middleware_la_server()
{
  auto funs = [](const char* ap, uint32_t aplen)
  {
    cout << "[" << ap << "]"<< endl;
    return true;
  };
  middleware::middleware_la_server lmc("lb", 10240, 1024, funs, false);
  uint32_t lsize = 3;
  char lchar1[64];
  char lchar2[64];
  char*p1 = lchar1;
  char*p2 = lchar2;
  uint32_t llen = 0;
  while (lsize--)
  {
    std::swap(p1, p2);
    llen = snprintf(p1, 64, "server%d", lsize);
    lmc.send(p1, llen + 1);
  }
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
}

void test_middleware_la_client()
{
  auto func = [](const char* ap, uint32_t aplen)
  {
    cout << "[" << ap << "]" << endl;
    return true;
  };
  middleware::middleware_la_client lmc("lb", 10240, 1024, func, false);
  uint32_t lsize = 3;
  char lchar1[64];
  char lchar2[64];
  char*p1 = lchar1;
  char*p2 = lchar2;
  uint32_t llen = 0;
  while (lsize--)
  {
    std::swap(p1, p2);
    llen = snprintf(p1, 64, "client%d", lsize);
    lmc.send(p1, llen + 1);
  }
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
}


/** recv call back */
bool rcb(bool aisclient, uint32_t aikey, const char* ap, uint32_t aplen)
{
  if (!aisclient)
  {
    cout << *((uint32_t*)ap) << endl;
  }
  else
  {
    cout << ap << endl;
  }
  aplen = 0;
  return true;
};

/** send failure call back*/
bool sfcb(const char* ap, uint32_t aplen)
{
  cout << "send err" << endl;
  aplen = 0;
  return true;
};

void test_middleware_soio_server()
{

  middleware::middleware_soio_server lserver(13140, boost::bind(&rcb, true, _1, _2,_3), 10240, 1024, sfcb);
  uint32_t lnum = 0;
  while (1)
  {
    lserver.send(0, (const char*)&(++lnum), sizeof(uint32_t));
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
  return;
}


void test_middleware_soio_client()
{
  middleware::middleware_soio_client lclient(boost::bind(&rcb,false,_1,_2,_3), 10240, 1024);
  lclient.create_connkey(0, "127.0.0.1", 13140, sfcb);
  char lbuf[] = "hello world";


  while (1)
  {
    lclient.send(0, lbuf, sizeof(lbuf));
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
}




void test_middleware_asio_server()
{
  boost::function<bool(uint32_t,const char*, uint32_t)> apfun = [](uint32_t ainum,const char* ap, uint32_t aplen) {
    middleware::unpack_head_process<middleware::spack_head::protocol_head> luhp;
    luhp.reset( ap, aplen);
    char ch[sizeof("hello world")] = { 0 };
    luhp.pop(ch, sizeof("hello world"));
    std::cout << ch << std::endl;
    middleware::asio_server().send(ainum,ap, aplen);
    return true;
  };

  std::vector<boost::function<bool(const char*, uint32_t)> > ltemp(5);
  for (uint32_t i = 0; i < 5; ++i)
  {
    ltemp[i] = boost::bind(apfun,i,_1,_2);
  }

  middleware::socket_asio_arg larg(5, ltemp);

  larg.m_activ = false;
  larg.m_extern_activ = false;
  larg.m_everyoncemaxsize = 1024;
  larg.m_extern_everyoncemaxsize = 1024;
  larg.m_extern_loopbuffermaxsize = 10240;
  larg.m_loopbuffermaxsize = 10240;
  larg.m_heartbeat_num = 32;
  larg.m_persecond_recvdatatbyte_num = 1024;
  larg.m_port = 13140;
  larg.m_recvpack_maxsize = 1024;
  larg.m_timeout = 10240;
  larg.m_s2c = true;
  larg.m_s2s = true;
  larg.m_session_num = 10240;

  middleware::middleware_asio_server lser(larg);
  middleware::asio_server( &lser );
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
}


void test_middleware_asio_client()
{
  middleware::middleware_asio_client lclient(boost::bind(&rcb, false, _1, _2, _3), 10240, 1024);
  lclient.create_connect(0, "127.0.0.1", 13140, sfcb);

  middleware::pack_head_process<middleware::cpack_head::protocol_head > lphp(sizeof("hello world"));
	char ch[] = "hello world";
	lphp.push(ch,sizeof("hello world"));
  lphp.set_pack_head();
  while (1)
  {
    lclient.send(0, lphp.get_send_buffer(), lphp.get_send_len());
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
  while (1)
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
  }
}


int main(int argc, char *argv[])
{
  if (argc >= 3)
  {

    if (memcmp(argv[1], "-sm", sizeof("-sm")) == 0)/** 共享内存测试 */
    {
      if (memcmp(argv[2], "-c", sizeof("-c")) == 0)
      {
        test_middleware_sm_client();
      }
      else if (memcmp(argv[2], "-s", sizeof("-s")) == 0)
      {
        test_middleware_sm_server();
      }
    }
    else if (memcmp(argv[1], "-la", sizeof("-la")) == 0)
    {
      boost::thread(boost::function<void()>(test_middleware_la_server));
      boost::thread(boost::function<void()>(test_middleware_la_client));
    }
    else if (memcmp(argv[1], "-soio", sizeof("-soio")) == 0)
    {
      if (memcmp(argv[2], "-c", sizeof("-c")) == 0)
      {
        test_middleware_soio_client();
      }
      else if (memcmp(argv[2], "-s", sizeof("-s")) == 0)
      {
        test_middleware_soio_server();
      }
    }
    else if (memcmp(argv[1], "-asio", sizeof("-asio")) == 0)
    {
      if (memcmp(argv[2], "-c", sizeof("-c")) == 0)
      {
        test_middleware_asio_client();
      }
      else if (memcmp(argv[2], "-s", sizeof("-s")) == 0)
      {
        test_middleware_asio_server();
      }
    }

    getchar();
    return 0;

  }


  cout << "/** " << endl;
  cout << " *  THIS.EXE -TYPE  -CLIENT OR SERVER " << endl;
  cout << " *  -TYPE      -sm    [-c|-s]  共享内存" << endl;
  cout << " *             -la    0    循环数组" << endl;
  cout << " *             -soio  [-c|-s]  简单socket" << endl;
  cout << " *" << endl;
  cout << " *" << endl;
  cout << " * example:" << endl;
  cout << " *          THIS.EXE -sm -c " << endl;
  cout << " *" << endl;
  cout << " */" << endl;

  return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
