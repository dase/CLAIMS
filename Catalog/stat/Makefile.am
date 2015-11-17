
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization

AM_LDFLAGS=-lc -lm -lrt lboost_serialization -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../libcatalog.a \
		../../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../../logical_operator/liblogicalqueryplan.a \
		../../common/Block/libblock.a \
		../../common/libcommon.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${BOOST_HOME}/stage/lib/libboost_system.so

noinst_LIBRARIES=libstat.a

libstat_a_SOURCES = \
	Analyzer.cpp 			Analyzer.h \
	AttributeStatistics.cpp 	AttributeStatistics.h \
	Estimation.cpp 			Estimation.h \
	StatManager.cpp 			StatManager.h \
	Statistic.cpp 			Statistic.h \
	TableStatistic.cpp 		TableStatistic.h 
