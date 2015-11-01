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
 *  /sql_parser/src/astnode/ast_insert_stmt.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSe
 *
 */

#include "../ast_node/ast_insert_stmt.h"
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
// namespace claims {
// namespace sql_parser {

AstInsertStmt::AstInsertStmt(AstNodeType ast_node_type, int insert_opt,
                             string table_name, AstNode* col_list,
                             AstNode* insert_val_list,
                             AstNode* insert_assign_list,
                             AstNode* insert_assign_list_from_set,
                             AstNode* select_stmt)
    : AstNode(ast_node_type),
      insert_opt_(insert_opt),
      table_name_(table_name),
      col_list_(col_list),
      insert_val_list_(insert_val_list),
      select_stmt_(select_stmt),
      insert_assign_list_(insert_assign_list),
      insert_assign_list_from_set_(insert_assign_list_from_set) {}

AstInsertStmt::~AstInsertStmt() {
  delete col_list_;
  delete insert_val_list_;
  delete select_stmt_;
  delete insert_assign_list_;
  delete insert_assign_list_from_set_;
}

void AstInsertStmt::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|Insert Statement|"
       << "   Table Name:" << table_name_ << endl;
  if (col_list_ != NULL) {
    col_list_->Print(level + 1);
  }
}

AstInsertValList::AstInsertValList(AstNodeType ast_node_type,
                                   AstNode* insert_vals, AstNode* next)
    : AstNode(ast_node_type), insert_vals_(insert_vals), next_(next) {}

AstInsertValList::~AstInsertValList() {
  delete insert_vals_;
  delete next_;
}

void AstInsertValList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|Insert ValList|" << endl;
}

AstInsertVals::AstInsertVals(AstNodeType ast_node_type, int value_type,
                             AstNode* expr, AstNode* next)
    : AstNode(ast_node_type),
      value_type_(value_type),
      expr_(expr),
      next_(next) {}

AstInsertVals::~AstInsertVals() {
  delete expr_;
  delete next_;
}

void AstInsertVals::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|Insert Vals|" << endl;
}

AstInsertAssignList::AstInsertAssignList(AstNodeType ast_node_type,
                                         string col_name, int value_type,
                                         AstNode* expr, AstNode* next)
    : AstNode(ast_node_type),
      col_name_(col_name),
      value_type_(value_type),
      expr_(expr),
      next_(next) {}

AstInsertAssignList::~AstInsertAssignList() {
  delete expr_;
  delete next_;
}

void AstInsertAssignList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|Insert Assign List|" << endl;
  if (expr_ != NULL) {
    expr_->Print(level + 1);
  }
  if (next_ != NULL) {
    next_->Print(level);
  }
}
//}  // namespace sql_parser
//}  // namespace claims
