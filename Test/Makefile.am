AM_CPPFLAGS= -fPIC -fpermissive\
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${GTEST_HOME}/include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD =	TestSuit/libtestsuit.a \
		../Executor/libexecutor.a \
		../Executor/Test/libtest.a \
		../Daemon/Test/libtest.a \
		../Client/Test/libtest.a \
		../loader/test/libtest.a \
		../IndexManager/Test/libtest.a \
		../catalog/Test/libtest.a \
		../common/libcommon.a \
		../common/file_handle/libfilehandle.a \
		../common/file_handle/test/libtest.a \
		../common/Schema/libschema.a \
		../common/Block/libblock.a \
		../common/types/libtypes.a \
		../sql_parser/parser/libparser_test.a	\
		../utility/libutility.a \
		${HADOOP_HOME}/lib/native/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = 
#	Environment_test.cpp 		MultiToMulti.cpp \
#	TestMain.cpp 			block_stream_iterator_serilaization_test.cpp \
#	block_stream_test.cpp 		gtest.h \
#	memory_leak_test.h 		set_up_environment.h \
#	test_adaptiveendpoint.cpp 	test_coordinator.cpp \
#	test_scdong.cpp

SUBDIRS =  common utility
DIST_SUBDIRS =  common utility
