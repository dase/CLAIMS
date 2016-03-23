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
 * /CLAIMS/sql_parser/ast_node/ast_desc_stmt.cpp
 *
 *  Created on: Feb 26, 2016
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>
#include <vector>
#include "../ast_node/ast_desc_stmt.h"
#include "../../common/error_define.h"
#include "./ast_select_stmt.h"
#include "../../catalog/catalog.h"
#include "../../catalog/table.h"
#include "../../Environment.h"
#include "../ast_node/ast_node.h"
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::setw;
// namespace claims {
// namespace sql_parser {

AstDescStmt::AstDescStmt(AstNodeType ast_node_type, string table_name)
    : AstNode(ast_node_type), table_name_(table_name) {}

AstDescStmt::~AstDescStmt() {}

void AstDescStmt::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|Desc Stmt|" << endl;
  cout << setw((level + 1) * TAB_SIZE) << " "
       << "table name: " << table_name_ << endl;
}
RetCode AstDescStmt::SemanticAnalisys(SemanticContext* sem_cntx) {
  int ret = rSuccess;
  cout << "SA table name :" << table_name_ << endl;
  if ((table_name_.empty())) {
    ret = claims::common::rTableNotExisted;
    LOG(ERROR) << "No table name!" << std::endl;
    sem_cntx->error_msg_ = "No table name!";
    return ret;
  }
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  TableDescriptor* table = local_catalog->getTable(table_name_);
  if (table == NULL) {
    ret = claims::common::rTableillegal;
    LOG(ERROR) << "The table " + table_name_ + " does not exist!" << std::endl;
    sem_cntx->error_msg_ = "The table " + table_name_ + " does not exist!";
    return ret;
  }
  return ret;
}

// } /* namespace sql_parser */
// } /* namespace claims */
