AM_CPPFLAGS= -fPIC -fpermissive\
-I${HADOOP_HDFS_HOME}/usr/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${GTEST_HOME}/include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs -lpthread  -lglog

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD =	../../sql_parser/ast_node/libast_node.a	 \
		../../common/log/liblog.a	\
		/usr/local/lib/libglog.a	\
		/usr/local/lib/libglog.so	\
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a  \
		${GTEST_HOME}/libgtest.a


noinst_LIBRARIES=libparser.a

libparser_a_SOURCES = \
	lex.yy.cpp 						sql.lex.h	\
	sql.tab.cpp 					sql.tab.hpp \
	parser.cpp						parser.h 
