AM_CPPFLAGS= -fPIC

AM_LDFLAGS=-lc -lm -lrt -lxs

LDADD = ../Catalog/libcatalog.a \
		../LogicalQueryPlan/liblogicalqueryplan.a \
		../common/Schema/libschema.a 

noinst_LIBRARIES=libscheduler.a
libscheduler_a_SOURCES = \
	Dataflow.cpp 			Dataflow.h

SUBDIRS = test
DIST_SUBDIRS = test