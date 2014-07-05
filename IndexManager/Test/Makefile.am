AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux

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
		/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a\
		/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES =	\
		test_CSB_index_building.cpp 		test_IndexManager_serialize.cpp \
		test_csb_classes.cpp 			test_csb_indexing.cpp \
		test_index_manager.cpp 			test_index_scan_iterator.cpp

