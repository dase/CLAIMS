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
 * /CLAIMS/sql_parser/ast_node/ast_show_table.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: fish
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#include <iostream>
#include <iomanip>
#include <string>
#include "../ast_node/ast_show_stmt.h"
#include "../../common/error_define.h"
using namespace claims::common;  // NOLINT
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::setw;

// namespace claims {
// namespace sql_parser {
AstShowStmt::AstShowStmt(AstNodeType ast_node_type, int show_type, bool full,
                         string database_name, string like_string)
    : AstNode(ast_node_type),
      show_type_(show_type),
      full_(full),
      database_name_(database_name),
      like_string_(like_string) {}
AstShowStmt::~AstShowStmt() {}
void AstShowStmt::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|Show Table|" << endl;
  cout << setw((level + 1) * TAB_SIZE) << " "
       << "database name: " << database_name_ << endl;
  cout << setw((level + 1) * TAB_SIZE) << " "
       << "like string: " << like_string_ << endl;
}
//}  // namespace sql_parser
//}  // namespace claims
