AM_CPPFLAGS= -fPIC -fpermissive -DTHERON_XS \
-I${BOOST_HOME} \
-I${BOOST_HOME}/boost/serialization \
-I${GTEST_HOME}/include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lpthread -lxs

if OPT_TCMALLOC
AM_CPPFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
AM_LDFLAGS+=-ltcmalloc 
endif

#LDADD = 	../BlockStreamIterator/Serialization/libserialization.a \
#		../BlockStreamIterator/libblockstreamiterator.a \
#		../storage/libstorage.a \
#		../common/libcommon.a \
#		../common/liblog.a \
#		../common/Block/libblock.a \
#		../utility/libutility.a \
#		types/libtypes.a 
#		${BOOST_HOME}/stage/lib/libboost_serialization.a \
#		${BOOST_HOME}/stage/lib/libboost_serialization.so \
#
#		${GTEST_HOME}/libgtest.a

noinst_LIBRARIES=libcommon.a
libcommon_a_SOURCES = \
	AttributeComparator.cpp 		AttributeComparator.h \
	Comparator.cpp 				Comparator.h \
	ExpandedThreadTracker.cpp 		ExpandedThreadTracker.h \
	InsertOptimizedHashTable.cpp 		InsertOptimizedHashTable.h \
	Logging.cpp 		Logging.h \
	Mapping.cpp 		Mapping.h \
	Message.cpp 		Message.h \
	TypeCast.cpp 		TypeCast.h \
	TypePromotionMap.cpp 		TypePromotionMap.h \
	data_type.h 		data_type.cpp functions.h \
	hash.cpp 			hash.h \
	hashtable.cpp 		hashtable.h \
	ids.cpp 			ids.h \
	rename.h			error_define.h \
	error_no.h			error_no.cpp \
	memory.h

SUBDIRS = Block file_handle log Schema types Expression expression serialization
DIST_SUBDIRS = Block file_handle log Schema types Expression expression serialization
