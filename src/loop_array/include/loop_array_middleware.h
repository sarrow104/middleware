#ifndef LOOP_ARRAY_MIDDLEWARE_H
#define LOOP_ARRAY_MIDDLEWARE_H

#include "looparray.h"
#include <map>
#ifndef MIDDLEWARE_BASE
#define MIDDLEWARE_BASE

class middleware_base
{
public:
	virtual bool send(char* apdata, uint32_t aiwlen) = 0;
	virtual bool close() = 0;
};

#endif //MIDDLEWARE_BASE


/* loop array server */
class middleware_la
{
	module_communicate la1;
	module_communicate la2;
public:
	middleware_la(
		uint32_t apbuffersize1,
		uint32_t aieverymaxsize1,
		FUN_READ_CALLBACK aireadfun1,
		bool apstartthread1,
		uint32_t apbuffersize2,
		uint32_t aieverymaxsize2,
		FUN_READ_CALLBACK aireadfun2,
		bool apstartthread2
		):
		la1(apbuffersize1, aieverymaxsize1, aireadfun1, apstartthread1),
		la2(apbuffersize2, aieverymaxsize2, aireadfun2, apstartthread2)
	{}

	module_communicate& get_1()
	{
		return la1;
	}

	module_communicate& get_2()
	{
		return la2;
	}
};


/* loop array client */
class middleware_looparray:
	public middleware_base
{
	static std::map<string, pair<module_communicate*, module_communicate*> > m_module_communicate_tab;
	module_communicate la;
	module_communicate* mpmc;
	std::string m_name;
	static boost::mutex m_lock;
public:
	middleware_looparray(
		const char* ainame,
		uint32_t apbuffersize,
		uint32_t aieverymaxsize,
		FUN_READ_CALLBACK aireadfun,
		bool apstartthread,
		bool apisclient
		) :
		la(apbuffersize, aieverymaxsize, aireadfun, apstartthread),
		m_name(ainame),
		mpmc(nullptr)
	{
			auto itor = m_module_communicate_tab.find(m_name);
		
		
			m_lock.lock();
			if (itor == m_module_communicate_tab.end())
			{
				/*´´½¨*/
				if (apisclient)
				{
					m_module_communicate_tab.insert(std::make_pair(ainame, std::make_pair(&la, nullptr)));
				}
				else
				{
					m_module_communicate_tab.insert(std::make_pair(ainame, std::make_pair(nullptr,&la)));
				}
				itor = m_module_communicate_tab.find(m_name);
			}
			else
			{
				if (apisclient)
				{
					mpmc = itor->second.first;
				}
				else
				{
					mpmc = itor->second.second;
				}
				
			}
			m_lock.unlock();
			
			while (1)
			{
				
				mpmc = (apisclient ? itor->second.second : itor->second.first );
				if (mpmc == nullptr)
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(20));
				}
				else
				{
					break;
				}
			}
	}

	virtual bool send(char* apdata, uint32_t aiwlen)
	{
		return la.send(apdata, aiwlen);
	}

	virtual bool close()
	{
		return la.close();
	}
};







#endif //LOOP_ARRAY_MIDDLEWARE_H