AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = ../Executor/libexecutor.a \
		../common/libcommon.a \
		../../storage/libstorage.a \
		../common/Schema/libschema.a \
		../common/Block/libblock.a	\
		../utility/libutility.a \
		../codegen/codegen.a\
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a \
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${THERON_HOME}/Lib/libtherond.a
		
noinst_LIBRARIES=libphysicalqueryplan.a
libphysicalqueryplan_a_SOURCES = \
	physical_operator.cpp 					physical_operator.h	\
	exchange_sender.cpp 					exchange_sender.h \
	physical_operator_base.cpp 				physical_operator_base.h \
	physical_limit.cpp 						physical_limit.h \
	performance_monitor.cpp 				performance_monitor.h \
	result_printer.cpp 						result_printer.h \
	result_collector.cpp 					result_collector.h \
	physical_aggregation.cpp 				physical_aggregation.h \
	combine_tuple.cpp 						combine_tuple.h \
	expander.cpp 							expander.h \
	in_operator.cpp 						in_operator.h \
	physical_project.cpp 					physical_project.h \
	physical_hash_join.cpp 						physical_hash_join.h \
	physical_sort.cpp 						physical_sort.h \
	exchange_merger.cpp 					physical_exchange_merger.h \
	exchange_sender_pipeline.cpp 			exchange_sender_pipeline.h \
	exchange_sender_materialized.cpp 		exchange_sender_materialized.h \
	physical_filter.cpp 					physical_filter.h \
	physical_projection_scan.cpp 			physical_projection_scan.h \
	physical_nest_loop_join.h				physical_nest_loop_join.cpp	
			
SUBDIRS = 
DIST_SUBDIRS = 
