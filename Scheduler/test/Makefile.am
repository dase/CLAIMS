AM_CPPFLAGS= -fPIC -fpermissive\
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

LDADD = 	${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = QueryDagTest.cpp
