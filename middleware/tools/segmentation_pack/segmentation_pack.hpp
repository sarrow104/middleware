///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SEGMENTATION_PACK_H
#define SEGMENTATION_PACK_H

#include <boost/thread.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/function.hpp>

#include <cstdint>
#include <unordered_map>


#define MAKE_POOL(TYPE,NAME)				typedef boost::singleton_pool<pool_tag, sizeof(TYPE)> NAME;
#define MALLOC_POOL(TYPE,NAME)				(TYPE*)NAME::malloc()
#define FREE_POOL(TYPE,NAME,TYPE_PTR)		NAME::free(TYPE_PTR)

#ifndef RESERVED_AREA_SIZE
# define RESERVED_AREA_SIZE		(0)							/** �������� */
#endif //RESERVED_AREA_SIZE

#define SINGLE_DATA_SIZE		(1024)						/** �������ݵ��ֽ� */
#define GET_LEN( DATA )			*( (uint32_t*)(DATA) )

typedef unsigned long IP_ADDRESS_TYPE;



namespace middleware{
  namespace tools{

    /** ��������MAKE_POOL�� ���ǿ��� */
    struct pool_tag {};
    /** ���ڱ���δ������������ */
    struct  not_recv
    {
      uint32_t m_size;
      char  m_dataarr[SINGLE_DATA_SIZE];
      char* m_data;
      void set()
      {
        m_data = m_dataarr + RESERVED_AREA_SIZE;
      }
    };
    /** �����ڴ��� */
    MAKE_POOL(not_recv, pool_not_recv);			/** �ڴ��� */

    /**
     *  ����TCPճ������
     *  �ְ�ģ��
     */
    template <typename T>
      class  segmentation_pack
      {
        typedef std::unordered_map< IP_ADDRESS_TYPE , not_recv*  >  type_ump;
        type_ump* m_ump;
        boost::function<bool(T,char*,uint32_t)>* m_logic_fun;
        boost::mutex* m_lock;

        bool every_seg( T aithis,char*& aidata , uint32_t& aidatalen , bool& airet )
        {
          char* ldata_copy = aidata;
          uint32_t ldatalen_copy = aidatalen;

          /* ��ȡlen */
          uint32_t llen;
          if( ldatalen_copy > sizeof( uint32_t ) )
          {
            llen = GET_LEN( ldata_copy );
          }
          else
          {
            return false; /* �����Ѿ��ָ����� */
          }

          /* ��������len�Ƿ��Ϸ� */
          if( llen > SINGLE_DATA_SIZE || llen == 0  )
          {
            airet = false;
          }

          if( ldatalen_copy >= llen )
          {
            aidatalen = ldatalen_copy- llen;
            aidata =  ldata_copy;
            aidata += llen;
            airet = (*m_logic_fun)( aithis , ldata_copy , llen );
            return true;
          }
          else
          {
            return false;
          }
        }


        /* hash���д��� */
        bool every_seg( T aithis,type_ump::iterator& itor , char*& aidata , uint32_t& aidatalen , bool& airet)
        {
          /* ��ȡlen */
          uint32_t llen;
          if( itor->second->m_size >= sizeof( uint32_t ) )
          {
            llen = GET_LEN( itor->second->m_data );
          }
          else
          {
            if( itor->second->m_size + aidatalen > sizeof( uint32_t ) )
            {
              uint32_t litemp = sizeof( uint32_t ) - itor->second->m_size;
              memcpy( &( itor->second->m_data[ itor->second->m_size ] ) , aidata , litemp  );
              itor->second->m_size = sizeof( uint32_t );

              aidata += litemp;
              aidatalen -= litemp;
              llen = GET_LEN( itor->second->m_data );
            }
            else
            {
              return false;
            }
          }

          /* ��������len�Ƿ��Ϸ� */
          if( llen > SINGLE_DATA_SIZE || llen == 0  )
          {
            airet = false;
          }

          /* ������ʵ�ܳ��� llen */
          /* �ѽ������ݳ��� itor->second->m_size + ldatalen_copy */
          if( llen <= itor->second->m_size + aidatalen )
          {
            /* ����Ҫ������������ lhavedatalen */
            uint32_t lhavedatalen = llen - itor->second->m_size;
            memcpy( &( itor->second->m_data[ itor->second->m_size ] ) , aidata , lhavedatalen  );
            aidata += lhavedatalen;
            aidatalen -= lhavedatalen;
            airet = (*m_logic_fun)( aithis , itor->second->m_data , itor->second->m_size );

            return true;
          }
          else/* û����ȫ */
          {
            memcpy( &( itor->second->m_data[ itor->second->m_size ] ) , aidata , aidatalen  );
            itor->second->m_size += aidatalen;
            aidata += aidatalen;
            aidatalen = 0 ;
            return false;
          }
        }

