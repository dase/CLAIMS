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
 * /sql_parser/ast_node/ast_select_stmt.cpp
 *  Created on: May 22, 2015 11:32:03 AM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 */

#include "../ast_node/ast_select_stmt.h"

#include <glog/logging.h>
#include <bitset>
#include <iostream>  // NOLINT
#include <iomanip>
#include <string>
#include <vector>
#include <map>

using std::bitset;
using std::endl;
using std::cout;
using std::setw;
using std::string;
using std::multimap;
using std::vector;
AstSelectList::AstSelectList(AstNodeType ast_node_type, bool is_all,
                             AstNode* args, AstNode* next)
    : AstNode(ast_node_type), is_all_(is_all), args_(args), next_(next) {}

AstSelectList::~AstSelectList() {
  delete args_;
  delete next_;
}

void AstSelectList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|select list|" << endl;
  if (args_ != NULL) {
    args_->Print(level + 1);
  } else {
    cout << setw((level + 1) * TAB_SIZE) << " "
         << "|select expr|" << endl;
    cout << setw((level + 1) * TAB_SIZE) << " "
         << "|column| : *(ALL) " << endl;
  }
  if (next_ != NULL) {
    next_->Print(level);
  }
}

AstSelectExpr::AstSelectExpr(AstNodeType ast_node_type, std::string expr_alias,
                             AstNode* expr)
    : AstNode(ast_node_type), expr_alias_(expr_alias), expr_(expr) {}

AstSelectExpr::~AstSelectExpr() { delete expr_; }

void AstSelectExpr::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|select expr|" << endl;
  if (expr_ != NULL) {
    expr_->Print(level);
  }
  cout << setw(level * TAB_SIZE) << " "
       << "expr alias: " << expr_alias_ << endl;
}

AstFromList::AstFromList(AstNodeType ast_node_type, AstNode* args,
                         AstNode* next)
    : AstNode(ast_node_type), args_(args), next_(next), condition_(NULL) {}

AstFromList::~AstFromList() {
  delete args_;
  delete next_;
}

void AstFromList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|from list|" << endl;
  if (args_ != NULL) {
    args_->Print(level + 1);
  }
  if (next_ != NULL) {
    next_->Print(level);
  }
}

AstTable::AstTable(AstNodeType ast_node_type, string db_name, string table_name,
                   string table_alias)
    : AstNode(ast_node_type),
      db_name_(db_name),
      table_name_(table_name),
      table_alias_(table_alias),
      table_id_(0) {}

AstTable::~AstTable() {}

void AstTable::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|table| " << endl;
  level++;
  cout << setw(level * TAB_SIZE) << " "
       << "db_name: " << db_name_ << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "table_name: " << table_name_ << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "table_alias: " << table_alias_ << endl;
}

AstSubquery::AstSubquery(AstNodeType ast_node_type, std::string subquery_alias,
                         AstNode* subquery)
    : AstNode(ast_node_type),
      subquery_alias_(subquery_alias),
      subquery_(subquery) {}

AstSubquery::~AstSubquery() { delete subquery_; }

void AstSubquery::Print(int level) const {
  cout << setw(level++ * TAB_SIZE) << " "
       << "|subquery| "
       << " subquery_alias: " << subquery_alias_ << endl;
  if (subquery_ != NULL) {
    subquery_->Print(level);
  }
}

AstJoinCondition::AstJoinCondition(AstNodeType ast_node_type,
                                   string join_condition_type,
                                   AstNode* condition)
    : AstNode(ast_node_type),
      join_condition_type_(join_condition_type),
      condition_(condition) {}

AstJoinCondition::~AstJoinCondition() { delete condition_; }
void AstJoinCondition::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|join condition| " << join_condition_type_ << endl;
  if (condition_ != NULL) {
    condition_->Print(level + 1);
  } else {
    cout << setw(level * TAB_SIZE) << " "
         << "null" << endl;
  }
}

