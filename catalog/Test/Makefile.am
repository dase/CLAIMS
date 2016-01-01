
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux\
-I${THERON_HOME}/Include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lboost_date_time -lboost_system -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../../logical_operator/liblogicalqueryplan.a \
		../stat/libstat.a \
		../libcatalog.a \
		../../common/libcommon.a \
		../../utility/libutility.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${BOOST_HOME}/stage/lib/libboost_system.so \
		${THERON_HOME}/Lib/libtherond.a

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES =
