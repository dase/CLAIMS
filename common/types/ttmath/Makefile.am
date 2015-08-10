AM_CPPFLAGS= -fPIC -fpermissive \
	-I${BOOST_HOME} \
	-I${BOOST_HOME}/boost/serialization \
	-I${GTEST_HOME}/include


#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD =	\
	${BOOST_HOME}/stage/lib/libboost_serialization.a \
	${BOOST_HOME}/stage/lib/libboost_serialization.so \
	${BOOST_HOME}/stage/lib/libboost_system.a \
	${BOOST_HOME}/stage/lib/libboost_system.so \
	${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libttmath.a

libttmath_a_SOURCES = 	\
	ttmathint.h 		ttmathmisc.h \
	ttmathtypes.h 		ttmathuint.h \
	ttmathuint_noasm.h 	ttmathuint_x86.h \
	ttmathuint_x86_64.h
	
