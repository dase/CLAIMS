AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lpthread

LDADD = 	../libblockstreamiterator.a \
		../../Executor/libexecutor.a \
		../../storage/libstorage.a \
		../../common/libcommon.a \
		../../common/Schema/libschema.a \
		../../common/Block/libblock.a \
		../../utility/libutility.a \
		/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a\
		/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a

noinst_LIBRARIES=libparallelblockstreamiterator.a
libparallelblockstreamiterator_a_SOURCES = \
	BlockStreamAggregationIterator.cpp 		BlockStreamAggregationIterator.h \
	BlockStreamCombinedIterator.cpp 		BlockStreamCombinedIterator.h \
	BlockStreamExpander.cpp 		BlockStreamExpander.h \
	BlockStreamInIterator.cpp 		BlockStreamInIterator.h \
	BlockStreamJoinIterator.cpp 		BlockStreamJoinIterator.h \
	BlockStreamProjectIterator.cpp 		BlockStreamProjectIterator.h \
	BlockStreamSortIterator.cpp 		BlockStreamSortIterator.h \
	ExpandableBlockStreamBuffer.cpp 		ExpandableBlockStreamBuffer.h \
	ExpandableBlockStreamExchangeEpoll.cpp 		ExpandableBlockStreamExchangeEpoll.h \
	ExpandableBlockStreamExchangeLowerEfficient.cpp 		ExpandableBlockStreamExchangeLowerEfficient.h \
	ExpandableBlockStreamExchangeLowerMaterialized.cpp 		ExpandableBlockStreamExchangeLowerMaterialized.h \
	ExpandableBlockStreamExchangeMaterialized.cpp 		ExpandableBlockStreamExchangeMaterialized.h \
	ExpandableBlockStreamFilter.cpp 		ExpandableBlockStreamFilter.h \
	ExpandableBlockStreamHdfsScan.cpp 		ExpandableBlockStreamHdfsScan.h \
	ExpandableBlockStreamProjectionScan.cpp 		ExpandableBlockStreamProjectionScan.h \
	ExpandableBlockStreamRandomDiskAccess.cpp 		ExpandableBlockStreamRandomDiskAccess.h \
	ExpandableBlockStreamRandomMemAccess.cpp 		ExpandableBlockStreamRandomMemAccess.h \
	ExpandableBlockStreamSingleColumnScan.cpp 		ExpandableBlockStreamSingleColumnScan.h \
	ExpandableBlockStreamSingleColumnScanDisk.cpp 		ExpandableBlockStreamSingleColumnScanDisk.h

SUBDIRS = Test
DIST_SUBDIRS = Test