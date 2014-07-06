AM_CPPFLAGS= -fPIC \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lpthread

LDADD = 	\
		../libindexmanager.a \
		../../Catalog/libcatalog.a \
		../../LogicalQueryPlan/liblogicalqueryplan.a \
		../../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../../BlockStreamIterator/libblockstreamiterator.a \
		../../Resource/libresource.a \
		../../common/libcommon.a \
		../../utility/libutility.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES =	\
		test_CSB_index_building.cpp 		test_IndexManager_serialize.cpp \
		test_csb_classes.cpp 			test_csb_indexing.cpp \
		test_index_manager.cpp 			test_index_scan_iterator.cpp

