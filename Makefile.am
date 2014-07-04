#现在写的动态链接库是两个地方都需要写include的文件，这里要想办法将系统中的hadoop_home指定
AM_CPPFLAGS= -fPIC\
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/imdb/supports/boost_1_53_0/boost \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

AM_LDFLAGS= -lc -lm -lrt -lpthread -lboost_serialization -lboost_date_time -lboost_system -lconfig++ -lxs


LDADD =  \
	Test/common/libcommon.a \
	Test/TestSuit/libtestsuit.a \
	Test/utility/libutility.a \
	Test/libtest.a \
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
	Catalog/libcatalog.a \
	Catalog/Test/libtest.a \
	Catalog/stat/libstat.a \
	storage/libstorage.a \
	BlockStreamIterator/ParallelBlockStreamIterator/Test/libtest.a \
	BlockStreamIterator/Serialization/libserialization.a \
	BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
	BlockStreamIterator/libblockstreamiterator.a \
	Resource/libresouce.a \
	Loader/libloader.a \
	Loader/Test/libtest.a \
	common/libcommon.a \
	common/Block/libblock.a \
	common/Schema/libschema.a \
	common/Schema/Test/libtest.a \
	common/types/libtypes.a \
	common/types/Test/libtest.a \
	common/types/ttmath/libttmath.a \
	ThirdParty/boost/serialization/libserialization.a \
	ThirdParty/Test/libtest.a \
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

include_HEADERS = Config.h Debug.h \
	Environment.h IDsGenerator.h
				
bin_PROGRAMS =  server 

server_SOURCES = Config.cpp	Environment.cpp 	IDsGenerator.cpp

#生成可执行文件是用LDADD，但是生成.a文件是用LIBADD，
#后面.o是需要加入的，有没有优雅点的方法呢？但如果只是.h文件呢？
#如果有多层依赖的话，要在第一层的makefile中写上第三层.a的依赖，但是这个地方是可以优化的感觉。
#server_LDADD = Block/libblock.a \
#	BlockStreamIterator/libblockstreamiterator.a \
#	BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
#	BufferManager/libbuffermanager.a \
#	Catalog/libcatalog.a \
#	Catalog/stat/libstat.a \
#	Client/libclient.a \
#	common/libcommon.a \
#	Daemon/libdaemon.a \
#	Executor/libexecutor.a \
#	IndexManager/libindexmanager.a \
#	iterator/libiterator.a \
#	Loader/libloader.a \
#	LogicalQueryPlan/liblogicalqueryplan.a \
#	Parsetree/libparsetree.a \
#	PerformanceMonitor/libperformancemonitor.a \
#	Resource/libresouce.a \
#	Scheduler/libscheduler.a \
#	Schema/libschema.a \
#	Serialization/libserialization.a \
#	storage/libstorage.a \
#	Test/TestSuit/libtestsuite.a \
#	try/libtry.a \
#	types/libtypes.a \
#	utility/libutility.a \
#	/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
#	/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
#	/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a \
#	/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.so \
#	/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.so \
#	/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a \
#	/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
#	/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a
	

SUBDIRS= BlockStreamIterator BufferManager Catalog Client common Daemon Executor IndexManager\
Loader LogicalQueryPlan Parsetree Resource Scheduler \
storage Test ThirdParty try utility

DIST_SUBDIRS = BlockStreamIterator BufferManager Catalog Client common Daemon Executor IndexManager\
Loader LogicalQueryPlan Parsetree Resource Scheduler \
storage Test ThirdParty try utility
