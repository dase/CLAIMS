AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

LDADD = /home/imdb/supports/boost_1_53_0/stage/lib/libboost_serializaiton.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serializaiton.so \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a
		
		

AM_LDFLAGS=-lc -lm -lrt -lpthread -lboost_serialization -lboost_date_time -lboost_system -lconfig++ -lxs

noinst_LIBRARIES=libserialization.a
libserialization_a_SOURCES = \
	unordered_collections_load_imp.hpp	unordered_collections_save_imp.hpp