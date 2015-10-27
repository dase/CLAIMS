AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = ../Executor/libexecutor.a \
		../common/libcommon.a \
		../../storage/libstorage.a \
		../common/Schema/libschema.a \
		../common/Block/libblock.a	\
		../utility/libutility.a \
		../codegen/codegen.a\
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a \
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a
		
noinst_LIBRARIES=libphysicalqueryplan.a
libphysicalqueryplan_a_SOURCES = \
	physical_operator.cpp 	physical_operator.h	\
	BlockStreamExchangeLowerBase.cpp 		BlockStreamExchangeLowerBase.h \
	BlockStreamIteratorBase.cpp 				BlockStreamIteratorBase.h \
	physical_limit.cpp 							physical_limit.h \
	BlockStreamPerformanceMonitorTop.cpp 	BlockStreamPerformanceMonitorTop.h \
	BlockStreamPrint.cpp 						BlockStreamPrint.h \
	BlockStreamResultCollector.cpp 			BlockStreamResultCollector.h \
	BlockStreamAggregationIterator.cpp 				BlockStreamAggregationIterator.h \
	BlockStreamCombinedIterator.cpp 				BlockStreamCombinedIterator.h \
	BlockStreamExpander.cpp 						BlockStreamExpander.h \
	BlockStreamInIterator.cpp 						BlockStreamInIterator.h \
	BlockStreamJoinIterator.cpp 						BlockStreamJoinIterator.h \
	physical_project.cpp 						physical_project.h \
	BlockStreamSortIterator.cpp 						BlockStreamSortIterator.h \
	ExpandableBlockStreamBuffer.cpp 					ExpandableBlockStreamBuffer.h \
	ExpandableBlockStreamExchangeEpoll.cpp 				ExpandableBlockStreamExchangeEpoll.h \
	ExpandableBlockStreamExchangeLowerEfficient.cpp 		ExpandableBlockStreamExchangeLowerEfficient.h \
	ExpandableBlockStreamExchangeLowerMaterialized.cpp 		ExpandableBlockStreamExchangeLowerMaterialized.h \
	physical_filter.cpp 								physical_filter.h \
	ExpandableBlockStreamHdfsScan.cpp 					ExpandableBlockStreamHdfsScan.h \
	physical_projection_scan.cpp 					physical_projection_scan.h \
	ExpandableBlockStreamRandomDiskAccess.cpp 			ExpandableBlockStreamRandomDiskAccess.h \
	ExpandableBlockStreamRandomMemAccess.cpp 			ExpandableBlockStreamRandomMemAccess.h \
	ExpandableBlockStreamSingleColumnScan.cpp 			ExpandableBlockStreamSingleColumnScan.h \
	ExpandableBlockStreamSingleColumnScanDisk.cpp 		ExpandableBlockStreamSingleColumnScanDisk.h \
	physical_nestloop_join.h					        physical_nestloop_join.cpp	
			
SUBDIRS = 
DIST_SUBDIRS = 
