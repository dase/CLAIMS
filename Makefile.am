AM_CPPFLAGS=-fPIC\
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/boost_1_53_0/ \
-I/home/imdb/supports/gtest-1.7.0/include

AM_LDFLAGS= -lc -lm -lrt -lpthread -lboost_serialization -lboost_date_time -lboost_system -lconfig++ -lxs

LDADD =  \
	Test/libtest.a \
	Test/common/libcommon.a \
	Test/TestSuit/libtestsuit.a \
	Test/utility/libutility.a \
	BlockStreamIterator/Serialization/libserialization.a \
	Client/libclient.a \
	Client/Test/libtest.a \
	Daemon/libdaemon.a \
	Daemon/Test/libtest.a \
	Executor/libexecutor.a \
	Executor/Test/libtest.a \
	IndexManager/libindexmanager.a \
	IndexManager/Test/libtest.a \
	Parsetree/libparsetree.a \
	Parsetree/Test/libtest.a \
	try/libtry.a \
	Scheduler/libscheduler.a \
	Scheduler/test/libtest.a \
	BufferManager/libbuffermanager.a \
	LogicalQueryPlan/Test/libtest.a \
	LogicalQueryPlan/liblogicalqueryplan.a \
	storage/libstorage.a \
	BlockStreamIterator/ParallelBlockStreamIterator/Test/libtest.a \
	BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
	BlockStreamIterator/libblockstreamiterator.a \
	Resource/libresouce.a \
	Catalog/libcatalog.a \
	Catalog/Test/libtest.a \
	Catalog/stat/libstat.a \
	Loader/libloader.a \
	Loader/Test/libtest.a \
	common/libcommon.a \
	common/Block/libblock.a \
	common/Schema/libschema.a \
	common/Schema/Test/libtest.a \
	common/types/libtypes.a \
	common/types/Test/libtest.a \
	common/types/ttmath/libttmath.a \
	utility/libutility.a \
	/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
	/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
	/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a \
	/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.so \
	/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.so \
	/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a \
	/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
	/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a \
	/home/imdb/supports/gtest-1.7.0/libgtest.a

include_HEADERS = Config.h \
				Debug.h \
				Environment.h \
				IDsGenerator.h \
				configure.h
				
bin_PROGRAMS =  server 

server_SOURCES = Config.cpp	\
				Environment.cpp \
				IDsGenerator.cpp \
				Server.cpp

SUBDIRS= BlockStreamIterator BufferManager Catalog Client common Daemon Executor IndexManager\
Loader LogicalQueryPlan Parsetree Resource Scheduler \
storage Test try utility

DIST_SUBDIRS = BlockStreamIterator BufferManager Catalog Client common Daemon Executor IndexManager\
Loader LogicalQueryPlan Parsetree Resource Scheduler \
storage Test try utility
