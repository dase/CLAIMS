AM_CPPFLAGS= -fPIC

AM_LDFLAGS=-lc -lm -lrt

LDADD = 	../storage/libstorage.a \
		../utility/libutility.a \
		../common/libcommon.a

noinst_LIBRARIES=libbuffermanager.a
libbuffermanager_a_SOURCES = \
	BufferManager.cpp    		BufferManager.h

#libbuffermanager_a_LIBADD = ../Block/libblock.a ../storage/libstorage.a