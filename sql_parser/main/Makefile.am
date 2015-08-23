AM_CPPFLAGS= -fPIC \
-I/usr/local/include

AM_LDFLAGS= -lc -lm -lrt -lxs  -lglog  -lpthread 

#include_HEADERS = m_test.h
				
bin_PROGRAMS = parser 

parser_SOURCES = main.cpp			

parser_LDADD =       \
				../parser/libparser.a	 \
				../ast_node/libast_node.a	\
				../../common/log/lib_logging.a		\
				/usr/local/lib/libglog.a	\
				/usr/local/lib/libglog.so	