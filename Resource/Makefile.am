
AM_CPPFLAGS= -fPIC -fpermissive \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization

AM_LDFLAGS=-lc -lm -lrt -lxs

LDADD = ../common/libcommon.a

noinst_LIBRARIES=libresouce.a
libresouce_a_SOURCES = \
	NodeTracker.cpp 		NodeTracker.h \
	ResourceInfo.cpp 		ResourceInfo.h \
	ResourceManagerMaster.cpp 		ResourceManagerMaster.h \
	ResourceManagerSlave.cpp 		ResourceManagerSlave.h

libresouce_a_LIBADD = ${THERON_HOME}/Lib/libtherond.a
