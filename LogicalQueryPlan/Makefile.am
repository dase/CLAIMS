AM_CPPFLAGS= -fPIC -fpermissive \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux\
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

LDADD = 	../LogicalQueryPlan/liblogicalqueryplan.a \
		../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../BlockStreamIterator/libblockstreamiterator.a \
		../Catalog/libcatalog.a \
		../Catalog/stat/libstat.a \
		../Scheduler/libscheduler.a \
		../Schema/libschema.a \
		../Resource/libresource.a \
		../common/libcommon.a \
		../common/Schema/libschema.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_system.a

noinst_LIBRARIES=liblogicalqueryplan.a
liblogicalqueryplan_a_SOURCES = \
	Aggregation.cpp 				Aggregation.h \
	Buffer.cpp 					Buffer.h \
	DataflowPartition.cpp 			DataflowPartition.h \
	DataflowPartitionDescriptor.cpp 	DataflowPartitionDescriptor.h \
	EqualJoin.cpp 				EqualJoin.h \
	Filter.cpp 					Filter.h \
	LogicalOperator.cpp 			LogicalOperator.h \
	LogicalQueryPlanRoot.cpp 		LogicalQueryPlanRoot.h \
	Project.cpp 				Project.h \
	Requirement.cpp 				Requirement.h \
	Scan.cpp 					Scan.h \
	Sort.cpp 					Sort.h

SUBDIRS = Test
DIST_SUBDIRS = Test
					
