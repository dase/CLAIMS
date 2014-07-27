AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${THERON_HOME}/Include

AM_LDFLAGS=-lc -lm -lrt  -lxs

LDADD = 	../Catalog/libcatalog.a \
		../Schema/libschema.a \
		../common/Block/libblock.a \
		../common/libcommon.a \
		../common/Schema/libschema.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a 

noinst_LIBRARIES=libloader.a
libloader_a_SOURCES = \
	Hdfsloader.cpp 		Hdfsloader.h \
	Hdfsconnector.cpp 	Hdfsconnector.h

SUBDIRS = Test
DIST_SUBDIRS = Test