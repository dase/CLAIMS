AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lpthread -lxs

LDADD = 	../libblockstreamiterator.a \
		../../Executor/libexecutor.a \
		../../storage/libstorage.a \
		../../common/libcommon.a \
		../../common/Schema/libschema.a \
		../../common/Block/libblock.a \
		../../utility/libutility.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a

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