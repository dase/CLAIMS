AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/Theron-5.01.01/Include 


AM_LDFLAGS=-lc -lm -lrt -lboost_serialization

LDADD = 	../Catalog/libcatalog.a \
		../Daemon/libdaemon.a \
		../common/Block/libblock.a \
		../common/Schema/libschema.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a		

noinst_LIBRARIES=libclient.a
libclient_a_SOURCES = \
	ClaimsServer.cpp			ClaimsServer.h \
	Client.cpp 				Client.h \
	ClientResponse.cpp 		ClientResponse.h

SUBDIRS = Test
DIST_SUBDIRS = Test