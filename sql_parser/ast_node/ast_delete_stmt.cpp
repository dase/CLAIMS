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
 * /CLAIMS/sql_parser/ast_node/tDeleteStmt.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#include "../ast_node/ast_delete_stmt.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <bitset>
using std::cout;
using std::endl;
using std::string;
using std::setw;
using std::bitset;

// namespace claims {
// namespace ast_node {

AstDeleteStmt::AstDeleteStmt(AstNodeType ast_node_type, AstNode* from_list,
                             AstNode* where_list, int options)
    : AstNode(ast_node_type), from_list_(from_list), where_list_(where_list) {}

AstDeleteStmt::~AstDeleteStmt() {
  if (NULL != from_list_) {
    delete from_list_;
    from_list_ = NULL;
  }

  if (NULL != where_list_) {
    delete where_list_;
    where_list_ = NULL;
  }
}

void AstDeleteStmt::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|Delete Stmt|" << endl;

  if (NULL != from_list_) {
    from_list_->Print(level + 1);
  }

  if (NULL != where_list_) {
    where_list_->Print(level + 1);
  }
}

// } /* namespace ast_node */
// } /* namespace claims */
