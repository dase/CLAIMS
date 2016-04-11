AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME}/boost/serialization \
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux\
-I${THERON_HOME}/Include \
-I${BOOST_HOME}
#<boost/unordered_map.hpp>添加编译选项
AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lboost_date_time -lboost_system -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD =	../storage/libstorage.a \
		../Resource/libresource.a \
		../utility/libutility.a \
		../common/Schema/libschema.a \
		../loader/libloader.a \
		../common/file_handle/libfilehandle.a \
		../common/libcommon.a \
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
	attribute.h 		catalog.cpp \
	catalog.h 			column.cpp \
	column.h 			partitioner.cpp \
	partitioner.h 		projection_binding.cpp \
	projection_binding.h 		projection.cpp \
	projection.h 		table.cpp \
	table.h 			unordered_map.hpp 

# if it is need to include stat folder and Test folder, add stat and Test at next two lines
SUBDIRS = stat
DIST_SUBDIRS = stat
