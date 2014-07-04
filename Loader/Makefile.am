AM_CPPFLAGS= -fPIC\
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux \
-I/home/imdb/supports/Theron-5.01.01/Include

AM_LDFLAGS=-lc -lm -lrt 

LDADD = 	../Catalog/libcatalog.a \
		../Schema/libschema.a \
		../common/Block/libblock.a \
		../common/libcommon.a \
		../common/Schema/libschema.a \
		/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a\
		/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a 

noinst_LIBRARIES=libloader.a
libloader_a_SOURCES = \
	Hdfsloader.cpp 		Hdfsloader.h \
	Hdfsconnector.cpp 	Hdfsconnector.h

SUBDIRS = Test
DIST_SUBDIRS = Test