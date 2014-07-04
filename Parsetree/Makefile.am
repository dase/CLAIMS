AM_CPPFLAGS= -fPIC \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

AM_LDFLAGS=-lc -lm -lrt -lboost_system -lboost_serialization -ll -ly -lstdc++

LDADD = ../Catalog/libcatalog.a \
		../Catalog/stat/libstat.a \
		../LogicalQueryPlan/liblogicalqueryplan.a \
		../Loader/libloader.a \
		../BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
		../common/libcommon.a \
		../utility/libutility.a

noinst_LIBRARIES=libparsetree.a

libparsetree_a_SOURCES = \
	semantic_analysis.cpp 		ExecuteLogicalQueryPlan.cpp \
	ExecuteLogicalQueryPlan.h 	function.cpp \
	lex.yy.cpp 				parsetree2logicalplan.cpp \
	preprocess.cpp 			runparsetree.cpp \
	sql.tab.cpp 			sql.tab.hpp \
	sql_node_struct.h 		wc2tb.cpp

SUBDIRS = Test
DIST_SUBDIRS = Test