AstJoin::AstJoin(AstNodeType ast_node_type, int join_type, AstNode* left_table,
                 AstNode* right_table, AstNode* join_condition)
    : AstNode(ast_node_type),
      left_table_(left_table),
      right_table_(right_table),
      join_condition_(reinterpret_cast<AstJoinCondition*>(join_condition)) {
  bitset<10> bit_num;
  bit_num = join_type;
  join_type_ = "";
  if (bit_num[9] == 1) {
    join_type_ = "straight ";
  } else {
    if (bit_num[0] == 1) {
      join_type_ = join_type_ + "inner ";
    }
    if (bit_num[1] == 1) {
      join_type_ = join_type_ + "cross ";
    }
    if (bit_num[2] == 1) {
      join_type_ = join_type_ + "outer ";
    }
    if (bit_num[3] == 1) {
      join_type_ = join_type_ + "left ";
    }
    if (bit_num[4] == 1) {
      join_type_ = join_type_ + "right ";
    }
    if (bit_num[5] == 1) {
      join_type_ = join_type_ + "natural ";
    }
  }
  join_type_ = join_type_ + "join";
}

AstJoin::~AstJoin() {
  delete left_table_;
  delete right_table_;
  delete join_condition_;
}

void AstJoin::Print(int level) const {
  cout << setw(level++ * TAB_SIZE) << " "
       << "|join|" << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "|join type|: " << join_type_ << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "|join left table| " << endl;
  if (left_table_ != NULL) left_table_->Print(level + 1);
  cout << setw(level * TAB_SIZE) << " "
       << "|join right table| " << endl;
  if (right_table_ != NULL) right_table_->Print(level + 1);
  //    cout << setw(level * TAB_SIZE) << " " << "|join condition| " << endl;
  if (join_condition_ != NULL) join_condition_->Print(level + 1);
}

AstWhereClause::AstWhereClause(AstNodeType ast_node_type, AstNode* expr)
    : AstNode(ast_node_type), expr_(expr) {}

AstWhereClause::~AstWhereClause() { delete expr_; }

void AstWhereClause::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|where clause| " << endl;
  if (expr_ != NULL) expr_->Print(level + 1);
}

AstGroupByList::AstGroupByList(AstNodeType ast_node_type, AstNode* expr,
                               AstNode* next)
    : AstNode(ast_node_type), expr_(expr), next_(next) {}

AstGroupByList::~AstGroupByList() {
  delete expr_;
  delete next_;
}

void AstGroupByList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|groupby list| " << endl;
  if (expr_ != NULL) expr_->Print(level + 1);
  if (next_ != NULL) {
    next_->Print(level);
  }
}

AstGroupByClause::AstGroupByClause(AstNodeType ast_node_type,
                                   AstNode* groupby_list, bool with_roolup)
    : AstNode(ast_node_type),
      groupby_list_(reinterpret_cast<AstGroupByList*>(groupby_list)),
      with_roolup_(with_roolup) {}

AstGroupByClause::~AstGroupByClause() { delete groupby_list_; }

void AstGroupByClause::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|groupby clause| "
       << "with rool_up: " << with_roolup_ << endl;
  groupby_list_->Print(level + 1);
}

AstOrderByList::AstOrderByList(AstNodeType ast_node_type, AstNode* expr,
                               int orderby_type, AstNode* next)
    : AstNode(ast_node_type),
      expr_(expr),
      orderby_type_(orderby_type == 0 ? "ASC" : "DESC"),
      next_(next) {}

AstOrderByList::~AstOrderByList() {
  delete expr_;
  delete next_;
}

void AstOrderByList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|orderby list| " << endl;
  if (expr_ != NULL) expr_->Print(level + 1);
  cout << setw((level + 1) * TAB_SIZE) << " "
       << "orderby type: " << orderby_type_ << endl;
  if (next_ != NULL) {
    next_->Print(level);
  }
}

