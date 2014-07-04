AM_CPPFLAGS= -fPIC

AM_LDFLAGS=-lc -lm -lrt

LDADD = ../common/Schema/libschema.a 

noinst_LIBRARIES=libutility.a
libutility_a_SOURCES = \
	ExpandabilityShrinkability.cpp 		ExpandabilityShrinkability.h \
	ThreadSafe.cpp 		ThreadSafe.h \
	Timer.h 			atomics.h \
	generator.h 		lock.h \
	rdtsc.h 			test_tool.h \
	warmup.h
