/*
 * Parser.h
 *
 *  Created on: May 8, 2015 10:10:18 AM
 *      Author: fzh
 */

#ifndef SQL_PARSER_PARSER_PARSER_H_
#define SQL_PARSER_PARSER_PARSER_H_
#include <string>
#include <vector>

#include "../../sql_parser/ast_node/ast_node.h"

using std::string;
using std::vector;

class Parser {
 public:
  Parser();
  explicit Parser(string SQL_statement);
  virtual ~Parser();
  AstNode* GetRawAST();
  string get_sql_stmt() { return sql_stmt_; }

 private:
  AstNode* CreateRawAST(string SQL_statement);
  // AstNode* CreateAst(string SQL_statement);
  void SemanticAnalysis();
  void RecoveryExpr();
  string sql_stmt_;
  AstNode* ast_root_;
};
#endif  // SQL_PARSER_PARSER_PARSER_H_
