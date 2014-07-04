AM_CPPFLAGS= -fPIC \
-I/home/NewHadoop/hadoop/hadoop-1.0.3/src/c++/libhdfs\
-I/home/NewHadoop/java/jdk1.7.0_45/include\
-I/home/NewHadoop/java/jdk1.7.0_45/include/linux \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/boost_1_53_0/boost/serialization

AM_LDFLAGS=-lc -lm -lrt 

LDADD = 	../BufferManager/libbuffermanager.a \
		../common/libcommon.a \
		../common/Block/libblock.a \
		../utility/libutility.a \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a

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
