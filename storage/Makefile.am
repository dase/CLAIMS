AM_CPPFLAGS= -fPIC -fpermissive\
-I${HADOOP_HOME}/include\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization

AM_LDFLAGS=-lc -lm -lrt  -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

LDADD = 	../BufferManager/libbuffermanager.a \
		../common/libcommon.a \
		../common/Block/libblock.a \
		../utility/libutility.a \
		${HADOOP_HOME}/lib/native/libhdfs.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a \
		${BOOST_HOME}/stage/lib/libboost_serialization.a

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
