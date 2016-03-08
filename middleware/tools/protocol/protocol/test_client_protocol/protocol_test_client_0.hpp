///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef PROTOCOL_TEST_CLIENT_COUT
#define PROTOCOL_TEST_CLIENT_COUT

#include "middleware/tools/protocol/protocol_base.hpp"
#include "middleware/tools/protocol/protocol/test_client_protocol/protocol_test_client_num.hpp"

#include <string>

namespace middleware {
	namespace tools {




#define CREATE_CPROTOCOL_BEG( NAME, PROTOCOL_NUM)			\
	class NAME:																					\
		public protocol_cbase															\
	{																										\
		struct pop_data;																	\
		struct push_data;																	\
		pop_data* m_popdata;															\
		push_data* m_pushdata;														\
	public:																							\
		NAME() :protocol_cbase(PROTOCOL_NUM)							\
		{																									\
			m_popdata = (pop_data*)m_premote2local_buffer;	\
			m_pushdata = (push_data*)m_plocal2remote_buffer;\
		}																									\
		virtual void init_data();													\
		virtual uint32_t task(uint32_t aikey);						\
		virtual void serialization();											\
		virtual void unserialization();										\
		virtual protocol_cbase* new_own(){return new NAME();}	\
	};



		CREATE_CPROTOCOL_BEG(protocol_test_client_0, E_TEST_CLIENT_0)
#if 0
    class protocol_test_client_0 :
      public protocol_cbase
    {
			STRUCT_DATA_RECV_VEG_BEG
				STRUCT_MEMBER(std::string, mpop_ls);
			STRUCT_DATA_RECV_VEG_END

      std::string mpop_ls;
    public:
      protocol_test_client_0() :
        protocol_cbase(E_TEST_CLIENT_0)
      {}

      virtual uint32_t task(uint32_t aikey)
      {
        return 0;
      }

      virtual void serialization()
      {
			}

      virtual void unserialization()
      {
        m_premote2local->pop(mpop_ls);
      }

      virtual protocol_cbase* new_own()
      {
        return new protocol_test_client_0();
      }

    };
#endif
  } //namespace tools
}//namespace middleware
#endif //PROTOCOL_TEST_CLIENT_COUT

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
