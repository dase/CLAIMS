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
 * /CLAIMS/sql_parser/ast_node/ast_desc_stmt.h
 *
 *  Created on: Feb 26, 2016
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#ifndef SQL_PARSER_AST_NODE_AST_DESC_STMT_H_
#define SQL_PARSER_AST_NODE_AST_DESC_STMT_H_

#include <string>
#include <vector>
#include "./ast_node.h"
using std::string;
using std::vector;

// namespace claims {
// namespace sql_parser {

/**
 * @brief the AST of description clause.
 * Desc mainly provides column name, column type, default value,
 * key or not, can be null or not and extra information.
 */
class AstDescStmt : public AstNode {
 public:
  /**
   * @brief AstDescTable Constructor.
   */
  AstDescStmt(AstNodeType ast_node_type, string table_name);
  /**
   * @brief AstDescTable Destructor.
   */
  ~AstDescStmt();
  RetCode SemanticAnalisys(SemanticContext *sem_cntx);

  void Print(int level = 0) const;

 public:
  AstNodeType ast_node_type_;
  string table_name_;
  vector<string> column_name_;
  vector<string> type_;
  vector<string> is_key_;
  vector<string> nullable_;
  vector<string> default_value_;
  vector<string> extra_;
  vector<unsigned> size_;
};

// } // namespace sql_parser
// } // namespace claims

#endif  //  SQL_PARSER_AST_NODE_AST_DESC_STMT_H_
