AM_CPPFLAGS= -fPIC -fpermissive -DTHERON_XS\
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux

AM_LDFLAGS=-lc -lm -lrt -lconfig++ -lpthread -lboost_serialization -lxs

LDADD = 	../../Catalog/libcatalog.a \
		../../common/libcommon.a \
		../../common/Schema/libschema.a \
		../../utility/libutility.a \
		../../parsetree/libparsetree.a \
		${THERON_HOME}/Lib/libtherond.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_system.a \
		${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libexpression.a

libexpression_a_SOURCES = 	\
	expr_node.cpp				expr_node.h			\
	expr_column.cpp				expr_column.h		\
	data_type_oper.cpp			data_type_oper.h	\
	expr_const.cpp				expr_const.h		\
	expr_binary.cpp				expr_binary.h		\
	expr_case_when.cpp			expr_case_when.h	\
	expr_date.cpp				expr_date.h			\
	expr_in.cpp					expr_in.h			\
	expr_ternary.cpp			expr_ternary.h		\
	expr_type_cast.cpp			expr_type_cast.h	\
	type_conversion_matrix.cpp 	type_conversion_matrix.h	\
	expr_unary.cpp				expr_unary.h		
