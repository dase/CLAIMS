AM_CPPFLAGS= -fPIC \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux\
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization

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
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a

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
					
