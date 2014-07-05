AM_CPPFLAGS= -fPIC \
-I/home/imdb/supports/boost_1_53_0 \
-I/home/imdb/supports/Theron-5.01.01/Include \
-I/home/imdb/supports/gtest-1.7.0/include

AM_LDFLAGS=-lc -lm -lrt -lboost_serialization -lpthread

LDADD = 	../BlockStreamIterator/Serialization/libserialization.a \
		../BlockStreamIterator/libblockstreamiterator.a \
		../storage/libstorage.a \
		../common/libcommon.a \
		../common/Block/libblock.a \
		../utility/libutility.a \
		types/libtypes.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.a \
		/home/imdb/supports/boost_1_53_0/stage/lib/libboost_serialization.so \
		/home/imdb/supports/Theron-5.01.01/Lib/libtherond.a \
		/home/imdb/supports/gtest-1.7.0/libgtest.a

noinst_LIBRARIES=libcommon.a
libcommon_a_SOURCES = \
	AttributeComparator.cpp 		AttributeComparator.h \
	Comparator.cpp 				Comparator.h \
	ExpandedThreadTracker.cpp 		ExpandedThreadTracker.h \
	ExpandedThreadTracker_test.cpp 		ExpressionCalculator.cpp \
	ExpressionCalculator.h 			ExpressionItem.cpp \
	ExpressionItem.h 				Expression_item.cpp \
	InsertOptimizedHashTable.cpp 		InsertOptimizedHashTable.h \
	Logging.cpp 		Logging.h \
	Mapping.cpp 		Mapping.h \
	Message.cpp 		Message.h \
	TimeOutReceiver.cpp 		TimeOutReceiver.h \
	TypeCast.cpp 		TypeCast.h \
	TypePromotionMap.cpp 		TypePromotionMap.h \
	data_type.h 		functions.h \
	hash.cpp 			hash.h \
	hashtable.cpp 		hashtable.h \
	ids.cpp 			ids.h \
	rename.h	

SUBDIRS = Block Schema types
DIST_SUBDIRS = Block Schema types