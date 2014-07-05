AM_CPPFLAGS= -fPIC\
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization \
-I/home/imdb/supports/gtest-1.7.0/include

AM_LDFLAGS=-lc -lm -lrt lboost_serialization

LDADD = 	../../Catalog/libcatalog.a \
		../../common/libcommon.a \
		../../common/Schema/libschema.a \
		../../utility/libutility.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_system.a \
				/home/imdb/supports/gtest-1.7.0/libgtest.a

noinst_LIBRARIES=libblock.a

libblock_a_SOURCES = 	\
	Block.cpp 						Block.h \
	BlockContainer.cpp 				BlockContainer.h \
	BlockStream.cpp 				BlockStream.h \
	BlockStreamBuffer.cpp 			BlockStreamBuffer.h \
	DynamicBlockBuffer.cpp 			DynamicBlockBuffer.h \
	MonitorableBuffer.cpp 			MonitorableBuffer.h \
	PartitionedBlockBuffer.cpp 		PartitionedBlockBuffer.h \
	PartitionedBlockContainer.cpp 	PartitionedBlockContainer.h \
	ResultSet.cpp 				    ResultSet.h