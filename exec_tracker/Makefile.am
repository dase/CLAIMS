AM_CPPFLAGS= -fPIC -fpermissive -DTHERON_XS\
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lconfig++ -lpthread -lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../BlockStreamIterator/libblockstreamiterator.a \
		../common/Block/libblock.a \
		../common/libcommon.a \
		../utility/libutility.a \
		${CAF_HOME}/build/lib/libcaf_core.so \
		${CAF_HOME}/build/lib/libcaf_io.so \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so

noinst_LIBRARIES=libexec_tracker.a

libexec_tracker_a_SOURCES = \
	segment_exec_tracker.h 	segment_exec_tracker.cpp	\
	stmt_exec_tracker.h		stmt_exec_tracker.cpp
	
libexec_tracker_a_LIBADD = 	\
	${CAF_HOME}/build/lib/libcaf_core.so \
	${CAF_HOME}/build/lib/libcaf_io.so \
	${BOOST_HOME}/stage/lib/libboost_serialization.a \
	${BOOST_HOME}/stage/lib/libboost_serialization.so
