AM_CPPFLAGS= -fPIC -fpermissive -DTHERON_XS\
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lconfig++ -lpthread -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = ../../common/libcommon.a 


noinst_LIBRARIES=libfilehandle.a

libfilehandle_a_SOURCES = 	\
	disk_file_handle_imp.cpp 		disk_file_handle_imp.h \
	file_handle_imp.cpp \
	file_handle_imp.h 				file_handle_imp_factory.h \
	hdfs_file_handle_imp.cpp 		hdfs_file_handle_imp.h 


SUBDIRS = test
DIST_SUBDIRS = test