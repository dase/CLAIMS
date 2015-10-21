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

#ifdef OLDSQL
#include "sql.tab.hpp"
#include "sql.lex.h"
#endif
extern timeval start_time;

extern vector<Node*> NodePointer;

extern int yyparse(struct ParserResult* result);

void GetInputSQL(StreamBuffer* stream_buffer, char* desc);

Node* getparsetreeroot();
Node* getparsetreeroot(const char* sql);
Node* getparsetreeroot(struct ParserResult* presult, const char* InputStr);
Node* getparsetreeroot(struct ParserResult* presult);
#endif /* RUNPARSETREE_H_ */
