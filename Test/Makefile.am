AM_CPPFLAGS= -fPIC -fpermissive\
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${GTEST_HOME}/include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

LDADD =	TestSuit/libtestsuit.a \
		../Executor/libexecutor.a \
		../Executor/Test/libtest.a \
		../Parsetree/Test/libtest.a \
		../LogicalQueryPlan/Test/libtest.a \
		../Daemon/Test/libtest.a \
		../Client/Test/libtest.a \
		../Loader/Test/libtest.a \
		../IndexManager/Test/libtest.a \
		../BlockStreamIterator/libblockstreamiterator.a \
		../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../BlockStreamIterator/ParallelBlockStreamIterator/Test/libtest.a \
		../Catalog/Test/libtest.a \
		../common/libcommon.a \
		../common/Schema/libschema.a \
		../common/Block/libblock.a \
		../common/types/libtypes.a \
		../utility/libutility.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a  \
		${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = 

#	Environment_test.cpp 		MultiToMulti.cpp \
#	TestMain.cpp 			block_stream_iterator_serilaization_test.cpp \
#	block_stream_test.cpp 		gtest.h \
#	memory_leak_test.h 		set_up_environment.h \
#	test_adaptiveendpoint.cpp 	test_coordinator.cpp \
#	test_scdong.cpp

SUBDIRS = TestSuit common utility
DIST_SUBDIRS = TestSuit common utility
