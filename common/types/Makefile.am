
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${GTEST_HOME}/include

#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD = 	ttmath/libttmath.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${BOOST_HOME}/stage/lib/libboost_system.so \
		${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libtypes.a

libtypes_a_SOURCES = 	\
	ExportSerializeIo.h 		NValue.cpp \
	NValue.hpp 		value_defs.h
	
SUBDIRS = Test ttmath
DIST_SUBDIRS = Test ttmath
