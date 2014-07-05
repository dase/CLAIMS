AM_CPPFLAGS= -fPIC\
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/gtest-1.7.0/include

AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD = 	../libcommon.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a

noinst_LIBRARIES=libschema.a

libschema_a_SOURCES = 	\
	Schema.cpp 			Schema.h \
	SchemaFix.cpp 		SchemaFix.h \
	SchemaVar.cpp 		SchemaVar.h \
	TupleConvertor.cpp 	TupleConvertor.h
	
SUBDIRS = Test
DIST_SUBDIRS = Test