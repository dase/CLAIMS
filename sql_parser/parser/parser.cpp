/*
 * Parser.cpp
 *
 *  Created on: May 8, 2015 10:10:18 AM
 *      Author: fzh
 */

#include <stdio.h>
#include <string>

#include "./sql.tab.hpp"
#include "./sql.lex.h"
#include "./parser.h"

#include "../ast_node/ast_node.h"

extern int yyparse(struct ParseResult* result);
using namespace std;
Parser::Parser() {
  puts("please input sql:");
  char InputStr[1000];
  int charnum = 0;
  while (1) {
    char c = getchar();
    InputStr[charnum++] = c;
    if (c == ';') {
      InputStr[charnum++] = '\0';
      break;
    }
  }
  sql_stmt_ = string(InputStr);
  CreateRawAST(sql_stmt_);
}

Parser::Parser(string sql_stmt) : sql_stmt_(sql_stmt) {
  CreateRawAST(sql_stmt_);
  // CreateAst();
}

Parser::~Parser() { delete ast_root_; }

AstNode* Parser::GetRawAST() { return ast_root_; }

AstNode* Parser::CreateRawAST(string SQL_statement) {
  struct ParseResult presult = {NULL, NULL, SQL_statement.c_str(), 0};
  if (yylex_init_extra(&presult, &presult.yyscan_info_)) {
    perror("init alloc failed");
    ast_root_ = NULL;
  } else {
    YY_BUFFER_STATE bp;
    bp = yy_scan_bytes(SQL_statement.c_str(), SQL_statement.size(),
                       presult.yyscan_info_);
    yy_switch_to_buffer(bp, presult.yyscan_info_);
    if (!yyparse(&presult)) {
      if (presult.ast != NULL) {
        ast_root_ = presult.ast;
      } else {
        printf("SQL parse failed, ast is null!\n");
        ast_root_ = NULL;
      }
    } else {
      printf("SQL parse failed\n");
      ast_root_ = NULL;
    }
    yy_delete_buffer(bp, presult.yyscan_info_);
  }
  return ast_root_;
}
/*
 * @brief TODO(yuyang):for dml/query, add semantic analysis and recover
 * expression string
 */
// AstNode* Parser::CreateAst() { AstNode* entire_AST = AST_root_; }
