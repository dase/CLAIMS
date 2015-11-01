/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *  /sql_parser/src/astnode/ast_insert_stmt.h
 *
 *  Created on: Jul 23, 2015
 *      Author: yuyang
 *       Email: youngfish93@hotmail.com
 *
 */

#ifndef SQL_PARSER_AST_NODE_AST_INSERT_STMT_H_
#define SQL_PARSER_AST_NODE_AST_INSERT_STMT_H_
#include <string>
#include <iostream>  //NOLINT

#include "../ast_node/ast_node.h"

using std::string;
// namespace claims {
// namespace sql_parser {
/***
 * @brief The AST of insert statement.
 * @details AstInsertStmt mainly includes table name, column name and value
 * list.
 * TODO(yuyang):There are some members not used in this version.
 */
class AstInsertStmt : public AstNode {
 public:
  AstInsertStmt(AstNodeType ast_node_type, int insert_opt, string table_name,
                AstNode* col_list, AstNode* insert_val_list,
                AstNode* insert_assign_list,
                AstNode* insert_assign_list_from_set, AstNode* select_stmt);
  ~AstInsertStmt();
  void Print(int level = 0) const;

 public:
  AstNodeType ast_node_type_;
  int insert_opt_;  // 2014-4-16---not used in this version
  string table_name_;
  AstNode* col_list_;
  AstNode* insert_val_list_;
  AstNode* insert_assign_list_;  // 2014-4-16---not used in this version
  AstNode* insert_assign_list_from_set_;  // 2014-4-17--not used in this version
  AstNode* select_stmt_;  // 2014-4-16---not used in this version

 private:
};
/**
 * @brief The data structure of value list from insert statement.
 * @details AstInsertValList mainly includes the pointer of insert value list.
 */
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
/**
 * @brief The information of value in the insert statement.
 * @details AstInsertVals mainly includes the type of value and expression list.
 * The member value_type_ stands for having DEFAULT or not.(1 means having
 * DEFAULT while 0 means not.)
 */
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
/**
 * @brief The AST of insert assign list from insert statement, usually after SET
 * statement.
 * @details AstInsertAssignList mainly includes the column names and types or
 * expression.
 * The member value_type_ stands for having DEFAULT or not.(1 means having
 * DEFAULT while 0 means not.)
 */
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
//}  // namespace sql_parser
//}  // namespace claims

#endif  // SQL_PARSER_AST_NODE_AST_INSERT_STMT_H_
