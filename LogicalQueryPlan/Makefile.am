
AM_CPPFLAGS= -fPIC -fpermissive \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux\
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../LogicalQueryPlan/liblogicalqueryplan.a \
		../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../BlockStreamIterator/libblockstreamiterator.a \
		../Catalog/libcatalog.a \
		../Catalog/stat/libstat.a \
		../Schema/libschema.a \
		../Resource/libresource.a \
		../common/libcommon.a \
		../common/Schema/libschema.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${BOOST_HOME}/stage/lib/libboost_system.so

noinst_LIBRARIES=liblogicalqueryplan.a
liblogicalqueryplan_a_SOURCES = \
	Aggregation.cpp 				Aggregation.h \
	Buffer.cpp 					Buffer.h \
	DataflowPartition.cpp 			DataflowPartition.h \
	DataflowPartitionDescriptor.cpp 	DataflowPartitionDescriptor.h \
	Dataflow.cpp				Dataflow.h\
	EqualJoin.cpp 				EqualJoin.h \
	logical_filter.cpp 			logical_filter.h \
	LogicalOperator.cpp 			LogicalOperator.h \
	logcial_query_plan_root.h	logical_query_plan_root.cpp \
	logical_project.cpp 				logical_project.h \
	Requirement.cpp 				Requirement.h \
	Scan.cpp 					Scan.h \
	Sort.cpp 					Sort.h \
	CrossJoin.cpp				CrossJoin.h

SUBDIRS = Test
DIST_SUBDIRS = Test
					
