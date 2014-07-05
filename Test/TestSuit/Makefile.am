AM_CPPFLAGS= -fPIC \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/imdb/supports/gtest-1.7.0/include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization

LDADD = 	../libtest.a \
		../../Catalog/libcatalog.a \
		../../Loader/libloader.a \
		../../LogicalQueryPlan/liblogicalqueryplan.a \
		../../BlockStreamIterator/libblockstreamiterator.a \
		../../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../../storage/libstorage \
		../../common/libcommon.a \
		../../common/types/libtypes.a \
		../../common/Block/libblock.a \
		../../utility/libutility.a \
		/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a\
		/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a  \
		/home/imdb/supports/gtest-1.7.0/libgtest.a

noinst_LIBRARIES=libtestsuit.a
libtestsuit_a_SOURCES = \
	ExpanderFrameTest.h 				hash_table_test.h \
	in_segment_scalability_test.cpp 		tpc_h_test.cpp
