AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux

#此文件需要包含boost序列化库
AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lpthread

#本模块最好依赖的是这些库：executor schema block utility common storage
#其中iterator可以合并进来,还需要包含blockiterator的库
LDADD = ../libparallelblockStreamiterator.a \
		../../../Catalog/libcatalog.a \
		../../../LogicalQueryPlan/liblogicalqueryplan.a \
		../../libblockstreamiterator.a \
		../../../utility/libutility.a \
		../../../common/libcommon.a \
		../../../common/Schema/libschema.a \
		../../../common/Block/libblock.a \
		/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a\
		/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = \
	Buffer.cpp 			Expanded_iterators_test.cpp \
	Project_test.cpp 		Sort_test.cpp \
	TopN_test.cpp 		in_iterator_test.cpp \
	iterator_test.cpp 	projectionScan.cpp 
