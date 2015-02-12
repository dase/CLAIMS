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

extern Node* parsetreeroot;
extern char globalInputText[10000];
extern int globalReadOffset;
extern int errorNumber;
extern timeval start_time;
extern vector<Node*> NodePointer;

extern "C" int yylex();
extern "C" int yyparse();

//extern "C" int yyclearin();
//extern "C" int yyerrok();

void GetInputSQL(StreamBuffer* stream_buffer, char *desc);

Node * getparsetreeroot();
Node * getparsetreeroot(const char *sql);


#endif /* RUNPARSETREE_H_ */
