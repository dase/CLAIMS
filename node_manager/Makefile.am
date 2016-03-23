AM_CPPFLAGS= -fPIC -fpermissive \
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization	\
-I${CAF_HOME}/libcaf_io	\
-I${CAF_HOME}/libcaf_core

AM_LDFLAGS=-lc -lm -lrt -lcaf_core -lcaf_io -lboost_system -lboost_serialization -ll -ly -lstdc++ -lxs 

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD =	../Executor/libexecutor.a	\
		../common/libcommon.a \
		../utility/libutility.a \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${BOOST_HOME}/stage/lib/libboost_system.so \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a	\
		${CAF_HOME}/build/lib/libcaf_core.so \
		${CAF_HOME}/build/lib/libcaf_io.so 

noinst_LIBRARIES=libnodemanager.a

libnodemanager_a_SOURCES = \
	base_node.cpp		base_node.h	\
	master_node.cpp		master_node.h	\
	slave_node.cpp		slave_node.h	\
	node_main.cpp
	

	
