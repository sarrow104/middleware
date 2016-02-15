///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef PACK_HEAD_HPP
#define PACK_HEAD_HPP

#include "middleware/tools/incidental/check_crc.hpp"

#include <cstdint>
#include <vector>

#define GET_HEAD( NAME , TYPE , POS  )        \
	TYPE& get_##NAME()                        \
	{                                         \
	return *( (TYPE*)&( m_data[ POS ] ) );    \
}


namespace middleware{

#define HEAD_SERVER 0
#define HEAD_CLIENT 1

#ifndef HEAD_TYPE
#define HEAD_TYPE   HEAD_SERVER
#endif //HEAD_TYPE

#if (HEAD_TYPE == HEAD_SERVER)
	/* 服务端 */
	typedef uint16_t    HEAD_SERVER_PROTOCOL_LEN_TYPE;          /* 协议长度 */
	typedef uint32_t    HEAD_IP_ADDRESS_TYPE;                   /* ip */
	typedef uint32_t    HEAD_CLOSESOCKET_COUNT_TYPE;            /* socket close次数 */
	typedef uint8_t     HEAD_SESSIONTYPE_TYPE;                  /* session 类型 */
	typedef uint8_t     HEAD_GROUPID_TYPE;                      /* 组id */
	typedef uint32_t    HEAD_SESSIONID_TYPE;                    /* session id */
	typedef uint16_t    HEAD_MASS_SIZE_TYPE;                    /* 群发数目类型 */
	typedef uint32_t    HEAD_SERVER_ERROR_TYPE;                 /* 错误 */
	typedef uint8_t     HEAD_SETTIMER_TYPE;                     /* set timer */

	/* 设置定时器  逻辑控制连接 */
	enum SET_TIMER__LOGIC_CONTROL
	{
		SET_NO_VALUES,
		SET_TIMER_OPEN,     /* 打开定时器 */
		SET_TIMER_CLOSE,    /* 关闭定时器 */
		SET_CONNECT_CLOSE,    /* 关闭连接 */
	};

#pragma pack(push)  //保存对齐状态
#pragma pack(1)     //设定为1字节对齐
	struct session_infor
	{
		HEAD_SESSIONID_TYPE m_session_id;
		HEAD_CLOSESOCKET_COUNT_TYPE m_socket_close_count;
		HEAD_GROUPID_TYPE m_group_id;
	};
#pragma pack(pop)   //恢复对齐状态


	/** 处理群发 */
	template < typename TYPE_ITEM >
	class mass_send
	{
		TYPE_ITEM* m_data;  /* 群发地址 */
	public:
		mass_send():m_data(NULL){}

		void reset(void* aidata)
		{
			m_data = (TYPE_ITEM*)aidata;
		}

		/* 设置群发队列 */
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

		/* 获取群发session id */
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

		/* 设置群发队列 */
		bool SH_set_mass_send_arr(std::vector< session_infor >& aisessionarr)
		{
			get_mass_size() = aisessionarr.size();
			m_mass.reset(const_cast<char*>(&m_data[get_server_protocol_len()]));
			return m_mass.set_mass_send_arr(aisessionarr);
		}

		/* 设置群发队列 */
		bool SH_set_mass_send_arr()
		{
			get_mass_size() = 0;
			return true;
		}

		/* 获取群发session id */
		bool SH_get_mass_send_arr(std::vector< session_infor* >& aisessionarr, uint32_t aibytesize)
		{
			if (get_mass_size() > 0 && get_mass_size() * sizeof(session_infor) + get_server_protocol_len() <= aibytesize)
			{
				m_mass.reset( const_cast<char*>(&m_data[get_server_protocol_len()]));
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
		/* 获取协议总长度 */
		HEAD_SERVER_PROTOCOL_LEN_TYPE get_server_protocol_len()
		{
			return *((HEAD_SERVER_PROTOCOL_LEN_TYPE*)&(m_data[HEAD_SERVER_PROTOCOL_LEN_POS]));
		}
		void set_server_protocol_len(uint32_t aisize)
		{
			*((HEAD_SERVER_PROTOCOL_LEN_TYPE*)&(m_data[HEAD_SERVER_PROTOCOL_LEN_POS])) = POS::END_POS + aisize;
		}

		/* 获取socket 关闭次数 */
		GET_HEAD(closesocket_count, HEAD_CLOSESOCKET_COUNT_TYPE, HEAD_CLOSESOCKET_COUNT_POS)

			/* 获取组id */
			GET_HEAD(groupid, HEAD_GROUPID_TYPE, HEAD_GROUPID_POS)

			/* 获取sessionid */
			GET_HEAD(sessionid, HEAD_SESSIONID_TYPE, HEAD_SESSIONID_POS)

			/* 获取ip */
			GET_HEAD(ip, HEAD_IP_ADDRESS_TYPE, HEAD_IP_ADDRESS_POS)

			/* 给逻辑机会 不设置某些连接的超时 */
			GET_HEAD(settimer, HEAD_SETTIMER_TYPE, HEAD_SETTIMER_POS)

			/* 获取群发数量*/
			GET_HEAD(mass_size, HEAD_MASS_SIZE_TYPE, HEAD_MASS_SIZE_POS)

			/* 获取error*/
			GET_HEAD(server_error, HEAD_SERVER_ERROR_TYPE, POS::HEAD_SERVER_ERROR_POS)
	};

#define PROTOCOL_HEAD_BEG_POS  (server_head::POS::END_POS)

#else  //HEAD_TYPE == HEAD_SERVER

#define PROTOCOL_HEAD_BEG_POS  (0)

#endif //HEAD_TYPE == HEAD_SERVER
	/* 公共头 */
	typedef uint32_t    HEAD_SERVER_MAGIC_TYPE;             /* 魔数 */
	typedef uint32_t    HEAD_DATA_BYTE_TYPE;                /* 数据长度 包括自身所占字节 */
	typedef uint32_t    HEAD_CRC_TYPE;                      /* crc校验 */
	typedef uint32_t    HEAD_ERROR_TYPE;                    /* 错误 */
	typedef uint64_t    HEAD_USER_ID_TYPE;                  /* 用户id */
	typedef uint32_t    HEAD_PROTOCOL_NUM_TYPE;             /* 协议号 */
	typedef uint32_t    HEAD_ORDER_TYPE;                    /* 序号 */
	typedef char*       PROTOCOL_BODY_TYPE;                 /* 协议体 */


	class protocol_head :
		public server_head
	{
		check_crc m_crc;
	public:
		enum POS
		{
			HEAD_DATA_BYTE_POS      = PROTOCOL_HEAD_BEG_POS,
			HEAD_CRC_POS            = HEAD_DATA_BYTE_POS + sizeof(HEAD_DATA_BYTE_TYPE),
			HEAD_ERROR_POS          = HEAD_CRC_POS + sizeof(HEAD_CRC_TYPE),
			HEAD_USER_ID_POS        = HEAD_ERROR_POS + sizeof(HEAD_ERROR_TYPE),
			HEAD_ANOTHER_ID_POS     = HEAD_USER_ID_POS + sizeof(HEAD_USER_ID_TYPE),
			HEAD_PROTOCOL_NUM_POS   = HEAD_ANOTHER_ID_POS + sizeof(HEAD_USER_ID_TYPE),
			HEAD_ORDER_POS          = HEAD_PROTOCOL_NUM_POS + sizeof(HEAD_PROTOCOL_NUM_TYPE),
			END_POS                 = HEAD_ORDER_POS + sizeof(HEAD_ORDER_TYPE),
		};

		void reset(const char* aidata, uint32_t aisize)
		{
#if (HEAD_TYPE == HEAD_SERVER)
			SH_reset(aidata, aisize);
#endif //(HEAD_TYPE == HEAD_SERVER)
		}

		HEAD_DATA_BYTE_TYPE get_data_byte()
		{
			return *((HEAD_DATA_BYTE_TYPE*)&(m_data[POS::HEAD_DATA_BYTE_POS])); 
		}

		void set_data_byte(uint32_t aibadyvalues)
		{
#if (HEAD_TYPE == HEAD_SERVER)
			*((HEAD_DATA_BYTE_TYPE*)&(m_data[POS::HEAD_DATA_BYTE_POS])) = aibadyvalues + (END_POS - PROTOCOL_HEAD_BEG_POS);
			set_server_protocol_len(*((HEAD_DATA_BYTE_TYPE*)&(m_data[POS::HEAD_DATA_BYTE_POS])) + get_mass_size() * sizeof(session_infor));
#else //(HEAD_TYPE == HEAD_SERVER)
			*((HEAD_DATA_BYTE_TYPE*)&(m_data[POS::HEAD_DATA_BYTE_POS])) = aibadyvalues + (END_POS - PROTOCOL_HEAD_BEG_POS);
#endif //(HEAD_TYPE == HEAD_SERVER)
		}

		/* 获取crc */
		GET_HEAD(crc, HEAD_CRC_TYPE, POS::HEAD_CRC_POS)
			/* 获取error */
			GET_HEAD(error, HEAD_ERROR_TYPE, POS::HEAD_ERROR_POS)
			/* 获取玩家id */
			GET_HEAD(userid, HEAD_USER_ID_TYPE, POS::HEAD_USER_ID_POS)
			/* 获取其他玩家id */
			GET_HEAD(anotherid, HEAD_USER_ID_TYPE, POS::HEAD_ANOTHER_ID_POS)
			/* 获取协议号 */
			GET_HEAD(protocol_num, HEAD_PROTOCOL_NUM_TYPE, POS::HEAD_PROTOCOL_NUM_POS)
			/* 获取顺序号 */
			GET_HEAD(order, HEAD_ORDER_TYPE, POS::HEAD_ORDER_POS)

			/* 设置crc */
			void set_crc()
		{
			get_crc() =
				m_crc(
				&(m_data[POS::HEAD_USER_ID_POS]),
				get_data_byte() - (POS::HEAD_USER_ID_POS - PROTOCOL_HEAD_BEG_POS)
				);
		}

		/* 检查crc */
		bool check_crc()
		{
			return m_crc(
				get_crc(),
				&(m_data[POS::HEAD_USER_ID_POS]),
				get_data_byte() - (POS::HEAD_USER_ID_POS - PROTOCOL_HEAD_BEG_POS)
				);
		}

		/* 获取数据*/
		const char* get_buffer()
		{
			return m_data;
		}

		/* 获取数据长度 */
		uint32_t get_buffer_len()
		{
#if (HEAD_TYPE == HEAD_SERVER)
			return get_server_protocol_len() + get_mass_size()*sizeof(session_infor);
#else //(HEAD_TYPE == HEAD_SERVER)
			return get_data_byte();
#endif //(HEAD_TYPE == HEAD_SERVER)

		}

		/*获取数据(不包括服务器那部分) */
		char* get_protocol_buffer()
		{
			return (char*)&(m_data[POS::HEAD_DATA_BYTE_POS]);
		}

		/* 获取数据长度(不包括服务器那部分)  */
		uint32_t get_protocol_buffer_len()
		{
			return get_data_byte();
		}

		/*协议体起始位置*/
		char* get_protocol_body()
		{
			return (char*)(&(m_data[POS::END_POS]));
		}

		/* 获取数据长度(不包括服务器那部分)  */
		uint32_t get_protocol_body_len()
		{
			return get_data_byte() - (POS::END_POS - PROTOCOL_HEAD_BEG_POS);
		}

		uint32_t get_protocol_head_len()
		{
			return POS::END_POS;
		}

#if (HEAD_TYPE == HEAD_SERVER)
		/* 设置群发队列 */
		bool set_mass_send_arr(std::vector< session_infor >& aisessionarr)
		{
			return SH_set_mass_send_arr(aisessionarr);
		}
		/* 设置群发队列 */
		bool set_mass_send_arr()
		{
			return SH_set_mass_send_arr();
		}
		/* 获取群发session id */
		bool get_mass_send_arr(std::vector< session_infor* >& aisessionarr, uint32_t aibytesize)
		{
			return SH_get_mass_send_arr(aisessionarr, aibytesize);
		}
#endif //(HEAD_TYPE == HEAD_SERVER)

	};

}//namespace middleware
#endif //PACK_HEAD_HPP

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */