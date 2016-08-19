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
 * /CLAIMS/sql_parser/ast_node/ast_update_stmt.cpp
 *
 *  Created on: Aug 16, 2016
 *      Author: cswang
 *		   Email: cs_wang@infosys.com
 *
 * Description:
 *
 */

#include "../ast_node/ast_update_stmt.h"

#include <glog/logging.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <bitset>

#include "./ast_select_stmt.h"
#include "../ast_node/ast_expr_node.h"
#include "../../common/error_define.h"
#include "../../catalog/table.h"
#include "../../Environment.h"
using namespace claims::common;  // NOLINT
using std::cout;
using std::endl;
using std::string;
using std::setw;
using std::bitset;

// namespace claims {
// namespace ast_node {

AstUpdateStmt::AstUpdateStmt(AstNodeType ast_node_type,
                             AstNode* update_set_list, AstNode* update_table,
                             AstNode* where_list)
    : AstNode(ast_node_type),
      update_set_list_(update_set_list),
      update_table_(update_table),
      where_list_(where_list) {}

AstUpdateStmt::~AstUpdateStmt() {
  if (NULL != update_set_list_) {
    delete update_set_list_;
    update_set_list_ = NULL;
  }

  if (NULL != where_list_) {
    delete where_list_;
    where_list_ = NULL;
  }
}

void AstUpdateStmt::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|Update Stmt|" << endl;

  AstUpdateSetList* update_set_list_temp = update_set_list_;
  while (NULL != update_set_list_temp) {
    update_set_list_temp->Print(level + 1);
    update_set_list_temp = update_set_list_temp->next_;
  }

  if (NULL != where_list_) {
    where_list_->Print(level + 1);
  }
}

RetCode AstUpdateStmt::SemanticAnalisys(SemanticContext* sem_cnxt) {
  RetCode ret = rSuccess;
  string tablename = dynamic_cast<AstTable*>(update_table_)->table_name_;
  cout << "AstUpdateStmt:[" << tablename << "]" << endl;
  TableDescriptor* new_table =
      Environment::getInstance()->getCatalog()->getTable(tablename);
  if (NULL == new_table) {
    LOG(ERROR) << "The table " + tablename + " is not existed.";
    sem_cnxt->error_msg_ = "The table " + tablename + " is not existed.";
    ret = rTableNotExisted;
    return ret;
  }
  string tabledel = tablename + "_DEL";
  new_table = Environment::getInstance()->getCatalog()->getTable(tabledel);
  if (NULL == new_table) {
    LOG(ERROR) << "The table DEL " + tabledel +
                      " is not existed during update data." << std::endl;
    sem_cnxt->error_msg_ =
        "The table DEL " + tabledel + " is not existed during update data.";
    ret = rTableNotExisted;
    return ret;
  }

  AstUpdateSetList* update_set_list_temp = update_set_list_;
  while (NULL != update_set_list_temp) {
    ret = update_set_list_temp->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      string column_name_tmp = reinterpret_cast<AstColumn*>(
                                   update_set_list_temp->args0_)->column_name_;
      LOG(ERROR) << "The column " + column_name_tmp +
                        " is not existed during update data." << std::endl;
      sem_cnxt->error_msg_ = "The column " + column_name_tmp +
                             " is not existed during update data.";
      return ret;
    }
    update_set_list_temp = update_set_list_temp->next_;
  }

  return ret;
}

AstUpdateSetList::AstUpdateSetList(AstNodeType ast_node_type, AstNode* args0,
                                   AstNode* args1, AstNode* next)
    : AstNode(ast_node_type), args0_(args0), args1_(args1), next_(next) {}

AstUpdateSetList::~AstUpdateSetList() {
  if (NULL != args0_) {
    delete args0_;
    args0_ = NULL;
  }

  if (NULL != args1_) {
    delete args1_;
    args1_ = NULL;
  }

  if (NULL != next_) {
    delete next_;
    next_ = NULL;
  }
}

void AstUpdateSetList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|UpdateSet List| " << endl;
  if (args0_ != NULL) args0_->Print(level + 1);
  if (args1_ != NULL) args1_->Print(level + 1);
}

RetCode AstUpdateSetList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  RetCode ret = rSuccess;
  //  cout << "args0_->ast_node_type():" << args0_->ast_node_type() << endl;
  //  cout << "args1_->ast_node_type():" << args1_->ast_node_type() << endl;
  if (args0_->ast_node_type() == AST_COLUMN) {
    AstColumn* column = reinterpret_cast<AstColumn*>(args0_);
    cout << "AstUpdateSetList args0: [" << column->relation_name_ << "."
         << column->column_name_ << "]" << endl;

  } else {
    printf("AstUpdateSetList args0 error ast_node_type\n");
  }
#if 1
  if (args1_->ast_node_type() == AST_EXPR_CONST) {
    AstExprConst* expr_const = reinterpret_cast<AstExprConst*>(args1_);
    cout << "AstUpdateSetList args1: [" << expr_const->expr_type_ << "."
         << expr_const->data_ << "]" << endl;
  } else {
    printf("AstUpdateSetList args1 error ast_node_type\n");
  }
#endif
  return ret;
}

RetCode AstUpdateSetList::PushDownCondition(PushDownConditionContext& pdccnxt) {
}

RetCode AstUpdateSetList::GetLogicalPlan(LogicalOperator*& logic_plan) {}

// } /* namespace ast_node */
// } /* namespace claims */
