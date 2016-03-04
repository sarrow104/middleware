#ifndef MACRO_SIMPLIFY_H
#define MACRO_SIMPLIFY_H

#define POP_PTR(MGT,DATA,KEY)				\
	middleware::tools::pop_ptr_fun(MGT,DATA,KEY)

#define POP_ARG_ARRAY(ARG,NAME)			\
		lp.pop(ARG,sizeof(ARG), (string(apkey)+NAME).c_str())

#define POP_ARG_PTR(ARG,NAME)				\
		POP_PTR( lp, arg4, (string(apkey) + NAME).c_str() )

#define POP_ARG(ARG,NAME)						\
		lp.pop(ARG, (string(apkey) + NAME).c_str())

#define PUSH_ARG_ARRAY(ARG,NAME)		\
		lp.push(ARG, sizeof(ARG), (string(apkey) + NAME).c_str())
#define PUSH_ARG(ARG,NAME)					\
		lp.push(ARG, (string(apkey) + NAME).c_str())
#define PUSH_ARG_PTR(ARG,NAME)			\
		lp.push(ARG, (string(apkey) + NAME).c_str())

#define POP_FUN_ARG(...)						\
	bool pop(middleware::tools::mgt_serializecpp& lp, const char* apkey = "")		\
	{																																						\
		__VA_ARGS__;																															\
		return true;																															\
	}

#define PUSH_FUN_ARG(...)						\
	bool push(middleware::tools::mgt_serializecpp& lp, const char* apkey = "")	\
	{																																						\
		__VA_ARGS__;																															\
		return true;																															\
	}

#define ENDIAN_ARG_ARRAY(ARG)				\
	middleware::tools::gendian_local2net.endian(ARG,sizeof(ARG))

#define ENDIAN_ARG(ARG)							\
	middleware::tools::gendian_local2net.endian(ARG)

#define ENDIAN_FUN(...)							\
		void endian()										\
		{																\
			__VA_ARGS__;									\
		}
	

#define GET_SERIALIZE_BUFFER( TYPE, FUN )             \
  switch(TYPE)                                        \
  {                                                   \
    case SERIALIZE_TYPE_BINARY:                       \
      return m_sbuf.FUN;                              \
    case SERIALIZE_TYPE_JSON:                         \
      return m_json_sbuf.FUN;                         \
    case SERIALIZE_TYPE_XML:                          \
      return m_xml_sbuf.FUN;                          \
  }

#define GET_PUSH_SERIALIZE( TYPE, FUN, ... )          \
  switch(TYPE)                                        \
  {                                                   \
  case SERIALIZE_TYPE_BINARY:                         \
    tools::serializecpp::FUN(m_sbuf,__VA_ARGS__);     \
    return;                                           \
  case SERIALIZE_TYPE_JSON:                           \
    tools::serializecpp_json::FUN(m_json_sbuf,__VA_ARGS__);\
    return;                                           \
  case SERIALIZE_TYPE_XML:                            \
    tools::serializecpp_xml::FUN(m_xml_sbuf,__VA_ARGS__);\
      return;                                         \
  }

#define GET_POP_SERIALIZE( TYPE, FUN , ...)           \
  switch(TYPE)                                        \
  {                                                   \
  case SERIALIZE_TYPE_BINARY:                         \
    tools::unserializecpp::FUN(m_sbuf,__VA_ARGS__);   \
    return;                                           \
  case SERIALIZE_TYPE_JSON:                           \
    tools::unserializecpp_json::FUN(m_json_sbuf,__VA_ARGS__);\
    return;                                           \
  case SERIALIZE_TYPE_XML:                            \
    tools::unserializecpp_xml::FUN(m_xml_sbuf,__VA_ARGS__);\
    return;                                           \
  }

#define GET_POP_NOTRET_SERIALIZE( TYPE, FUN , ...)           \
  switch(TYPE)                                        \
  {                                                   \
  case SERIALIZE_TYPE_BINARY:                         \
    tools::unserializecpp::FUN(m_sbuf,__VA_ARGS__);   \
		break;																						\
  case SERIALIZE_TYPE_JSON:                           \
    tools::unserializecpp_json::FUN(m_json_sbuf,__VA_ARGS__);\
		break;																						\
  case SERIALIZE_TYPE_XML:                            \
    tools::unserializecpp_xml::FUN(m_xml_sbuf,__VA_ARGS__);\
		break;																						\
	default:																						\
		throw 0;																					\
  }


#endif //MACRO_SIMPLIFY_H
