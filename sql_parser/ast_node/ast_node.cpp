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
 * ./sql_parser/ast_node.cpp
 *  Created on: May 21, 2015 4:10:35 PM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 */

#include "../ast_node/ast_node.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::setw;
using std::endl;

AstNode::AstNode(AstNodeType ast_node_type) : ast_node_type_(ast_node_type) {}

AstNode::~AstNode() {}
void AstNode::Print(int level) const {
  cout << setw(level * 8) << " "
       << "This is an AST_NODE!" << endl;
}

AstNodeType AstNode::ast_node_type() { return ast_node_type_; }

AstStmtList::AstStmtList(AstNodeType ast_node_type, AstNode* stmt,
                         AstNode* next)
    : AstNode(ast_node_type), stmt_(stmt), next_(next) {}

AstStmtList::~AstStmtList() {
  delete stmt_;
  delete next_;
}

void AstStmtList::Print(int level) const {
  cout << setw(level * 8) << " "
       << "|stmt list|" << endl;
  if (stmt_ != NULL) {
    stmt_->Print(level);
  }
  if (next_ != NULL) {
    next_->Print(level++);
  }
}
