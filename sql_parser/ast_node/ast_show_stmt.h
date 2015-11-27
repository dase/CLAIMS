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
 * /CLAIMS/sql_parser/ast_node/ast_show_table.h
 *
 *  Created on: Nov 16, 2015
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#ifndef SQL_PARSER_AST_NODE_AST_SHOW_STMT_H_
#define SQL_PARSER_AST_NODE_AST_SHOW_STMT_H_
#include <iostream>  //  NOLINT
#include <iomanip>
#include <string>
#include <vector>

#include "../ast_node/ast_node.h"
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::setw;
using std::vector;

// namespace claims {
// namespace sql_parser {
/**
 * @brief The AST of show statement.
 */
class AstShowStmt : public AstNode {
 public:
  /**
   * @brief AstShowTable Constructor.
   */
  AstShowStmt(AstNodeType ast_node_type, int show_type, bool full,
              string database_name, string like_string);
  /**
   * @brief AstShowTable Destructor.
   */
  ~AstShowStmt();

  void Print(int level = 0) const;

 public:
  AstNodeType ast_node_type_;
  bool full_;
  int show_type_;
  string database_name_;
  string like_string_;
};

//}  // namespace sql_parser
//}  // namespace claims
#endif  //  SQL_PARSER_AST_NODE_AST_SHOW_STMT_H_
