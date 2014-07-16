AM_CPPFLAGS= -fPIC\
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${GTEST_HOME}/include

AM_LDFLAGS=-lc -lm -lrt lboost_serialization -lxs

LDADD = 	../libcommon.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_system.a

noinst_LIBRARIES=libschema.a

libschema_a_SOURCES = 	\
	Schema.cpp 			Schema.h \
	SchemaFix.cpp 		SchemaFix.h \
	SchemaVar.cpp 		SchemaVar.h \
	TupleConvertor.cpp 	TupleConvertor.h
	
SUBDIRS = Test
DIST_SUBDIRS = Test