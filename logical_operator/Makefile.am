
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

LDADD = 	../logical_operator/liblogicalqueryplan.a \
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
	logical_aggregation.cpp 			logical_aggregation.h \
	plan_partition_info.cpp 			plan_partition_info.h \
	plan_partitioner.cpp 				plan_partitioner.h \
	plan_context.cpp					plan_context.h\
	logical_equal_join.cpp 				logical_equal_join.h \
	logical_filter.cpp 					logical_filter.h \
	logical_operator.cpp 				logical_operator.h \
	logcial_query_plan_root.h			logical_query_plan_root.cpp \
	logical_project.cpp 				logical_project.h \
	Requirement.cpp 					Requirement.h \
	logical_scan.cpp 					logical_scan.h \
	logical_limit.cpp                   logical_limit.h \
	logical_cross_join.cpp				logical_cross_join.h \
	logical_sort.cpp 					logical_sort.h 

SUBDIRS = 
DIST_SUBDIRS = 
					
