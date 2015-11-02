AM_CPPFLAGS= -fPIC \
		-I${GLOG_HOME}/src

AM_LDFLAGS=-lc -lm -lrt -lglog  -lpthread -ll -ly

#LDADD=${GLOG_HOME}/.libs/libglog.so.0

noinst_LIBRARIES=liblog.a
liblog_a_SOURCES = \
	logging.cpp			logging.h 