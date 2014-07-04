AM_CPPFLAGS= -fPIC\
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

#<boost/unordered_map.hpp>应该加什么编译选项
AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD = 	../../Catalog/libcatalog.a \
		../../common/libcommon.a \
		../../common/Schema/libschema.a \
		../../utility/libutility.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serializaiton.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a

noinst_LIBRARIES=libblock.a

libblock_a_SOURCES = 	\
	Block.cpp 					Block.h \
	BlockContainer.cpp 			BlockContainer.h \
	BlockStream.cpp 				BlockStream.h \
	BlockStreamBuffer.cpp 			BlockStreamBuffer.h \
	DynamicBlockBuffer.cpp 			DynamicBlockBuffer.h \
	MonitorableBuffer.cpp 			MonitorableBuffer.h \
	PartitionedBlockBuffer.cpp 		PartitionedBlockBuffer.h \
	PartitionedBlockContainer.cpp 	PartitionedBlockContainer.h \
	ResultSet.cpp 				ResultSet.h