
#ifndef MGT_SOCKET_SHARED_MEMORY_H
#define MGT_SOCKET_SHARED_MEMORY_H

#include "shared_memory_client.h"
#include "shared_memory_server.h"

class communicate_module
{
	gateway_shared_memory* m_client_gsm;
	gateway_shared_memory* m_server_gsm;
	uint64_t  m_client_byte_sum;
	uint64_t  m_server_byte_sum;
	shared_memory_base* m_client;
	shared_memory_base* m_server;
	std::string m_namestr;
	uint32_t m_everyone_maxsize;
	boost::function<bool(char*, uint32_t&)> m_recv_callbackfun;
	bool m_ok;

	gateway_shared_memory* create_shared_memory_obj(const char* aismname, uint64_t  ai_client_byte_sum)
	{
		gateway_shared_memory* lret_gsm = new gateway_shared_memory();
		if (!lret_gsm->open_shared_memort_head(aismname))
		{
			lret_gsm->create_shared_memort_head(aismname, ai_client_byte_sum);
		}
		return lret_gsm;
	}

	void set_callback()
	{
		if (m_recv_callbackfun != NULL)
		{
			get_client()->set_callback(m_recv_callbackfun);
		}
	}

	shared_memory_server* get_server()
	{
		return (shared_memory_server*)m_server;	
	}
	shared_memory_client* get_client()
	{
		return (shared_memory_client*)m_client;
	}

public:
	communicate_module(
		const char* aismname,
		uint64_t  ai_client_byte_sum,
		uint64_t ai_server_byte_sum,
		uint32_t aieveryonemaxsize,
		boost::function<bool(char*, uint32_t&)> aifun,
		bool aiok = true
		):
		m_client_byte_sum(ai_client_byte_sum),
		m_server_byte_sum(ai_server_byte_sum),
		m_everyone_maxsize(aieveryonemaxsize),
		m_recv_callbackfun(aifun),
		m_namestr(aismname),
		m_ok(aiok)
	{
		/*定义共享内存对象*/
		
		m_client_gsm = create_shared_memory_obj((m_namestr + "_client").c_str(), m_client_byte_sum);
		m_server_gsm = create_shared_memory_obj((m_namestr + "_server").c_str(), m_server_byte_sum);
		
		set_operator_shared_memory();
		set_callback();
	}

	void set_operator_shared_memory()
	{
		if (m_ok)
		{
			m_client = new shared_memory_client(m_server_gsm->address(), m_client_byte_sum, m_everyone_maxsize, (m_namestr + "c").c_str());
			m_server = new shared_memory_server(m_client_gsm->address(), m_server_byte_sum, m_everyone_maxsize, (m_namestr + "s").c_str());
		}
		else
		{
			m_server = new shared_memory_server(m_server_gsm->address(), m_client_byte_sum, m_everyone_maxsize, (m_namestr + "c").c_str());
			m_client = new shared_memory_client(m_client_gsm->address(), m_server_byte_sum, m_everyone_maxsize, (m_namestr + "s").c_str());
		}
		
		
	}


	/* 读进程专用 */
	char* rget_strat(uint32_t& ailen)
	{
		return get_client()->rget_strat(ailen);
	}

	void rget_finish()
	{
		get_client()->rget_finish();
	}


	/* 写进程专用 */
	char* wget_start()
	{
		return get_server()->wget_start();
	}

	void wget_finish(uint32_t aiwlen)
	{
		get_server()->wget_finish(aiwlen);
	}

};


#if 0
struct server_base 
{
	shared_memory_client* m_client;
	shared_memory_server* m_server;

	server_base(const char* aismname ,gateway_shared_memory* ai_client_gsm, uint32_t aieveryonemaxsize,gateway_shared_memory* ai_server_gsm )
	{
		std::string lnamestr( aismname ) ;
		m_client = new shared_memory_client( ai_server_gsm->address() , ai_server_gsm->size() ,aieveryonemaxsize, (lnamestr + "c").c_str() ,  true );
		m_server = new shared_memory_server( ai_client_gsm->address() , ai_client_gsm->size() ,aieveryonemaxsize, (lnamestr + "s").c_str() , true );
	}
};

struct client_base
{
	shared_memory_client* m_client;
	shared_memory_server* m_server;

	client_base( const char* aismname ,gateway_shared_memory* ai_client_gsm, uint32_t aieveryonemaxsize,gateway_shared_memory* ai_server_gsm  )
	{
		std::string lnamestr( aismname ) ;
		m_server = new shared_memory_server( ai_server_gsm->address() , ai_server_gsm->size() , aieveryonemaxsize, (lnamestr + "c").c_str() );
		m_client = new shared_memory_client( ai_client_gsm->address() , ai_client_gsm->size() , aieveryonemaxsize, (lnamestr + "s").c_str() );
	}
};


