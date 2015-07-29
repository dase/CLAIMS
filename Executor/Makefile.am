AM_CPPFLAGS= -fPIC -fpermissive -DTHERON_XS\
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lconfig++ -lpthread -lboost_serialization -lxs

LDADD = 	../BlockStreamIterator/libblockstreamiterator.a \
		../common/Block/libblock.a \
		../common/libcommon.a \
		../utility/libutility.a \
		${THERON_HOME}/Lib/libtherond.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so

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
	
libexecutor_a_LIBADD = 	${THERON_HOME}/Lib/libtherond.a \
				${BOOST_HOME}/stage/lib/libboost_serialization.a \
				${BOOST_HOME}/stage/lib/libboost_serialization.so

SUBDIRS = Test
DIST_SUBDIRS = Test
