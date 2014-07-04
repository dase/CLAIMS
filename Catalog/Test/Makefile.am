AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux\
-I/home/imdb/supports/Theron-5.01.01/Include

#<boost/unordered_map.hpp>添加编译选项
AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lboost_date_time -lboost_system

LDADD = 	../../LogicalQueryPlan/liblogicalqueryplan.a \
		../stat/libstat.a \
		../libcatalog.a \
		../../common/libcommon.a \
		../../utility/libutility.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serializaiton.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a

#DIR = ${shell /bin/pwd}
#INCLUDES = -I${DIR}/ThirdParty

#export INCLUDES 

#noinst means not install
noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = 	Partitioner_test.cpp 		statistic_manager_test.cpp


#libcatalog_a_LIBADD = ../Block/libblock.a \
#				../storage/libstorage.a \
#				../Resource/libresource.a \
#				../Schema/libschema.a