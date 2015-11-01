
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${GTEST_HOME}/include

#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../libschema.a \
		../../../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../../../BlockStreamIterator/libblockstreamiterator.a \
		../../../logical_operator/liblogicalqueryplan.a \
		../../../Catalog/libcatalog.a \
		../../../common/Block/libblock.a \
		../../libcommon.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a \
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.so \
		${THERON_HOME}/Lib/libtherond.a	\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
				${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libtest.a

libtest_a_SOURCES = 