        segmentation_pack();
        segmentation_pack( const segmentation_pack&);
        bool segmentation_data( T aithis,IP_ADDRESS_TYPE aiip , char* aidata , uint32_t aidatalen )

        {
          char* ldata_copy = aidata;
          uint32_t ldatalen_copy = aidatalen;
          bool lbret1 = true;
          bool lbret2 = true;

          while( 1 )
          {
            lbret2 = every_seg( aithis , ldata_copy , ldatalen_copy , lbret1);
            if( !lbret1 )/* ���ݴ��� */
            {
              return false;
            }

            if( !lbret2 )
            {
              if( ldatalen_copy != 0 )
              {
                /* ����stl �������� insert����ֵ */
                not_recv* lp = MALLOC_POOL(not_recv, pool_not_recv);

                // NOTE
                // gcc is more strict about const reference from a tmp rvalue;
                // comment-out by sarrow104 2016-02-03
                // type_ump::iterator& itor = m_ump->insert( std::make_pair( aiip , lp )).first;
                const type_ump::iterator& itor = m_ump->insert( std::make_pair( aiip , lp )).first;
                itor->second->set();
                memcpy( itor->second->m_data , ldata_copy , ldatalen_copy );
                itor->second->m_size = ldatalen_copy;
              }
              else
              {
                /* �ͷŷ�ʽ */
                /* 1.ֱ���ͷ�ȫ����Դ*/
                type_ump::iterator itor;
                if( find_ump( itor , aiip ) )
                {
                  FREE_POOL( not_recv , pool_not_recv, itor->second );
                  m_ump->erase( itor );
                }
              }
              break;
            }
          }
          return true;
        }


        bool segmentation_data( T aithis,type_ump::iterator& itor , IP_ADDRESS_TYPE aiip , char* aidata , uint32_t aidatalen )
        {
          char* ldata_copy = aidata;
          uint32_t ldatalen_copy = aidatalen;
          bool lbret1 = true;
          bool lbret2 = true;

          lbret2 = every_seg( aithis , itor , ldata_copy , ldatalen_copy , lbret1);
          if( !lbret1 )/* ���ݴ��� */
          {
            FREE_POOL( not_recv , pool_not_recv, itor->second );
            m_ump->erase( itor );
            return false;
          }

          if( !lbret2 )/* ˵���Ѿ��޿������� */
          {
            return true;
          }

          return segmentation_data( aithis , aiip , ldata_copy , ldatalen_copy );

        }

      public:
        segmentation_pack( boost::function<bool(T,char*,uint32_t)> ailogic_fun ):
          m_logic_fun( new boost::function<bool(T,char*,uint32_t)>() ),
          m_ump( new type_ump() ),
          m_lock( new boost::mutex() )
        {
          *m_logic_fun = ailogic_fun;
        }


        ~segmentation_pack(){
          delete m_ump;
          delete m_lock;
          delete m_logic_fun;
        }


        /* ����ip��Ӧ����Դ*/
        bool find_ump( type_ump::iterator& itor , IP_ADDRESS_TYPE aiip  ){
          itor = m_ump->find( aiip );
          return itor != m_ump->end() ;
        }


        bool segmentation(T aithis,IP_ADDRESS_TYPE aiip , char* aidata , uint32_t aidatalen)
        {
          boost::mutex::scoped_lock lock(*m_lock);

          type_ump::iterator itor;
          bool lbret1 = true;
          bool lbret2 = true;

          if(  find_ump( itor , aiip ) )
          {
            /* �ҵ� */
            return segmentation_data( aithis , itor, aiip , aidata , aidatalen );
          }
          else
          {
            //if( aidatalen >= protocol_server_head::POS::END_POS - server_head::POS::END_POS )
            //{
            return segmentation_data( aithis , aiip , aidata , aidatalen );
            //}
            //else
            //{
            //	return false;
            //}
          }

        }
        /* ǿ��ɾ��ĳ��ip����ռ�õ���Դ*/
        void close( IP_ADDRESS_TYPE aiip ){
          boost::mutex::scoped_lock lock(*m_lock);

          if( m_ump == NULL)
          {
            return ;
          }

          type_ump::iterator itor = m_ump->find( aiip );
          if( itor != m_ump->end() )
          {
            FREE_POOL( not_recv , pool_not_recv, itor->second );
            m_ump->erase( itor );
          }
        }
      };

  } //namespace tools
} //namespace middleware
#endif //SEGMENTATION_PACK_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