template <typename T_TYPE >
class mgt_shared_memory_base
{
	mgt_shared_memory_base();
	mgt_shared_memory_base( const mgt_shared_memory_base&);

	void set_callback( boost::function<bool ( char* ,uint32_t& )> aifun)
	{
		if( aifun != NULL )
		{
			m_shared_memory_type->m_client->set_callback( aifun );
		}
	}


protected:
	gateway_shared_memory* m_client_gsm;
	gateway_shared_memory* m_server_gsm;
	uint64_t  m_client_byte_sum ;
	uint64_t  m_server_byte_sum ;
	T_TYPE* m_shared_memory_type;

public:

	mgt_shared_memory_base( const char* aismname , uint64_t  ai_client_byte_sum , uint64_t ai_server_byte_sum , uint32_t aieveryonemaxsize, boost::function<bool( char* ,uint32_t& )> aifun  ):
		 m_client_byte_sum( ai_client_byte_sum ),
		 m_server_byte_sum( ai_server_byte_sum )
	{
		std::string lnamestr( aismname ) ;
		m_client_gsm = new gateway_shared_memory( (lnamestr + "_client").c_str() , ai_client_byte_sum );
		m_server_gsm = new gateway_shared_memory( (lnamestr + "_server").c_str() , ai_server_byte_sum );
		m_shared_memory_type = new T_TYPE( aismname , m_client_gsm ,aieveryonemaxsize, m_server_gsm );

		set_callback( aifun );
	}

	gateway_shared_memory* create_shared_memory_obj( const char* aismname, uint64_t  ai_client_byte_sum )
	{
		gateway_shared_memory* lclient_gsm = new gateway_shared_memory();
		if (lclient_gsm->open_shared_memort_head(aismname))
		{
			lclient_gsm->create_shared_memort_head();
		}
	}


	mgt_shared_memory_base( const char* aismname , uint64_t  ai_client_byte_sum, uint64_t ai_server_byte_sum, uint32_t aieveryonemaxsize,  boost::function<bool ( char* ,uint32_t& )> aifun )
	{
		std::string lnamestr( aismname ) ;
		m_client_gsm = create_shared_memory_obj( (lnamestr + "_client").c_str(), );
		m_server_gsm = new gateway_shared_memory();

		m_shared_memory_type = new T_TYPE( aismname ,m_client_gsm , aieveryonemaxsize, m_server_gsm   );

		set_callback( aifun );
	}




	/* 读进程专用 */
	char* rget_strat(uint32_t& ailen)
	{
		return m_shared_memory_type->m_client->rget_strat( ailen );
	}

	void rget_finish()
	{
		 m_shared_memory_type->m_client->rget_finish();
	}


	/* 写进程专用 */
	char* wget_start()
	{
		return m_shared_memory_type->m_server->wget_start();
	}

	void wget_finish( uint32_t aiwlen)
	{
		m_shared_memory_type->m_server->wget_finish( aiwlen );
	}

	void print()
	{
		std::cout << "____________________________server__________________________" << std::endl;
		m_shared_memory_type->m_server->print();

		std::cout << "____________________________client__________________________" << std::endl;
		m_shared_memory_type->m_client->print();
		std::cout << "____________________________________________________________" << std::endl;
	}

};


class mgt_shared_memory_server:
	public mgt_shared_memory_base< server_base >
{
	mgt_shared_memory_server();
	mgt_shared_memory_server( const mgt_shared_memory_server&);
	
public:
	mgt_shared_memory_server( const char* aismname , uint64_t  ai_client_byte_sum , uint64_t ai_server_byte_sum , uint32_t aieveryonemaxsize, boost::function<bool ( char* ,uint32_t& )> aifun ):
		mgt_shared_memory_base< server_base >( aismname , ai_client_byte_sum , ai_server_byte_sum , aieveryonemaxsize, aifun )
	{

	}
};



class mgt_shared_memory_client:
	public mgt_shared_memory_base< client_base >
{
	mgt_shared_memory_client();
	mgt_shared_memory_client( const mgt_shared_memory_client&);

public:
	mgt_shared_memory_client( const char* aismname , uint32_t aieveryonemaxsize, boost::function<bool ( char* ,uint32_t& )> aifun ):
		mgt_shared_memory_base< client_base >( aismname , aieveryonemaxsize ,aifun )
	{

	}

};


#endif //0

#endif