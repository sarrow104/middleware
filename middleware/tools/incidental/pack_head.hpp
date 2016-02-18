///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef PACK_HEAD_HPP
#define PACK_HEAD_HPP

#include "middleware/tools/incidental/check_crc.hpp"
#include "middleware/tools/serializecpp/serializecpp.hpp"
#include "middleware/tools/serializecpp/unserializecpp.hpp"


#include <cstdint>
#include <vector>

#define GET_HEAD( NAME , TYPE , POS  )      \
  TYPE& get_##NAME ()                       \
{                                           \
  return *( (TYPE*)&( m_data[ POS ] ) );    \
}


namespace middleware{

  
  /* ĺŹĺąĺ¤?*/
  typedef uint32_t    HEAD_SERVER_MAGIC_TYPE;             /* é­ć° */
  typedef uint32_t    HEAD_DATA_BYTE_TYPE;                /* ć°ćŽéżĺşŚ ĺćŹčŞčşŤćĺ ĺ­č?*/
  typedef uint32_t    HEAD_CRC_TYPE;                      /* crcć ĄéŞ */
  typedef uint32_t    HEAD_ERROR_TYPE;                    /* éčŻŻ */
  typedef uint64_t    HEAD_USER_ID_TYPE;                  /* ç¨ćˇid */
  typedef uint32_t    HEAD_PROTOCOL_NUM_TYPE;             /* ĺčŽŽĺ?*/
  typedef uint32_t    HEAD_ORDER_TYPE;                    /* ĺşĺˇ */
  typedef char*       PROTOCOL_BODY_TYPE;                 /* ĺčŽŽä˝?*/


  namespace spack_head{

    typedef uint16_t    HEAD_SERVER_PROTOCOL_LEN_TYPE;          /* ĺčŽŽéżĺşŚ */
    typedef uint32_t    HEAD_IP_ADDRESS_TYPE;                   /* ip */
    typedef uint32_t    HEAD_CLOSESOCKET_COUNT_TYPE;            /* socket closećŹĄć° */
    typedef uint8_t     HEAD_SESSIONTYPE_TYPE;                  /* session çąťĺ */
    typedef uint8_t     HEAD_GROUPID_TYPE;                      /* çťid */
    typedef uint32_t    HEAD_SESSIONID_TYPE;                    /* session id */
    typedef uint16_t    HEAD_MASS_SIZE_TYPE;                    /* çž¤ĺć°çŽçąťĺ */
    typedef uint32_t    HEAD_SERVER_ERROR_TYPE;                 /* éčŻŻ */
    typedef uint8_t     HEAD_SETTIMER_TYPE;                     /* set timer */

                                                                /* čŽžç˝ŽĺŽćśĺ? éťčžć§ĺśčżćĽ */
    enum SET_TIMER__LOGIC_CONTROL
    {
      SET_NO_VALUES,
      SET_TIMER_OPEN,     /* ćĺźĺŽćśĺ?*/
      SET_TIMER_CLOSE,    /* ĺłé­ĺŽćśĺ?*/
      SET_CONNECT_CLOSE,    /* ĺłé­čżćĽ */
    };

#pragma pack(push)  //äżĺ­ĺŻšé˝çść?
#pragma pack(1)     //čŽžĺŽä¸?ĺ­čĺŻšé˝
    struct session_infor
    {
      HEAD_SESSIONID_TYPE m_session_id;
      HEAD_CLOSESOCKET_COUNT_TYPE m_socket_close_count;
      HEAD_GROUPID_TYPE m_group_id;
    };
#pragma pack(pop)   //ć˘ĺ¤ĺŻšé˝çść?


    /** ĺ¤ççž¤ĺ */
    template < typename TYPE_ITEM >
    class mass_send
    {
      TYPE_ITEM* m_data;  /* çž¤ĺĺ°ĺ */
    public:
      mass_send() :m_data(NULL) {}

      void reset(void* aidata)
      {
        m_data = (TYPE_ITEM*)aidata;
      }

      /* čŽžç˝Žçž¤ĺéĺ */
      bool set_mass_send_arr(std::vector< TYPE_ITEM >& aisessionarr)
      {
        if (!aisessionarr.empty())
        {
          memcpy(m_data, aisessionarr.data(), sizeof(TYPE_ITEM) * aisessionarr.size());
        }
        return true;
      }

      TYPE_ITEM* get_item(uint32_t ainum)
      {
        return &(m_data[ainum]);
      }

