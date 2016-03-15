AM_CPPFLAGS=-fPIC  -DTHERON_XS\
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${GTEST_HOME}/include

#-L/usr/local/lib \
#-I/usr/local/include
AM_LDFLAGS=  -lc -lm -lrt -lpthread -lboost_serialization -lboost_date_time -lboost_system \
	-lconfig++ -lxs -lnuma -lreadline -lhistory -lz -ltinfo -Wl,--no-as-needed -ldl -rdynamic -lglog

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD =  \
    ../../sql_parser/parser/libparser.a \
    ../../sql_parser/ast_node/libast_node.a \
    ../../stmt_handler/libstmthandler.a \
	${BOOST_HOME}/stage/lib/libboost_serialization.a \
	${BOOST_HOME}/stage/lib/libboost_serialization.so \
	${BOOST_HOME}/stage/lib/libboost_system.a \
	${BOOST_HOME}/stage/lib/libboost_system.so \
	${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.so \
	${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a \
	${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
	${THERON_HOME}/Lib/libtherond.a \
	${GTEST_HOME}/libgtest.a 


noinst_LIBRARIES=libparser_test.a
libparser_test_a_SOURCES = \
	test_new_sql.cpp

