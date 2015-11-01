
AM_CPPFLAGS= -fPIC -fpermissive\
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

LDADD =	../BlockStreamIterator/libblockstreamiterator.a \
		../Catalog/libcatalog.a \
		../logical_operator/liblogicalqueryplan.a \
		../storage/libstorage.a \
		../common/libcommon.a \
		../common/Schema/libschema.a \
		../common/Block/libblock.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a 

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
