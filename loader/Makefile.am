
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${THERON_HOME}/Include

AM_LDFLAGS=-lc -lm -lrt  -lxs -lboost_serialization

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../catalog/libcatalog.a \
		../common/libcommon.a \
		../common/Block/libblock.a \
		../common/file_handle/libfilehandle.a \
		../common/Schema/libschema.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a 

noinst_LIBRARIES=libloader.a
libloader_a_SOURCES = \
	data_injector.cpp \
	data_injector.h 				file_connector.h \
	single_file_connector.cpp 		single_file_connector.h \
	table_file_connector.cpp 		table_file_connector.h  \
	validity.h 						validity.cpp

SUBDIRS = test
DIST_SUBDIRS = test
