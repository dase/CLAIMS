AM_CPPFLAGS= -fPIC -fpermissive\
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${GTEST_HOME}/include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs -lpthread  -lglog

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD =	../../Executor/libexecutor.a \
	    ../../catalog/libcatalog.a \
		../../common/libcommon.a \
		../../common/Schema/libschema.a \
		../../common/Block/libblock.a \
		../../common/types/libtypes.a \
		../../common/log/liblog.a	\
	    ../../logical_operator/liblogicalqueryplan.a \
		../../common/expression/libexpression.a	\
		../../utility/libutility.a \
		/usr/local/lib/libglog.a	\
		/usr/local/lib/libglog.so	\
		${HADOOP_HOME}/lib/native/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so \
		${GTEST_HOME}/libgtest.a


noinst_LIBRARIES=libast_node.a

libast_node_a_SOURCES = \
    ast_desc_stmt.cpp           ast_desc_stmt.h \
    ast_delete_stmt.cpp          ast_delete_stmt.h  \
    ast_update_stmt.cpp          ast_update_stmt.h  \
	ast_show_stmt.cpp           ast_show_stmt.h \
	ast_select_stmt.cpp			ast_select_stmt.h \
	ast_create_stmt.cpp			ast_create_stmt.h \
	ast_drop_stmt.cpp			ast_drop_stmt.h	\
	ast_insert_stmt.cpp			ast_insert_stmt.h	\
	ast_load_stmt.cpp           ast_load_stmt.h \
	ast_expr_node.cpp			ast_expr_node.h \
	ast_node.cpp				ast_node.h 