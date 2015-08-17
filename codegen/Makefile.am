AM_CPPFLAGS= -fPIC -fpermissive -DTHERON_XS \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${THERON_HOME}/Include \
-I${GTEST_HOME}/include \
-I/usr/local/include  -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS \
-D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -g \
-std=c++11 -fvisibility-inlines-hidden  \
-fPIC -Woverloaded-virtual -ffunction-sections \
-fdata-sections -Wcast-qual
#-fno-rtti -fno-exceptions
AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lpthread -lxs -L/usr/local/lib 

LDADD = ../common/libcommon.a \
		../utility/libutility.a \
		types/libtypes.a \
		${GTEST_HOME}/libgtest.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so
		

noinst_LIBRARIES=libcodegen.a
libcodegen_a_SOURCES = \
    CodeGenerator.h			CodeGenerator.cpp \
    ExpressionGenerator.h	ExpressionGenerator.cpp \
    CompareFunctonGenerator.h CompareFunctonGenerator.cpp 
SUBDIRS = 
DIST_SUBDIRS = 
