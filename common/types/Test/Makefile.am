AM_CPPFLAGS= -fPIC\
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/gtest-1.7.0/include

#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD = 	../libtypes.a \
		../../Schema/libschema.a \
		../../../utility/libutility.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a \
		/home/imdb/supports/gtest-1.7.0/libgtest.a

noinst_LIBRARIES=libtest.a

libtest_a_SOURCES = data_type_test.h
	