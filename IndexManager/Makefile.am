AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization

LDADD =	../BlockStreamIterator/libblockstreamiterator.a \
		../Catalog/libcatalog.a \
		../LogicalQueryPlan/liblogicalqueryplan.a \
		../Scheduler/libscheduler.a \
		../storage/libstorage.a \
		../common/libcommon.a \
		../common/Schema/libschema.a \
		../common/Block/libblock.a \
		/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a\
		/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a 

noinst_LIBRARIES=libindexmanager.a
libindexmanager_a_SOURCES = \
	CSBIndexBuilding.cpp 		CSBIndexBuilding.h \
	CSBPlusTree.cpp 			CSBPlusTree.h \
	IndexManager.cpp 			IndexManager.h \
	IndexScanIterator.cpp 		IndexScanIterator.h \
	LogicalCSBIndexBuilding.cpp 	LogicalCSBIndexBuilding.h \
	LogicalIndexFilter.cpp 		LogicalIndexFilter.h \
	LogicalIndexScan.cpp 		LogicalIndexScan.h

SUBDIRS = Test
DIST_SUBDIRS = Test