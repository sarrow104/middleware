//        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_ASIO_SESSION_KEEP_H
#define SOCKET_ASIO_SESSION_KEEP_H

#include "middleware/middleware_base/socket_asio/socket_asio_session_base.hpp"
#include "middleware/middleware_base/socket_asio/alonelist.hpp"

namespace middleware {

	class socket_asio_session_keep :
		public socket_asio_session_base
	{
		/* 广播队列 */
		struct broadcasting_list
		{
			typedef std::vector<session_infor*  >  type_session_infor_list;
			type_session_infor_list  m_sessioninfor_list;
			broadcasting_list() {}
		};

		static std::vector<broadcasting_list> m_broadcasting_list;
		static alonelist3<socket_asio_session_base>* mp_alone;
		static uint32_t m_persecond_recvdatatbyte_num;
		uint32_t m_datatbyte_num;			/* 获取的字节数 */
		static std::vector<tools::segmentation_pack<socket_asio_session_keep*>* > m_segmentation;
	public:
		virtual bool close_complementary()
		{
			mp_alone->release_instance(this);
			return true;
		}

		explicit socket_asio_session_keep(
			const socket_asio_arg& aiarg,
			size_t aigroupid,
			boost::asio::io_service& aiio_service,
			alonelist3<socket_asio_session_base>* aialone
			) :
			socket_asio_session_base(aiarg, aigroupid, aiio_service)
		{
			static bool lfirst = true;
			if (lfirst)
			{
				lfirst = false;

				m_broadcasting_list.resize(aiarg.get_thread_maxsize());
				m_segmentation.resize(aiarg.get_thread_maxsize());
				mp_alone = aialone;
			}
			m_persecond_recvdatatbyte_num = aiarg.m_persecond_recvdatatbyte_num;

			if (m_segmentation[aigroupid] == NULL)
			{
				m_segmentation[aigroupid] = new tools::segmentation_pack<socket_asio_session_keep*>(bind(&socket_asio_session_keep::handle_segmentation, this, _1, _2, _3));
			}
		}

		virtual ~socket_asio_session_keep()
		{
		}


		virtual bool reset_complementary()
		{
			m_datatbyte_num = 0;
			m_settimer = true;
			return true;
		}


#ifdef   OPEN_MAX_RECV_COUNT
		bool check_message_count(uint32_t ailen)
		{
			/* 是否超过1分钟 */
			time_t temp_time = time(NULL) - m_begin;
			m_datatbyte_num += ailen;
			if (temp_time == 0.0)
			{
				return true;
			}

			if (m_datatbyte_num / temp_time >= m_persecond_recvdatatbyte_num)
			{
				return false;
			}
			else
			{
				/*重置*/
				m_datatbyte_num = 0;
				m_begin = temp_time;
				return true;
			}



			return true;
		}
#endif  //OPEN_MAX_RECV_COUNT

		virtual bool handle_read_complementary(const boost::system::error_code& error, size_t bytes_transferred)
		{

			char* lp = mp_data1;
			size_t litempsize = bytes_transferred;
			if (*((uint16_t*)(lp)) == bytes_transferred - sizeof(uint16_t))
			{
				lp += sizeof(uint16_t);
				litempsize -= sizeof(uint16_t);
			}
			/* 分包处理  */
			if (!get_segmentation()->segmentation(this, m_ipadress, lp, litempsize))
			{
				socket_asio_session_base::push_close();
				//write_log(m_errlog, m_ipadress, THROW__RECV_DATA_SEGMENTATION, m_sessionid, m_groupid, "", 0);
				//single_callback_module_fun()->writeipsessionidnummsglog(  m_ipadress , THROW__RECV_DATA_SEGMENTATION , m_sessionid , m_groupid, "" , 0 );
				return false;
			}
			else
			{
				start();
				return true;
			}


		}



		/* 分包回调 */
		bool handle_segmentation(socket_asio_session_keep* aithis, char* ap, uint32_t alen)
		{

#ifdef   OPEN_MAX_RECV_COUNT
			if (!aithis->check_message_count(alen))
			{

				push_close();
				single_callback_module_fun()->writeipsessionidnummsglog(aithis->m_ipadress, THROW__CHECK_CRC, aithis->m_sessionid, aithis->m_groupid, "", 0);
				return false;
			}
#endif  //OPEN_MAX_RECV_COUNT

			ap -= server_head::END_POS;
			alen += server_head::END_POS;

			aithis->set_php(ap, alen);
			if (aithis->check_heartbeat_fun())
			{
				return true;
			}


			if (aithis->check_crc_fun())
			{

				get_middleware()->send(ap, alen);
				return true;
			}
			else
			{

				socket_asio_session_base::push_close();
				//write_log(m_errlog, aithis->m_ipadress, THROW__CHECK_CRC, aithis->m_sessionid, aithis->m_groupid, "", 0);
				//single_callback_module_fun()->writeipsessionidnummsglog(  aithis->m_ipadress , THROW__CHECK_CRC , aithis->m_sessionid , aithis->m_groupid , "" , 0 );
				return false;
			}
		}


		virtual bool middleware_callback_complementary(protocol_head& m_php2, uint32_t aibyte)
		{
			/*  */
			/* 设置m_settimer */
			if (m_settimer != m_php2.get_settimer() && m_settimer == SET_TIMER__LOGIC_CONTROL::SET_TIMER_OPEN)
			{
				m_settimer = m_php2.get_settimer();
				/* 取消 */
				m_timer.cancel();
			}


			/* 获取广播session队列 */
			session_mapping<socket_asio_session_base*>& lsmsasb = get_sessionidmapp( /*m_type*/);

			typedef broadcasting_list::type_session_infor_list  sessioninfor_list;
			sessioninfor_list& lsessioninfor_list = get_broadcasting_sessionid_list(m_groupid);


			lsessioninfor_list.clear();
			m_php2.get_mass_send_arr(lsessioninfor_list, aibyte);



			socket_asio_session_base* ltempptr;
			for (sessioninfor_list::iterator itor = lsessioninfor_list.begin(); itor != lsessioninfor_list.end(); ++itor)
			{
				ltempptr = lsmsasb.find((*itor)->m_session_id);
				if (ltempptr != NULL)
				{
					ltempptr->session_send((*itor)->m_socket_close_count, m_php2.get_protocol_buffer(), m_php2.get_protocol_buffer_len());
				}
			}

			if (m_settimer == SET_TIMER__LOGIC_CONTROL::SET_CONNECT_CLOSE)
			{
				/* 关闭连接 */
				socket_asio_session_base::push_close();
			}

			return true;
		}

		static broadcasting_list::type_session_infor_list& get_broadcasting_sessionid_list(uint32_t ainum)
		{
			return m_broadcasting_list[ainum].m_sessioninfor_list;
		}

		tools::segmentation_pack<socket_asio_session_keep*>*& get_segmentation()
		{
			return m_segmentation[m_groupid];
		}
	};

} //namespace middleware

#endif //SOCKET_ASIO_SESSION_KEEP_H

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */