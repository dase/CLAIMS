AM_CPPFLAGS= -fPIC \
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${THERON_HOME}/Include \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization

AM_LDFLAGS=-lc -lm -lrt 

LDADD = 	../BufferManager/libbuffermanager.a \
		../common/libcommon.a \
		../common/Block/libblock.a \
		../utility/libutility.a \
		${THERON_HOME}/Lib/libtherond.a

noinst_LIBRARIES=libstorage.a
libstorage_a_SOURCES = \
	AllBlockInfo.cpp 			AllBlockInfo.h \
	BlanceMatcher.cpp 		BlanceMatcher.h \
	BlockManager.cpp 			BlockManager.h \
	BlockManagerId.cpp 		BlockManagerId.h \
	BlockManagerMaster.cpp 		BlockManagerMaster.h \
	BlockMessage.cpp 			BlockMessage.h \
	BlockStore.cpp 			BlockStore.h \
	ChunkStorage.cpp 			ChunkStorage.h \
	DiskStore.cpp 			DiskStore.h \
	MemoryStore.cpp 			MemoryStore.h \
	PartitionReaderIterator.cpp 	PartitionReaderIterator.h \
	PartitionStorage.cpp 		PartitionStorage.h \
	StorageLevel.h
