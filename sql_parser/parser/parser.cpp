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

#include "../../common/memory_handle.h"
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
  string result = "";
  CreateRawAST(sql_stmt_, result);
}

Parser::Parser(string sql_stmt, string& result_info) : sql_stmt_(sql_stmt) {
  CreateRawAST(sql_stmt_, result_info);
  // CreateAst();
}

Parser::~Parser() { DELETE_PTR(ast_root_); }

AstNode* Parser::GetRawAST() { return ast_root_; }

AstNode* Parser::CreateRawAST(string SQL_statement, string& result_info) {
  struct ParseResult presult = {NULL, NULL, SQL_statement.c_str(), 0, ""};
  if (yylex_init_extra(&presult, &presult.yyscan_info_)) {
    // if (yylex_init(&presult.yyscan_info_)) {
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
        result_info = presult.error_info_;
        ast_root_ = NULL;
      }
    } else {
      printf("SQL parse failed\n");
      ast_root_ = NULL;
    }
    yy_flush_buffer(bp, presult.yyscan_info_);
    yy_delete_buffer(bp, presult.yyscan_info_);
    // yylex_destroy(presult.yyscan_info_);
  }
  return ast_root_;
}
// AstNode* Parser::CreateAst() { AstNode* entire_AST = AST_root_; }
