
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

LDADD = ../Executor/libexecutor.a \
		../common/libcommon.a \
		../common/Schema/libschema.a \
		../common/Block/libblock.a	\
		../utility/libutility.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a \
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a
		
noinst_LIBRARIES=libblockstreamiterator.a
libblockstreamiterator_a_SOURCES = \
	BlockStreamExchangeBase.cpp 			BlockStreamExchangeBase.h \
	BlockStreamExchangeLowerBase.cpp 		BlockStreamExchangeLowerBase.h \
	BlockStreamFilter.cpp 				BlockStreamFilter.h \
	BlockStreamIteratorBase.cpp 			BlockStreamIteratorBase.h \
	BlockStreamLimit.cpp 				BlockStreamLimit.h \
	BlockStreamPerformanceMonitorTop.cpp 	BlockStreamPerformanceMonitorTop.h \
	BlockStreamPerformanceTest.cpp 		BlockStreamPerformanceTest.h \
	BlockStreamPrint.cpp 				BlockStreamPrint.h \
	BlockStreamRandomMemAccess.cpp 		BlockStreamRandomMemAccess.h \
	BlockStreamResultCollector.cpp 		BlockStreamResultCollector.h \
	BlockStreamSingleColumnScan.cpp 		BlockStreamSingleColumnScan.h \
	ExpandableBlockStreamIteratorBase.cpp 	ExpandableBlockStreamIteratorBase.h
			
SUBDIRS = ParallelBlockStreamIterator Serialization
DIST_SUBDIRS = ParallelBlockStreamIterator Serialization			
