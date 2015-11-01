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
 *  /sql_parser/src/astnode/ast_load_stmt.h
 *
 *  Created on: Aug 4, 2015
 *      Author: yuyang
 *       Email: youngfish93@hotmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSe
 *
 */

#ifndef SQL_PARSER_AST_NODE_AST_LOAD_STMT_H_
#define SQL_PARSER_AST_NODE_AST_LOAD_STMT_H_

#include <string>
#include <iostream>  //NOLINT

#include "../ast_node/ast_node.h"

using std::string;
// namespace claims {
// namespace sql_parser {
/***
 * @brief The AST of load table statement.
 * @details AstLoadTable mainly includes table names, file path, column
 * separator, tuple separator, samples and modes.
 */
class AstLoadTable : public AstNode {
 public:
  AstLoadTable(AstNodeType ast_node_type, string table_name, AstNode* path,
               string column_separator, string tuple_separator, double sample,
               int mode);
  ~AstLoadTable();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string table_name_;
  AstNode* path_;
  string column_separator_;
  string tuple_separator_;
  double sample_;
  int mode_;

 private:
};
//}  // namespace sql_parser
//}  // namespace claims

#endif  // SQL_PARSER_AST_NODE_AST_LOAD_STMT_H_
