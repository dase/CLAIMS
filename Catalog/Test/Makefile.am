AM_CPPFLAGS= -fPIC \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux\
-I${THERON_HOME}/Include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lboost_date_time -lboost_system

LDADD = 	../../LogicalQueryPlan/liblogicalqueryplan.a \
		../stat/libstat.a \
		../libcatalog.a \
		../../common/libcommon.a \
		../../utility/libutility.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${THERON_HOME}/Lib/libtherond.a

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = 	Partitioner_test.cpp 		statistic_manager_test.cpp