      /* čˇĺçž¤ĺsession id */
      bool get_mass_send_arr(uint16_t aisize, std::vector< TYPE_ITEM* >& aisessionarr)
      {
        aisessionarr.clear();
        aisessionarr.resize(aisize);
        for (uint16_t i = 0; i < aisize; ++i)
        {
          aisessionarr[i] = get_item(i);
        }
        return true;
      }
    };


    class server_head
    {
      mass_send< session_infor > m_mass;
    protected:
      const char* m_data;
      uint32_t m_size;

      /* čŽžç˝Žçž¤ĺéĺ */
      bool SH_set_mass_send_arr(std::vector< session_infor >& aisessionarr)
      {
        get_mass_size() = (HEAD_MASS_SIZE_TYPE)aisessionarr.size();
        m_mass.reset(const_cast<char*>(&m_data[get_server_protocol_len()]));
        return m_mass.set_mass_send_arr(aisessionarr);
      }

      /* čŽžç˝Žçž¤ĺéĺ */
      bool SH_set_mass_send_arr()
      {
        get_mass_size() = 0;
        return true;
      }

      /* čˇĺçž¤ĺsession id */
      bool SH_get_mass_send_arr(std::vector< session_infor* >& aisessionarr, uint32_t aibytesize)
      {
        if (get_mass_size() > 0 && get_mass_size() * sizeof(session_infor) + get_server_protocol_len() <= aibytesize)
        {
          m_mass.reset(const_cast<char*>(&m_data[get_server_protocol_len()]));
          return m_mass.get_mass_send_arr(get_mass_size(), aisessionarr);
        }
        else
        {
          return false;
        }
      }

      void SH_reset(const char* aidata, uint32_t aisize)
      {
        m_data = aidata;
        m_size = aisize;
      }

    public:
      enum POS
      {
        HEAD_SERVER_PROTOCOL_LEN_POS = 0,
        HEAD_SERVER_ERROR_POS = HEAD_SERVER_PROTOCOL_LEN_POS + sizeof(HEAD_SERVER_PROTOCOL_LEN_TYPE),
        HEAD_CLOSESOCKET_COUNT_POS = HEAD_SERVER_ERROR_POS + sizeof(HEAD_SERVER_ERROR_TYPE),
        HEAD_GROUPID_POS = HEAD_CLOSESOCKET_COUNT_POS + sizeof(HEAD_CLOSESOCKET_COUNT_TYPE),
        HEAD_SESSIONID_POS = HEAD_GROUPID_POS + sizeof(HEAD_GROUPID_TYPE),
        HEAD_SETTIMER_POS = HEAD_SESSIONID_POS + sizeof(HEAD_SESSIONID_TYPE),
        HEAD_IP_ADDRESS_POS = HEAD_SETTIMER_POS + sizeof(HEAD_SETTIMER_TYPE),
        HEAD_MASS_SIZE_POS = HEAD_IP_ADDRESS_POS + sizeof(HEAD_IP_ADDRESS_TYPE),
        END_POS = HEAD_MASS_SIZE_POS + sizeof(HEAD_MASS_SIZE_TYPE),
      };
      /* čˇĺĺčŽŽćťéżĺş?*/
      HEAD_SERVER_PROTOCOL_LEN_TYPE get_server_protocol_len()
      {
        return *((HEAD_SERVER_PROTOCOL_LEN_TYPE*)&(m_data[HEAD_SERVER_PROTOCOL_LEN_POS]));
      }
      void set_server_protocol_len(uint32_t aisize)
      {
        *((HEAD_SERVER_PROTOCOL_LEN_TYPE*)&(m_data[HEAD_SERVER_PROTOCOL_LEN_POS])) = POS::END_POS + aisize;
      }

      /* čˇĺsocket ĺłé­ćŹĄć° */
      GET_HEAD(closesocket_count, HEAD_CLOSESOCKET_COUNT_TYPE, HEAD_CLOSESOCKET_COUNT_POS)

        /* čˇĺçťid */
        GET_HEAD(groupid, HEAD_GROUPID_TYPE, HEAD_GROUPID_POS)

        /* čˇĺsessionid */
        GET_HEAD(sessionid, HEAD_SESSIONID_TYPE, HEAD_SESSIONID_POS)

        /* čˇĺip */
        GET_HEAD(ip, HEAD_IP_ADDRESS_TYPE, HEAD_IP_ADDRESS_POS)

        /* çťéťčžćşäź ä¸čŽžç˝ŽćäşčżćĽçčśćś */
        GET_HEAD(settimer, HEAD_SETTIMER_TYPE, HEAD_SETTIMER_POS)

        /* čˇĺçž¤ĺć°é*/
        GET_HEAD(mass_size, HEAD_MASS_SIZE_TYPE, HEAD_MASS_SIZE_POS)

        /* čˇĺerror*/
        GET_HEAD(server_error, HEAD_SERVER_ERROR_TYPE, POS::HEAD_SERVER_ERROR_POS)
    };

#define PROTOCOL_HEAD_BEG_POS (server_head::END_POS)


    class protocol_head :
      public server_head
    {
      middleware::check_crc m_crc;
    public:
      enum POS
      {
        HEAD_DATA_BYTE_POS = PROTOCOL_HEAD_BEG_POS,
        HEAD_CRC_POS = HEAD_DATA_BYTE_POS + sizeof(HEAD_DATA_BYTE_TYPE),
        HEAD_ERROR_POS = HEAD_CRC_POS + sizeof(HEAD_CRC_TYPE),
        HEAD_USER_ID_POS = HEAD_ERROR_POS + sizeof(HEAD_ERROR_TYPE),
        HEAD_PROTOCOL_NUM_POS = HEAD_USER_ID_POS + sizeof(HEAD_USER_ID_TYPE),
        HEAD_ORDER_POS = HEAD_PROTOCOL_NUM_POS + sizeof(HEAD_PROTOCOL_NUM_TYPE),
        END_POS = HEAD_ORDER_POS + sizeof(HEAD_ORDER_TYPE),
      };

      void reset(const char* aidata, uint32_t aisize)
      {
        SH_reset(aidata, aisize);
      }

      HEAD_DATA_BYTE_TYPE get_data_byte()
      {
        return *((HEAD_DATA_BYTE_TYPE*)&(m_data[POS::HEAD_DATA_BYTE_POS]));
      }

      void set_data_byte(uint32_t aibadyvalues)
      {
        *((HEAD_DATA_BYTE_TYPE*)&(m_data[POS::HEAD_DATA_BYTE_POS])) = aibadyvalues + (END_POS - PROTOCOL_HEAD_BEG_POS);
        set_server_protocol_len(*((HEAD_DATA_BYTE_TYPE*)&(m_data[POS::HEAD_DATA_BYTE_POS])) + get_mass_size() * sizeof(session_infor));
      }

      /* čˇĺcrc */
      GET_HEAD(crc, HEAD_CRC_TYPE, POS::HEAD_CRC_POS)
        /* čˇĺerror */
        GET_HEAD(error, HEAD_ERROR_TYPE, POS::HEAD_ERROR_POS)
        /* čˇĺçŠĺŽśid */
        GET_HEAD(userid, HEAD_USER_ID_TYPE, POS::HEAD_USER_ID_POS)
        /* čˇĺĺčŽŽĺ?*/
        GET_HEAD(protocol_num, HEAD_PROTOCOL_NUM_TYPE, POS::HEAD_PROTOCOL_NUM_POS)
        /* čˇĺéĄşĺşĺ?*/
        GET_HEAD(order, HEAD_ORDER_TYPE, POS::HEAD_ORDER_POS)

        /* čŽžç˝Žcrc */
        void set_crc()
      {
        get_crc() =
          m_crc(
            &(m_data[POS::HEAD_USER_ID_POS]),
            get_data_byte() - (POS::HEAD_USER_ID_POS - PROTOCOL_HEAD_BEG_POS)
            );
      }

      /* ćŁćĽcrc */
      bool check_crc()
      {
        return m_crc(
          get_crc(),
          &(m_data[POS::HEAD_USER_ID_POS]),
          get_data_byte() - (POS::HEAD_USER_ID_POS - PROTOCOL_HEAD_BEG_POS)
          );
      }

      /* čˇĺć°ćŽ*/
      const char* get_buffer()
      {
        return m_data;
      }

      /* čˇĺć°ćŽéżĺşŚ */
      uint32_t get_buffer_len()
      {
        return get_server_protocol_len() + get_mass_size()*sizeof(session_infor);
      }

      /*čˇĺć°ćŽ(ä¸ĺćŹćĺĄĺ¨éŁé¨ĺ? */
      char* get_protocol_buffer()
      {
        return (char*)&(m_data[POS::HEAD_DATA_BYTE_POS]);
      }

      /* čˇĺć°ćŽéżĺşŚ(ä¸ĺćŹćĺĄĺ¨éŁé¨ĺ?  */
      uint32_t get_protocol_buffer_len()
      {
        return get_data_byte();
      }

      /*ĺčŽŽä˝čľˇĺ§ä˝ç˝*/
      char* get_protocol_body()
      {
      return (char*)(&(m_data[POS::END_POS]));
      }

      /* čˇĺć°ćŽéżĺşŚ(ä¸ĺćŹćĺĄĺ¨éŁé¨ĺ?  */
      uint32_t get_protocol_body_len()
      {
        return get_data_byte() - (POS::END_POS - PROTOCOL_HEAD_BEG_POS);
      }

      uint32_t get_protocol_head_len()
      {
        return POS::END_POS;
      }

      /* čŽžç˝Žçž¤ĺéĺ */
      bool set_mass_send_arr(std::vector< session_infor >& aisessionarr)
      {
        return SH_set_mass_send_arr(aisessionarr);
      }
      /* čŽžç˝Žçž¤ĺéĺ */
      bool set_mass_send_arr()
      {
        return SH_set_mass_send_arr();
      }
      /* čˇĺçž¤ĺsession id */
      bool get_mass_send_arr(std::vector< session_infor* >& aisessionarr, uint32_t aibytesize)
      {
        return SH_get_mass_send_arr(aisessionarr, aibytesize);
      }

      void copy_head(protocol_head& ap)
      {
        memcpy((char*)get_buffer(), ap.get_buffer(), protocol_head::POS::END_POS);
      }
      
    };



  } //namespace c_pack_head


