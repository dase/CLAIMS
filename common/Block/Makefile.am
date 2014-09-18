
AM_CPPFLAGS= -fPIC -fpermissive \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${GTEST_HOME}/include

AM_LDFLAGS=-lc -lm -lrt lboost_serialization -lxs

LDADD = 	../../Catalog/libcatalog.a \
		../../common/libcommon.a \
		../../common/Schema/libschema.a \
		../../utility/libutility.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_system.a \
				${GTEST_HOME}/libgtest.a

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
