AM_CPPFLAGS= -fPIC \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lpthread

LDADD = ../libparallelblockStreamiterator.a \
		../../../Catalog/libcatalog.a \
		../../../LogicalQueryPlan/liblogicalqueryplan.a \
		../../libblockstreamiterator.a \
		../../../utility/libutility.a \
		../../../common/libcommon.a \
		../../../common/Schema/libschema.a \
		../../../common/Block/libblock.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = \
	Buffer.cpp 			Expanded_iterators_test.cpp \
	Project_test.cpp 		Sort_test.cpp \
	TopN_test.cpp 		in_iterator_test.cpp \
	iterator_test.cpp 	projectionScan.cpp 
