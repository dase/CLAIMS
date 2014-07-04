AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux

AM_LDFLAGS=-lc -lm -lrt -lconfig++ -lpthread -lboost_serialization

LDADD = 	../BlockStreamIterator/libblockstreamiterator.a \
		../common/Block/libblock.a \
		../common/libcommon.a \
		../utility/libutility.a \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so

noinst_LIBRARIES=libexecutor.a
libexecutor_a_SOURCES = \
	AdaptiveEndPoint.cpp 		AdaptiveEndPoint.h \
	Coordinator.cpp 			Coordinator.cppnew \
	Coordinator.h 			ExchangeTracker.cpp \
	ExchangeTracker.h 		ExpanderTracker.cpp \
	ExpanderTracker.h 		IteratorExecutorMaster.cpp \
	IteratorExecutorMaster.h 	IteratorExecutorSlave.cpp \
	IteratorExecutorSlave.h 	PortManager.cpp \
	PortManager.h
	
libexecutor_a_LIBADD = 	/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a \
				/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
				/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so	

SUBDIRS = Test
DIST_SUBDIRS = Test