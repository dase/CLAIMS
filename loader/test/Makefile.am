
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include \
-I${JAVA_HOME}/include/linux \
-I${GTEST_HOME}/include	\
-I${BOOST_HOME}
AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = ../../catalog/libcatalog.a \
		../libloader.a \
		${HADOOP_HOME}/lib/native/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so 

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = \
	single_file_connector_test.h	single_file_connector_test.cpp \
	single_thread_single_file_connector_test.h\
	table_file_connector_test.h 	table_file_connector_test.cpp \
	data_injector_test.cpp			data_injector_test.h
