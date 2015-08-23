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
    SQL_statement_ = string(InputStr);
    CreateRawAST(SQL_statement_);
}

Parser::Parser(string SQL_statement)
        : SQL_statement_(SQL_statement) {
    CreateRawAST(SQL_statement_);
}

Parser::~Parser() {
    delete AST_root_;
}

AstNode* Parser::GetRawAST() {
    return AST_root_;
}

AstNode* Parser::CreateRawAST(string SQL_statement) {
    struct ParseResult presult = { NULL, NULL, SQL_statement.c_str(), 0 };
    if (yylex_init_extra(&presult, &presult.yyscan_info_)) {
        perror("init alloc failed");
        AST_root_ = NULL;
    } else {
        YY_BUFFER_STATE bp;
        bp = yy_scan_bytes(SQL_statement.c_str(), SQL_statement.size(),
                           presult.yyscan_info_);
        yy_switch_to_buffer(bp, presult.yyscan_info_);
        if (!yyparse(&presult)) {
          if (presult.ast != NULL) {
            AST_root_ = presult.ast;
          } else {
            printf("SQL parse failed, ast is null!\n");
            AST_root_ = NULL;
          }
        } else {
            printf("SQL parse failed\n");
            AST_root_ = NULL;
        }
        yy_delete_buffer(bp, presult.yyscan_info_);
    }
    return AST_root_;
}

