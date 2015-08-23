/*
 *  /sql_parser/src/astnode/ast_insert_stmt.h
 *
 *  Created on: Jul 23, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSe
 *
 */

#ifndef SQL_PARSER_AST_NODE_AST_INSERT_STMT_H_
#define SQL_PARSER_AST_NODE_AST_INSERT_STMT_H_
#include <string>
#include <iostream>  //NOLINT

#include "../ast_node/ast_node.h"

using std::string;

/***
 * @brief Insert Statment
 */
class AstInsertStmt : public AstNode {
 public:
  /**
   * @brief Method description.
   * @param
   */
  AstInsertStmt(AstNodeType ast_node_type, int insert_opt, string table_name,
                AstNode* col_list, AstNode* insert_val_list,
                AstNode* insert_assign_list,
                AstNode* insert_assign_list_from_set, AstNode* select_stmt);
  /**
   * @brief Method description.
   * @param
   */
  ~AstInsertStmt();
  void Print(int level = 0) const;

 public:
  AstNodeType ast_node_type_;
  int insert_opt_;  // 2014-4-16---not used in this version
  string table_name_;
  AstNode *col_list_;
  AstNode *insert_val_list_;
  AstNode *insert_assign_list_;  // 2014-4-16---not used in this version
  AstNode *insert_assign_list_from_set_;  // 2014-4-17--not used in this version
  AstNode *select_stmt_;  // 2014-4-16---not used in this version

 private:
};

class AstInsertValList : public AstNode {
 public:
  AstInsertValList(AstNodeType ast_node_type, AstNode* insert_vals,
                   AstNode* next);
  ~AstInsertValList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  AstNode* insert_vals_;
  AstNode* next_;
};

class AstInsertVals : public AstNode {
 public:
  AstInsertVals(AstNodeType ast_node_type, int value_type, AstNode* expr,
                AstNode* next);
  ~AstInsertVals();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int value_type_;
  AstNode* expr_;
  AstNode* next_;
};

class AstInsertAssignList : public AstNode {
 public:
  AstInsertAssignList(AstNodeType ast_node_type, string col_name,
                      int value_type, AstNode* expr, AstNode* next);
  ~AstInsertAssignList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string col_name_;
  int value_type_;
  AstNode* expr_;
  AstNode* next_;
};

#endif  // SQL_PARSER_AST_NODE_AST_INSERT_STMT_H_