  namespace cpack_head {

#define PROTOCOL_HEAD_BEG_POS (0)

    class protocol_head
    {
      middleware::check_crc m_crc;
      const char* m_data;
      uint32_t m_size;
    public:
      enum POS
      {
        HEAD_DATA_BYTE_POS = PROTOCOL_HEAD_BEG_POS,
        HEAD_CRC_POS = HEAD_DATA_BYTE_POS + sizeof(HEAD_DATA_BYTE_TYPE),
        HEAD_ERROR_POS = HEAD_CRC_POS + sizeof(HEAD_CRC_TYPE),
        HEAD_USER_ID_POS = HEAD_ERROR_POS + sizeof(HEAD_ERROR_TYPE),
        HEAD_PROTOCOL_NUM_POS = HEAD_USER_ID_POS + sizeof(HEAD_USER_ID_TYPE),
        HEAD_ORDER_POS = HEAD_PROTOCOL_NUM_POS + sizeof(HEAD_PROTOCOL_NUM_TYPE),
        END_POS = HEAD_ORDER_POS + sizeof(HEAD_ORDER_TYPE),
      };

      void reset(const char* aidata, uint32_t aisize)
      {
        m_data = aidata;
        m_size = aisize;
      }

      HEAD_DATA_BYTE_TYPE get_data_byte()
      {
        return *((HEAD_DATA_BYTE_TYPE*)&(m_data[POS::HEAD_DATA_BYTE_POS]));
      }

      void set_data_byte(uint32_t aibadyvalues)
      {
        *((HEAD_DATA_BYTE_TYPE*)&(m_data[POS::HEAD_DATA_BYTE_POS])) = aibadyvalues + (END_POS - PROTOCOL_HEAD_BEG_POS);
      }

      /* čˇĺcrc */
      GET_HEAD(crc, HEAD_CRC_TYPE, POS::HEAD_CRC_POS)
        /* čˇĺerror */
        GET_HEAD(error, HEAD_ERROR_TYPE, POS::HEAD_ERROR_POS)
        /* čˇĺçŠĺŽśid */
        GET_HEAD(userid, HEAD_USER_ID_TYPE, POS::HEAD_USER_ID_POS)
        /* čˇĺĺčŽŽĺ?*/
        GET_HEAD(protocol_num, HEAD_PROTOCOL_NUM_TYPE, POS::HEAD_PROTOCOL_NUM_POS)
        /* čˇĺéĄşĺşĺ?*/
        GET_HEAD(order, HEAD_ORDER_TYPE, POS::HEAD_ORDER_POS)

        /* čŽžç˝Žcrc */
        void set_crc()
      {
        get_crc() =
          m_crc(
            &(m_data[POS::HEAD_USER_ID_POS]),
            get_data_byte() - (POS::HEAD_USER_ID_POS - PROTOCOL_HEAD_BEG_POS)
            );
      }

