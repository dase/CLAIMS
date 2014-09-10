AM_CPPFLAGS= -fPIC -fpermissive -DTHERON_XS\
-I${BOOST_HOME} \
-I${THERON_HOME}/Include 


AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

LDADD = 	../Catalog/libcatalog.a \
		../Daemon/libdaemon.a \
		../common/Block/libblock.a \
		../common/Schema/libschema.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a		

noinst_LIBRARIES=libclient.a
libclient_a_SOURCES = \
	ClaimsServer.cpp			ClaimsServer.h \
	Client.cpp 				Client.h \
	ClientResponse.cpp 		ClientResponse.h

SUBDIRS = Test
DIST_SUBDIRS = Test