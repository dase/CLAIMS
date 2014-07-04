AM_CPPFLAGS= -fPIC\
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

AM_LDFLAGS=-lc -lm -lrt

LDADD = ../common/libcommon.a

noinst_LIBRARIES=libresouce.a
libresouce_a_SOURCES = \
	NodeTracker.cpp 		NodeTracker.h \
	ResourceInfo.cpp 		ResourceInfo.h \
	ResourceManagerMaster.cpp 		ResourceManagerMaster.h \
	ResourceManagerSlave.cpp 		ResourceManagerSlave.h

libresouce_a_LIBADD = /home/imdb/supports/Theron-5.01.01/Lib/libtherond.a