AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

LDADD = 	../../Test/libtest.a \
		../liblogicalqueryplan.a \
		../../Catalog/libcatalog.a \
		../../Catalog/stat/libstat.a \
		../../Parsetree/libparsetree.a \
		../../LogicalQueryPlan/liblogicalqueryplan.a \
		../../BlockStreamIterator/libblockstreamiterator.a \
		../../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../../common/libcommon.a \
		../../common/Block/libblock.a \
		../../utility/libutility.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a 

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = \
	Aggregation_test.cpp 			ResultCollect_test.cpp \
	getOptimalQueryPlan.cpp 		query_optmization_based_on_statisitic_test.cpp \
	testGenerateIteratorTree.cpp 		testGetDataflow.cpp \
	testIn.cpp 				testProject.cpp \
	testProject_wl.cpp 		testSort.cpp
