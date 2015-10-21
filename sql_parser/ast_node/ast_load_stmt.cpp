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
 *  /sql_parser/src/astnode/ast_load_stmt.cpp
 *
 *  Created on: Aug 4, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSe
 *
 */

#include "../ast_node/ast_load_stmt.h"
#include <iostream>  //  NOLINT
#include <iomanip>
#include <string>
#include <bitset>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::setw;
using std::bitset;

AstLoadTable::AstLoadTable(AstNodeType ast_node_type, string table_name,
                           AstNode* path, string column_separator,
                           string tuple_separator, double sample, int mode)
    : AstNode(ast_node_type),
      table_name_(table_name),
      path_(path),
      column_separator_(column_separator),
      tuple_separator_(tuple_separator),
      sample_(sample),
      mode_(mode) {}

AstLoadTable::~AstLoadTable() { delete path_; }

void AstLoadTable::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|Load Table|"
       << "    ";
  cout << " Table name: " << table_name_ << endl;
  cout << setw(level * TAB_SIZE) << " "
       << " Path: " << endl;
  if (path_ != NULL) {
    path_->Print(++level);
  }
}
