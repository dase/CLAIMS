
AM_CPPFLAGS= -fPIC -fpermissive 

AM_LDFLAGS=-lc -lm -lrt	

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

noinst_LIBRARIES=libjson.a
libjson_a_SOURCES = \
	json.h				json-forwards.h

