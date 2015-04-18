AM_CPPFLAGS= -fPIC -fpermissive

AM_LDFLAGS=-lc -lm -lrt -lxs 

LDADD = ../common/Schema/libschema.a 


noinst_LIBRARIES=libutility.a
libutility_a_SOURCES = \
	ExpandabilityShrinkability.cpp 		ExpandabilityShrinkability.h \
	ThreadSafe.cpp 						ThreadSafe.h \
	Timer.h 							atomics.h \
	generator.h 						lock.h \
	rdtsc.h 							test_tool.h \
	warmup.h \
	data_distribution_generator.cpp 	data_distribution_generator.h \
	command_line.cpp 					command_line.h\
	string_process.cpp					string_process.h
