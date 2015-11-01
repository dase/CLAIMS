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
 *
 *  /sql_parser/astnode/ast_drop_stmt.h
 *
 *  Created on: Jul 23, 2015
 *      Author: yuyang
 *       Email: youngfish93@hotmail.com
 *
 */

#ifndef SQL_PARSER_AST_NODE_AST_DROP_STMT_H_
#define SQL_PARSER_AST_NODE_AST_DROP_STMT_H_

#include <string>
#include <iostream>  //NOLINT

#include "../ast_node/ast_node.h"

using std::string;
// namespace claims {
// namespace sql_parser {
/**
 * @brief The AST of drop index statement.
 * @details AstDropIndex mainly includes index name and table name.
 */
class AstDropIndex : public AstNode {
 public:
  AstDropIndex(AstNodeType ast_node_type, string index_name, string table_name);
  ~AstDropIndex();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string index_name_;
  string table_name_;
};

/**
 * @brief The AST of drop database statement.
 * @details AstDropDatabase mainly includes database name and drop type.
 * The member drop_type means database or scheme.(1 means drop database while 2
 * means drop scheme.)
 */
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

/**
 * @brief The AST of drop table statement.
 * @details AstDropTalbe mainly includes table list and some options.
 * The member is_temporary stands for having temporary option or not.
 * (1 means having temporary while 0 means not.)
 * The member is_if_exists_ stands for having IF EXISTS or not.
 * (1 means having IF EXISTS while 0 means not.)
 * The member option_rc_ stands for  having RESTRICT, CASCADE or not.
 * (1 means having RESTRICT; 2 means CASCADE; 0 means none of them.)
 */
class AstDropTable : public AstNode {
 public:
  AstDropTable(AstNodeType ast_node_type, int is_temporary, int is_if_exists,
               int option_rc, AstNode* table_list);
  ~AstDropTable();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int is_temporary_;
  int is_if_exists_;
  int option_rc_;
  AstNode* table_list_;
};

/***
 * @brief The data structure of table list in drop table statement.
 * @details AstDropTableList mainly includes database name,(not used in this
 * version) and table name.
 */
class AstDropTableList : public AstNode {
 public:
  AstDropTableList(AstNodeType ast_node_type, string db_name, string table_name,
                   AstNode* next);
  ~AstDropTableList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string db_name_;
  string table_name_;
  AstNode* next_;

 private:
};
//}  // namespace sql_parser
//}  // namespace claims

#endif  // SQL_PARSER_AST_NODE_AST_DROP_STMT_H_
