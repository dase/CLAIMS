AM_CPPFLAGS= -fPIC \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux \
-I/home/imdb/supports/Theron-5.01.01/Include 
#-I/home/imdb/supports/boost_1_53_0 \
#-I/home/imdb/supports/boost_1_53_0/boost/serialization

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization

LDADD = 	/home/NewHadoop/hadoop/hadoop-1.0.3/c++/Linux-amd64-64/lib/libhdfs.a\
		/home/NewHadoop/java/jdk1.7.0_45/jre/lib/amd64/server/libjvm.so\
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a  
#/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a 
#/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so 

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES = TheronTest.cpp