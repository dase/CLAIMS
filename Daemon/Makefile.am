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
LDADD = 	../Parsetree/libparsetree.a \
		../LogicalQueryPlan/liblogicalqueryplan.a \
		../common/Block/libblock.a \
		../utility/libutility.a \
		/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a\
		/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a

noinst_LIBRARIES=libdaemon.a
libdaemon_a_SOURCES =	Daemon.cpp	Daemon.h \
		Executing.cpp	Executing.h

SUBDIRS = Test
DIST_SUBDIRS = Test
