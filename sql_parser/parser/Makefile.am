AM_CPPFLAGS= -fPIC 

AM_LDFLAGS=-lc -lm -lrt -lpthread -ll -ly

noinst_LIBRARIES=lib_parser.a
lib_parser_a_SOURCES = \
	sql_parser.cpp					sql_parser.h \
	sql.tab.cpp 					sql.tab.hpp \
	lex.yy.cpp 						sql.lex.h