AstOrderByClause::AstOrderByClause(AstNodeType ast_node_type,
                                   AstNode* orderby_list)
    : AstNode(ast_node_type),
      orderby_list_(reinterpret_cast<AstOrderByList*>(orderby_list)) {}

AstOrderByClause::~AstOrderByClause() { delete orderby_list_; }

void AstOrderByClause::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|orderby clause| " << endl;
  if (orderby_list_ != NULL) {
    orderby_list_->Print(level + 1);
  }
}

AstHavingClause::AstHavingClause(AstNodeType ast_node_type, AstNode* expr)
    : AstNode(ast_node_type), expr_(expr) {}

AstHavingClause::~AstHavingClause() { delete expr_; }

void AstHavingClause::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|having clause| " << endl;
  if (expr_ != NULL) expr_->Print(level + 1);
}

AstLimitClause::AstLimitClause(AstNodeType ast_node_type, AstNode* offset,
                               AstNode* row_count)
    : AstNode(ast_node_type), offset_(offset), row_count_(row_count) {}

AstLimitClause::~AstLimitClause() {
  delete offset_;
  delete row_count_;
}

void AstLimitClause::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|limit clause| " << endl;
  level++;
  cout << setw(level * TAB_SIZE) << " "
       << " offset: " << endl;
  if (offset_ != NULL) offset_->Print(level + 1);
  cout << setw(level * TAB_SIZE) << " "
       << " row_count " << endl;
  if (row_count_ != NULL) row_count_->Print(level + 1);
}

AstColumn::AstColumn(AstNodeType ast_node_type, std::string relation_name,
                     std::string column_name)
    : AstNode(ast_node_type),
      relation_name_(relation_name),
      column_name_(column_name),
      next_(NULL) {}

AstColumn::AstColumn(AstNodeType ast_node_type, std::string relation_name,
                     std::string column_name, AstNode* next)
    : AstNode(AST_COLUMN),
      relation_name_(relation_name),
      column_name_(column_name),
      next_(next) {}

AstColumn::~AstColumn() { delete next_; }

void AstColumn::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|column| " << endl;
  cout << setw((level + 1) * TAB_SIZE) << " "
       << "relation name: " << relation_name_ << endl;
  cout << setw((level + 1) * TAB_SIZE) << " "
       << "column name: " << column_name_ << endl;

  if (next_ != NULL) {
    next_->Print(level);
  }
  level++;
}

AstSelectStmt::AstSelectStmt(AstNodeType ast_node_type, int select_opts,
                             AstNode* select_list, AstNode* from_list,
                             AstNode* where_clause, AstNode* groupby_clause,
                             AstNode* having_clause, AstNode* orderby_clause,
                             AstNode* limit_clause, AstNode* select_into_clause)
    : AstNode(ast_node_type),
      select_opts_((SelectOpts)select_opts),
      select_list_(select_list),
      from_list_(from_list),
      where_clause_(where_clause),
      groupby_clause_(groupby_clause),
      having_clause_(having_clause),
      orderby_clause_(orderby_clause),
      limit_clause_(limit_clause),
      select_into_clause_(select_into_clause) {}

AstSelectStmt::~AstSelectStmt() {
  delete select_list_;
  delete from_list_;
  delete where_clause_;
  delete groupby_clause_;
  delete having_clause_;
  delete orderby_clause_;
  delete limit_clause_;
  delete select_into_clause_;
}

void AstSelectStmt::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|select statement| " << endl;
  level++;
  select_list_->Print(level);
  if (from_list_ != NULL) from_list_->Print(level);
  if (where_clause_ != NULL) where_clause_->Print(level);
  if (groupby_clause_ != NULL) groupby_clause_->Print(level);
  if (orderby_clause_ != NULL) orderby_clause_->Print(level);
  if (having_clause_ != NULL) having_clause_->Print(level);
  if (limit_clause_ != NULL) limit_clause_->Print(level);
  if (select_into_clause_ != NULL) select_into_clause_->Print(level);
  cout << "------------select ast print over!------------------" << endl;
}
