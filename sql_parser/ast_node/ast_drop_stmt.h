/*
 *  /sql_parser/src/astnode/ast_drop_stmt.h
 *
 *  Created on: Jul 23, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSe
 *
 */

#ifndef SQL_PARSER_AST_NODE_AST_DROP_STMT_H_
#define SQL_PARSER_AST_NODE_AST_DROP_STMT_H_

#include <string>
#include <iostream>  //NOLINT

#include "../ast_node/ast_node.h"

using std::string;

class AstDropIndex : public AstNode {
 public:
  AstDropIndex(AstNodeType ast_node_type, string index_name, string table_name);
  ~AstDropIndex();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string index_name_;
  string table_name_;
};

class AstDropDatabase : public AstNode {
 public:
  AstDropDatabase(AstNodeType ast_node_type, int drop_type, int check,
                  string db_name);
  ~AstDropDatabase();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string db_name_;
  int drop_type_;
  int check_;
};

class AstDropTable : public AstNode {
 public:
  AstDropTable(AstNodeType ast_node_type, int is_temp, int is_check,
               int option_rc, AstNode* table_list);
  ~AstDropTable();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int is_temp_;
  int is_check_;
  int option_rc_;
  AstNode* table_list_;
};

/***
 * @brief
 */
class AstDropTableList : public AstNode {
 public:
  /**
   * @brief Method description.
   * @param
   * @details  here is an example (additional)
   */
  AstDropTableList(AstNodeType ast_node_type, string db_name, string table_name,
                   AstNode* next);
  /**
   * @brief Method description.
   * @param
   * @details  here is an example (additional)
   */
  ~AstDropTableList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string db_name_;
  string table_name_;
  AstNode* next_;

 private:
};

#endif  // SQL_PARSER_AST_NODE_AST_DROP_STMT_H_
