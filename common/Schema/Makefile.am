AM_CPPFLAGS= -fPIC\
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD = 	../libcommon.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serializaiton.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a

noinst_LIBRARIES=libschema.a

libschema_a_SOURCES = 	\
	Schema.cpp 			Schema.h \
	SchemaFix.cpp 		SchemaFix.h \
	SchemaVar.cpp 		SchemaVar.h \
	TupleConvertor.cpp 	TupleConvertor.h
	
SUBDIRS = Test
DIST_SUBDIRS = Test