///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef ALONE_LIST_H
#define ALONE_LIST_H

#include "middleware/socket_io/lpthread.hpp"

#include <boost/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#include <vector>

//2
#include <list>
#include <map>

#include <boost/multi_index_container.hpp> 
#include <boost/multi_index/member.hpp> 
#include <boost/multi_index/ordered_index.hpp> 


namespace middleware {

/**
 *  普通链表,但是支持多线程获取元素和释放元素
 *  获取和添加元素加锁同步
 *  缺点 锁粒度较大
 */
template <typename T>
struct alonelist
{
	static std::vector<T* > arr;
	static boost::mutex lock;
	static boost_sem mo_semls;
	alonelist();
public:
	typedef T  type_value;
	template <typename T_ARRAY>
	static void create(T_ARRAY& aivec)
	{
		arr.resize( aivec.size() );
		std::copy( aivec.begin() , aivec.end() , arr.begin() );
		return ;
	}

	static T* get_instance()
	{
		while(1)
		{
			while(1)
			{
				boost::mutex::scoped_lock llock(lock);
				{/** 锁作用域 */
					if( arr.empty() )
					{
						break;
					}

					T* ret = *arr.rbegin();
					arr.pop_back();
				}/** 锁作用域 */
				return ret;
			}
			mo_semls.wait();
		}
		
	}

	static void release_instance(T* aiptr)
	{
		boost::mutex::scoped_lock lolock(lock);
		arr.push_back( aiptr );		
		mo_semls.post();
		return;
	}

};

template <typename T>
boost::mutex alonelist<T>::lock;

template <typename T>
std::vector<T* > alonelist<T>::arr;

template <typename T>
boost_sem alonelist<T>::mo_semls(0);













/**
 *  普通链表,但是支持多线程获取元素和释放元素
 *  获取和添加元素加锁同步
 *  缺点 锁粒度较大
 */
template <typename T>
struct alonelist2
{
	std::vector<T* > arr1;
	std::vector<T* > arr2;
	boost::mutex lock;
	boost_sem mo_semls;
	uint32_t m_size;
public:
	typedef T  type_value;
	alonelist2():
		mo_semls( 0 )
	{}


	template <typename T_ARRAY>
	void create(T_ARRAY& aivec)
	{
		arr1.resize( aivec.size() );
		arr2.reserve( aivec.size() );
		std::copy( aivec.begin() , aivec.end() , arr1.begin() );
		m_size = arr1.size();
		return ;
	}

	T* get_instance()
	{
		while(1)
		{
			if( arr1.empty() )
			{
				if( arr2.empty() )
				{
					mo_semls.wait();
					continue;
				}
				else
				{
					boost::mutex::scoped_lock llock(lock);
					{/** 锁作用域 */
						arr1.swap(arr2);
					}/** 锁作用域 */
				}
			}

			T* ret = *arr1.rbegin();
			arr1.pop_back();
			--m_size;
			return ret;
		}
	}

	void release_instance(T* aiptr)
	{
		boost::mutex::scoped_lock lolock(lock);
		arr2.push_back( aiptr );
		++m_size;
		mo_semls.post();
		return;
	}


	uint32_t size()const
	{
		return m_size;
	}
};









struct key_values
{
	uint32_t m_key_count;
	uint32_t m_values_pos;
};

struct stu_count{};   // 索引-uid 
struct stu_pos{};  // 索引-uname  
  
typedef  boost::multi_index::multi_index_container<
		key_values,  
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_non_unique<  boost::multi_index::tag<stu_count>, BOOST_MULTI_INDEX_MEMBER(key_values,uint32_t,m_key_count)> ,
		boost::multi_index::ordered_unique<  boost::multi_index::tag<stu_pos>,  BOOST_MULTI_INDEX_MEMBER(key_values,uint32_t,m_values_pos)>
	 >  
 > map_key_values;  

typedef map_key_values::index<stu_count>::type  map_key_values__count; 
typedef map_key_values::index<stu_pos>::type  map_key_values__pos;




template <typename T>
struct alonelist3
{
	static std::vector< std::vector<T* > > arr;
	static std::vector< std::vector<T* > > arr_copy;
	static std::vector< std::vector<T* > >* parr;
	static std::vector< std::vector<T* > >* parr_copy;

	static map_key_values arr_key_size;
	static boost::mutex lock;
	static boost_sem mo_semls;
	static uint32_t m_count;
	
public:
	typedef T  type_value;
	alonelist3()
	{
	}
	
	template <typename T_ARRAY>
	static void create(T_ARRAY& aivec)
	{
		arr.resize( ++m_count );
		arr_copy.resize( m_count );
		arr[ m_count-1 ].resize( aivec.size() );
		arr_copy[ m_count-1 ].reserve( aivec.size() );
		std::copy( aivec.begin() , aivec.end() , arr[ m_count-1 ].begin() );

		key_values ltemp;
		ltemp.m_key_count = 0;
		ltemp.m_values_pos = m_count - 1;
		arr_key_size.insert( ltemp );
		
		return ;
	}

	static T* get_instance()
	{
		map_key_values__count::iterator  itor;
		key_values lkv;
		while(1)
		{
			map_key_values__count& lkvc = arr_key_size.get<stu_count>();
			itor = lkvc.begin();
			lkv = *itor;
			if( ( (*parr)[ lkv.m_values_pos ] ).empty() )
			{
				lock.lock();
				if( ( (*parr_copy)[ lkv.m_values_pos ] ).empty() )
				{
					lock.unlock();
					mo_semls.wait();
				}
				else
				{
					std::swap( parr , parr_copy  );
					lock.unlock();
				}
				
				continue;
			}
			
			T* ret = *( (*parr)[ lkv.m_values_pos  ] ).rbegin();
			( (*parr)[ lkv.m_values_pos  ] ).pop_back();

			lkvc.erase( itor );
			++lkv.m_key_count;
			lkvc.insert( lkv );

			return ret/*->data*/;

		}

		
	}

	static void release_instance(T* aiptr)
	{
		boost::mutex::scoped_lock lolock(lock);
		map_key_values__pos& lkvp = arr_key_size.get<stu_pos>();
		map_key_values__pos::iterator  itor = lkvp.find( aiptr->get_groupid() );
		
		if( itor !=  lkvp.end() )
		{
			key_values lkv = *itor;
			lkvp.erase( itor );
			--lkv.m_key_count;
			lkvp.insert( lkv );
			(*parr_copy)[ lkv.m_values_pos ].push_back( aiptr );
		}
		mo_semls.post();
		return;
	}

};


template <typename T>
boost::mutex alonelist3<T>::lock;


template <typename T>
boost_sem alonelist3<T>::mo_semls(0);

template <typename T>
map_key_values alonelist3<T>::arr_key_size;
template <typename T>
uint32_t alonelist3<T>::m_count = 0;
template <typename T>
std::vector< std::vector<T* > > alonelist3<T>::arr;
template <typename T>
std::vector< std::vector<T* > > alonelist3<T>::arr_copy;

template <typename T>
std::vector< std::vector<T* > >* alonelist3<T>::parr = &( alonelist3<T>::arr );
template <typename T>
std::vector< std::vector<T* > >* alonelist3<T>::parr_copy = &( alonelist3<T>::arr_copy );

} //namespace middleware 
#endif //ALONE_LIST_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */