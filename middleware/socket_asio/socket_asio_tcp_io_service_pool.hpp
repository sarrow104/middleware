///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef SOCKET_ASIO_TCP_IO_SERVICE_POOL
#define SOCKET_ASIO_TCP_IO_SERVICE_POOL

#include <boost/asio.hpp>
#include <boost/thread.hpp>


namespace middleware {

	/**
	 * socket asio service pool
	 */
	class socket_asio_tcp_io_service_pool
	{
	public:
		explicit socket_asio_tcp_io_service_pool( uint32_t airecvthreadsize ) : 
		next_io_service_( 0 ) ,
			m_recvthreadsize( airecvthreadsize )
		{ 
			for (std::size_t i = 0; i < m_recvthreadsize + 1 ; ++ i)
			{
				io_service_sptr lio_service(new boost::asio::io_service());
				work_sptr work(new boost::asio::io_service::work(*lio_service));
				io_services_.push_back( lio_service );
				work_.push_back(work);
			}
			start();
		}

		void start()
		{ 
			for (std::size_t i = 0; i < io_services_.size(); ++ i)
			{
				boost::shared_ptr<boost::thread>(new boost::thread(
					boost::bind(&boost::asio::io_service::run, io_services_[i])));
			}
		}


		std::size_t get_next_io_service()
		{
			return  next_io_service_++ % m_recvthreadsize;
		}

		boost::asio::io_service& get_io_service( )
		{
			return *( io_services_[ next_io_service_%m_recvthreadsize ] );
		}



		boost::asio::io_service& get_io()
		{
			return *( io_services_[ m_recvthreadsize ] );
		}


	private:
		typedef boost::shared_ptr<boost::asio::io_service> io_service_sptr;
		typedef boost::shared_ptr<boost::asio::io_service::work> work_sptr;
		typedef boost::shared_ptr<boost::thread> thread_sptr;
		std::vector<io_service_sptr> io_services_;
		std::vector<work_sptr> work_;
		std::vector<thread_sptr> threads_; 
		std::size_t next_io_service_;
		uint32_t m_recvthreadsize;
	};

} //namespace middleware 
#endif //SOCKET_ASIO_TCP_IO_SERVICE_POOL

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */