AM_CPPFLAGS= -fPIC\
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD = 	/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serializaiton.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a

noinst_LIBRARIES=libttmath.a

libttmath_a_SOURCES = 	\
	ttmathint.h 		ttmathmisc.h \
	ttmathtypes.h 		ttmathuint.h \
	ttmathuint_noasm.h 	ttmathuint_x86.h \
	ttmathuint_x86_64.h
	