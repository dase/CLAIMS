
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${GTEST_HOME}/include

#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD = 	../libtypes.a \
		../../Schema/libschema.a \
		../../../utility/libutility.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libtest.a

libtest_a_SOURCES =
	
