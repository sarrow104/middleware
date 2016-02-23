#include "readconfig.h"


std::ifstream tabkey_config::m_readfile;

boost::mutex tabkey_config::m_lock;

char table::m_segmentation = CSV_SEGMENTATION_CHAR;