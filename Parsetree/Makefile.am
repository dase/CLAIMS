AM_CPPFLAGS= -fPIC -fpermissive \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization

AM_LDFLAGS=-lc -lm -lrt -lboost_system -lboost_serialization -ll -ly -lstdc++ -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = ../Catalog/libcatalog.a \
		../Catalog/stat/libstat.a \
		../logical_operator/liblogicalqueryplan.a \
		../Loader/libloader.a \
		../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../common/libcommon.a \
		../common/Expression/libexpression.a \
		../utility/libutility.a \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${BOOST_HOME}/stage/lib/libboost_system.so \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a

noinst_LIBRARIES=libparsetree.a

libparsetree_a_SOURCES = \
	semantic_analysis.cpp 		ExecuteLogicalQueryPlan.cpp \
	ExecuteLogicalQueryPlan.h 	function.cpp \
	lex.yy.cpp 				parsetree2logicalplan.cpp \
	preprocess.cpp \
	StreamBuffer.h			StreamBuffer.cpp \
	runparsetree.h			runparsetree.cpp \
	sql.tab.cpp 			sql.tab.hpp \
	sql_node_struct.h 		wc2tb.cpp

SUBDIRS = Test
DIST_SUBDIRS = Test
