AM_CPPFLAGS= -fPIC -fpermissive\
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${GTEST_HOME}/include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../libtest.a \
		../../Catalog/libcatalog.a \
		../../Loader/libloader.a \
		../../logical_operator/liblogicalqueryplan.a \
		../../BlockStreamIterator/libblockstreamiterator.a \
		../../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../../storage/libstorage \
		../../common/libcommon.a \
		../../common/types/libtypes.a \
		../../common/Block/libblock.a \
		../../utility/libutility.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a  \
		${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libtestsuit.a
libtestsuit_a_SOURCES = 
