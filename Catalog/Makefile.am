AM_CPPFLAGS= -fPIC \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux\
-I${THERON_HOME}/Include

#<boost/unordered_map.hpp>添加编译选项
AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lboost_date_time -lboost_system

LDADD =	../storage/libstorage.a \
		../Resource/libresource.a \
		../Schema/libschema.a \
		../common/libcommon.a \
		../common/Schema/libschema.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${BOOST_HOME}/stage/lib/libboost_system.so \
		${THERON_HOME}/Lib/libtherond.a

#DIR = ${shell /bin/pwd}
#INCLUDES = -I${DIR}/ThirdParty

#export INCLUDES 

#noinst means not install
noinst_LIBRARIES=libcatalog.a
libcatalog_a_SOURCES = \
	Attribute.h 		Catalog.cpp \
	Catalog.h 			Column.cpp \
	Column.h 			Partitioner.cpp \
	Partitioner.h 		ProjectionBinding.cpp \
	ProjectionBinding.h 	table.cpp \
	table.h 			unordered_map.hpp

SUBDIRS = stat Test
DIST_SUBDIRS = stat Test

#libcatalog_a_LIBADD = ../Block/libblock.a \
#				../storage/libstorage.a \
#				../Resource/libresource.a \
#				../Schema/libschema.a