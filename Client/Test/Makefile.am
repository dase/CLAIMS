
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux\
-I${THERON_HOME}/Include

#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization -lxs

LDADD = 	../../Catalog/libcatalog.a \
		../../common/libcommon.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a \
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.so \
		${THERON_HOME}/Lib/libtherond.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${BOOST_HOME}/stage/lib/libboost_serialization.a

noinst_LIBRARIES=libtest.a

libtest_a_SOURCES = TestSeverClient.cpp
