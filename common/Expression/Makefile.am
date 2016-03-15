AM_CPPFLAGS= -fPIC -fpermissive -DTHERON_XS\
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HDFS_HOME}/usr/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lconfig++ -lpthread -lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../../catalog/libcatalog.a \
		../../common/libcommon.a \
		../../common/Schema/libschema.a \
		../../utility/libutility.a \
		${THERON_HOME}/Lib/libtherond.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${BOOST_HOME}/stage/lib/libboost_system.so \
		${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libexpression.a

libexpression_a_SOURCES = 	\
	execfunc.cpp 		execfunc.h \
	expression_test.h 	initquery.cpp \
	initquery.h 		qnode.cpp \
	qnode.h 			queryfunc.cpp \
	queryfunc.h			sql_node_struct.h
	