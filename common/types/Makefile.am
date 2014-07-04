AM_CPPFLAGS= -fPIC\
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD = 	ttmath/libttmath.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serializaiton.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a \
		/home/imdb/supports/gtest-1.7.0/libgtest.a

noinst_LIBRARIES=libtypes.a

libtypes_a_SOURCES = 	\
	ExportSerializeIo.h 		NValue.cpp \
	NValue.hpp 		value_defs.h
	
SUBDIRS = Test ttmath
DIST_SUBDIRS = Test ttmath