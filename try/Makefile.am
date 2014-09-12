AM_CPPFLAGS= -fPIC -fpermissive\
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization 

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lxs

LDADD = 	../utility/libutility.a \
		${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a\
		${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.so 

noinst_LIBRARIES=libtry.a
libtry_a_SOURCES = \
	Epoll.cpp 				IteratorTreeSerialization.cpp \
	Socket.cpp 				SocketSender.cpp \
	data_generator.cpp 		select.cpp
