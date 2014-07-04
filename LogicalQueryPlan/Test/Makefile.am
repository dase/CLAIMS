AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization

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
		/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a\
		/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a 

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = \
	Aggregation_test.cpp 			ResultCollect_test.cpp \
	getOptimalQueryPlan.cpp 		query_optmization_based_on_statisitic_test.cpp \
	testGenerateIteratorTree.cpp 		testGetDataflow.cpp \
	testIn.cpp 				testProject.cpp \
	testProject_wl.cpp 		testSort.cpp