      /* ćŁćĽcrc */
      bool check_crc()
      {
        return m_crc(
          get_crc(),
          &(m_data[POS::HEAD_USER_ID_POS]),
          get_data_byte() - (POS::HEAD_USER_ID_POS - PROTOCOL_HEAD_BEG_POS)
          );
      }

      /* čˇĺć°ćŽ*/
      const char* get_buffer()
      {
        return m_data;
      }

      /* čˇĺć°ćŽéżĺşŚ */
      uint32_t get_buffer_len()
      {
        return get_data_byte();

      }

      /*čˇĺć°ćŽ(ä¸ĺćŹćĺĄĺ¨éŁé¨ĺ? */
      char* get_protocol_buffer()
      {
        return (char*)&(m_data[POS::HEAD_DATA_BYTE_POS]);
      }

      /* čˇĺć°ćŽéżĺşŚ(ä¸ĺćŹćĺĄĺ¨éŁé¨ĺ?  */
      uint32_t get_protocol_buffer_len()
      {
        return get_data_byte();
      }

      /*ĺčŽŽä˝čľˇĺ§ä˝ç˝*/
      char* get_protocol_body()
      {
      return (char*)(&(m_data[POS::END_POS]));
      }

      /* čˇĺć°ćŽéżĺşŚ(ä¸ĺćŹćĺĄĺ¨éŁé¨ĺ?  */
      uint32_t get_protocol_body_len()
      {
        return get_data_byte() - (POS::END_POS - PROTOCOL_HEAD_BEG_POS);
      }

      uint32_t get_protocol_head_len()
      {
        return POS::END_POS;
      }

      void copy_head(protocol_head& ap)
      {
        memcpy((char*)get_buffer(), ap.get_buffer(), protocol_head::POS::END_POS);
      }

    };
    
  }//namespace cpack_head
  


 
   /** ÄŁ°ĺ˛ÎĘý 
    * PHÎŞ cpack_head::protocol_head 
    * ťňŐß spack_head::protocol_head 
    */
  template <typename PH>
  class pack_head_process
  {
    char* m_arr;
    tools::serializecpp_buffer m_sbuf;
    PH m_ph;
  public:
    pack_head_process(uint32_t aibytes):
      m_arr(new char[PH::END_POS+ aibytes]),
      m_sbuf(m_arr+ PH::END_POS,aibytes)
    {
    }

    void reset()
    {
      m_sbuf.reset();
    }

    template <typename T>
    void push(T& apdata)
    {
      tools::serializecpp::push(&m_sbuf, apdata);
    }

    template <typename T>
    static bool push(const T* aivalues, uint32_t ailen)
    {
      tools::serializecpp::push(&m_sbuf, aivalues, ailen);
    }

    PH* get_head()
    {
      m_ph.reset(m_arr, get_send_len());
      return &m_ph;
    }

    char* get_send_buffer()
    {
      return m_arr;
    }

    uint32_t get_send_len()
    {
      return PH::END_POS + m_sbuf.get_uselen();
    }

    /** żě˝ÝÉčÖĂÍˇ */
    void set_pack_head(uint32_t aierrornum = 0,uint32_t aiuserid = 0,uint32_t aiprotocolnum = 0)
    {
      m_ph.reset(m_arr, get_send_len());
      m_ph.set_data_byte(m_sbuf.get_uselen());

      m_ph.get_userid() = aiuserid;
      m_ph.get_error() = aierrornum;
      m_ph.get_protocol_num() = aiprotocolnum;
      ++m_ph.get_order();
      
      m_ph.set_crc();
    }


  };


  template <typename PH>
  class unpack_head_process
  {
    tools::serializecpp_buffer m_sbuf;
    PH m_ph;
  public:
    unpack_head_process(){}

    void reset( const char* ap, uint32_t aplen)
    {
      m_sbuf.reset( (char*)(ap+ PH::END_POS), aplen- PH::END_POS);
      m_ph.reset(ap, aplen );
    }

    PH* get_head()
    {
      return &m_ph;
    }

    template <typename T>
    void pop(T& aivalues)
    {
      tools::unserializecpp::pop(&m_sbuf,aivalues);
    }

    template <typename T>
    static bool pop(const T* aivalues, uint32_t ailen)
    {
      tools::unserializecpp::pop(&m_sbuf, aivalues, ailen);
    }


  };




}//namespace middleware
#endif //PACK_HEAD_HPP

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
