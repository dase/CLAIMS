AM_CPPFLAGS= -fPIC -fpermissive

AM_LDFLAGS=-lc -lm -lrt -lxs 

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = ../common/Schema/libschema.a 


noinst_LIBRARIES=libutility.a
libutility_a_SOURCES = \
	ExpandabilityShrinkability.cpp 		ExpandabilityShrinkability.h \
	ThreadSafe.cpp 						ThreadSafe.h \
	Timer.h 							atomics.h \
	command_line.cpp 		command_line.h \
	cpu_scheduler.h 		data_distribution_generator.cpp \
	data_distribution_generator.h 		generator.h \
	lock.h 			lock_guard.h \
	maths.h 		print_tool.h \
	rdtsc.h 		string_process.cpp \
	string_process.h 		task.cpp \
	task.h 					test_tool.h \
	thread_pool.cpp 		thread_pool.h \
	warmup.h
	