AM_CPPFLAGS= -fPIC

AM_LDFLAGS=-lc -lm -lrt

LDADD = ../Catalog/libcatalog.a \
		../LogicalQueryPlan/liblogicalqueryplan.a \
		../common/Schema/libschema.a 

noinst_LIBRARIES=libscheduler.a
libscheduler_a_SOURCES = \
	Dataflow.cpp 			Dataflow.h

#libscheduler_a_LIBADD = \
#		../Catalog/libcatalog.a \
#		../LogicalQueryPlan/liblogicalqueryplan.a \
#		../Schema/libschema.a \
#		../BlockStreamIterator/libblockstreamiterator.a

SUBDIRS = test
DIST_SUBDIRS = test