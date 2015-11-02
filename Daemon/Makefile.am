AM_CPPFLAGS= -fPIC -fpermissive -DTHERON_XS\
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lpthread -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../Parsetree/libparsetree.a \
		../logical_operator/liblogicalqueryplan.a \
		../common/Block/libblock.a \
		../utility/libutility.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a

noinst_LIBRARIES=libdaemon.a
libdaemon_a_SOURCES =	Daemon.cpp	Daemon.h \
		Executing.cpp	Executing.h
		
SUBDIRS = Test
DIST_SUBDIRS = Test