
AM_CPPFLAGS= -fPIC -fpermissive \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization

AM_LDFLAGS=-lc -lm -lrt -lxs -lboost_serialization

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = ../common/libcommon.a \
		../storage/libstorage.a \
		../utility/libutility.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a

noinst_LIBRARIES=libresouce.a
libresouce_a_SOURCES = \
	NodeTracker.cpp 		NodeTracker.h \
	ResourceInfo.cpp 		ResourceInfo.h \
	ResourceManagerMaster.cpp 		ResourceManagerMaster.h \
	ResourceManagerSlave.cpp 		ResourceManagerSlave.h \
	BufferManager.cpp		BufferManager.h \
	CPUResource.cpp			CPUResource.h

libresouce_a_LIBADD = ${THERON_HOME}/Lib/libtherond.a
