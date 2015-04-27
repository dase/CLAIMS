/*
 * runparsetree.h
 *
 *  Created on: 2015-2-10
 *      Author: imdb
 */

#ifndef RUNPARSETREE_H_
#define RUNPARSETREE_H_
#include <sys/time.h>
#include <algorithm>
#include <vector>

#include "sql_node_struct.h"
#include "StreamBuffer.h"
#include "../common/Logging.h"
#include "sql.tab.hpp"
#include "sql.lex.h"
extern timeval start_time;

extern vector<Node*> NodePointer;
//
//extern "C" int yylex();
//extern "C" int yyparse();

//extern "C" int yyclearin();
//extern "C" int yyerrok();
extern int yyparse (struct ParseResult* result);

void GetInputSQL(StreamBuffer* stream_buffer, char *desc);

Node * getparsetreeroot();
Node * getparsetreeroot(const char *sql);
Node * getparsetreeroot(struct ParseResult * presult,const char * InputStr);
Node * getparsetreeroot(struct ParseResult * presult);
#endif /* RUNPARSETREE_H_ */
