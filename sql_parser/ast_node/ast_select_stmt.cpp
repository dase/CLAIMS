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

#include "./ast_select_stmt.h"

#include <glog/logging.h>
#include <algorithm>
#include <bitset>
#include <iostream>  // NOLINT
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>

#include "../../Environment.h"
#include "../../Catalog/Attribute.h"
#include "../../Catalog/table.h"
#include "../../logical_operator/logical_equal_join.h"
#include "../../logical_operator/logical_cross_join.h"
#include "../../logical_operator/logical_filter.h"
#include "../../logical_operator/logical_scan.h"

#include "../ast_node/ast_node.h"

using claims::logical_operator::LogicalCrossJoin;
using claims::logical_operator::LogicalEqualJoin;
using claims::logical_operator::LogicalFilter;
using claims::logical_operator::LogicalScan;
using std::bitset;
using std::endl;
using std::cout;
using std::setw;
using std::string;
using std::map;
using std::endl;
using std::multimap;
using std::sort;
using std::vector;
// namespace claims {
// namespace sql_parser {
AstSelectList::AstSelectList(AstNodeType ast_node_type, bool is_all,
                             AstNode* args, AstNode* next)
    : AstNode(ast_node_type), is_all_(is_all), args_(args), next_(next) {
  if (is_all) {
    args_ = new AstColumn(AST_COLUMN_ALL_ALL, "*", "*", "*.*");
  }
}

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
ErrorNo AstSelectList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  sem_cnxt->clause_type_ = SemanticContext::kSelectClause;
  ErrorNo ret = eOK;
  if (NULL != args_) {
    ret = args_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (is_all_) {
    ret = sem_cnxt->AddSelectAttrs(args_);  // collect select expr node
    return eOK;
  }
  if (NULL != next_) {
    ret = next_->SemanticAnalisys(sem_cnxt);
    return ret;
  }
  sem_cnxt->clause_type_ = SemanticContext::kNone;
  return eOK;
}
void AstSelectList::RecoverExprName(string& name) {
  if (NULL != args_) {
    args_->RecoverExprName(name);
  }
  if (NULL != next_) {
    next_->RecoverExprName(name);
  }
  return;
}
void AstSelectList::ReplaceAggregation(AstNode*& agg_column,
                                       set<AstNode*>& agg_node,
                                       bool is_select) {
  if (NULL != args_) {
    args_->ReplaceAggregation(agg_column, agg_node, true);
  }
  if (NULL != next_) {
    next_->ReplaceAggregation(agg_column, agg_node, true);
  }
  return;
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
// there is no need to eliminate alias conflict in top select, but in sub query,
// the alias conflict will be checked by Ast.
ErrorNo AstSelectExpr::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != expr_) {
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
    expr_->expr_str_ = expr_alias_;
    ret = sem_cnxt->AddSelectAttrs(expr_);  // collect select expr node
    if (eOK != ret) {
      return ret;
    }
  }
  return eOK;
}
void AstSelectExpr::RecoverExprName(string& name) {
  string expr_name = "";
  if (NULL != expr_) {
    expr_->RecoverExprName(expr_name);
  }
  if (expr_alias_ == "NULL") {
    expr_alias_ = expr_name;
  }
  return;
}
void AstSelectExpr::ReplaceAggregation(AstNode*& agg_column,
                                       set<AstNode*>& agg_node,
                                       bool is_select) {
  if (NULL != expr_) {
    agg_column = NULL;
    expr_->ReplaceAggregation(agg_column, agg_node, true);
    if (NULL != agg_column) {
      expr_ = agg_column;
    }
    agg_column = NULL;
  }
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
  ++level;
  cout << setw(level * TAB_SIZE) << " "
       << "|equal join condition: |" << endl;
  for (auto it = equal_join_condition_.begin();
       it != equal_join_condition_.end(); ++it) {
    (*it)->Print(level + 1);
    cout << endl;
  }
  cout << setw(level * TAB_SIZE) << " "
       << "|normal condition: |" << endl;
  for (auto it = normal_condition_.begin(); it != normal_condition_.end();
       ++it) {
    (*it)->Print(level + 1);
  }
  cout << endl;

  if (args_ != NULL) {
    args_->Print(level + 1);
  }
  if (next_ != NULL) {
    --level;
    next_->Print(level);
  }
}
ErrorNo AstFromList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  sem_cnxt->clause_type_ = SemanticContext::kFromClause;
  ErrorNo ret = eOK;
  if (NULL != args_) {
    ret = args_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != next_) {
    ret = next_->SemanticAnalisys(sem_cnxt);
    return ret;
  }
  sem_cnxt->clause_type_ = SemanticContext::kNone;
  return eOK;
}
void AstFromList::GetJoinedRoot(map<string, AstNode*> table_joined_root,
                                AstNode* joined_root) {
  if (NULL != next_) {
    next_->GetJoinedRoot(table_joined_root, NULL);
  }
  if (NULL != args_) {
    args_->GetJoinedRoot(table_joined_root, NULL);
  }
}
ErrorNo AstFromList::PushDownCondition(PushDownConditionContext* pdccnxt) {
  PushDownConditionContext* cur_pdccnxt = new PushDownConditionContext();
  cur_pdccnxt->sub_expr_info_ = pdccnxt->sub_expr_info_;

  if (NULL != args_) {
    cur_pdccnxt->from_tables_.clear();
    args_->PushDownCondition(cur_pdccnxt);
    pdccnxt->from_tables_.insert(cur_pdccnxt->from_tables_.begin(),
                                 cur_pdccnxt->from_tables_.end());
  }
  if (NULL != next_) {
    cur_pdccnxt->from_tables_.clear();
    next_->PushDownCondition(cur_pdccnxt);
    pdccnxt->from_tables_.insert(cur_pdccnxt->from_tables_.begin(),
                                 cur_pdccnxt->from_tables_.end());
  }
  pdccnxt->SetCondition(equal_join_condition_, normal_condition_);
  return eOK;
}
ErrorNo AstFromList::GetLogicalPlan(LogicalOperator*& logic_plan) {
  LogicalOperator* args_lplan = NULL;
  LogicalOperator* next_lplan = NULL;
  ErrorNo ret = eOK;
  if (NULL != args_) {
    args_->GetLogicalPlan(args_lplan);
  }
  if (NULL != next_) {
    next_->GetLogicalPlan(next_lplan);
  }
  if (NULL != next_lplan) {
    if (equal_join_condition_.size() > 0) {
      vector<LogicalEqualJoin::JoinPair> join_pair;
      join_pair.clear();
      // TODO(FZH)
      for (auto it = equal_join_condition_.begin();
           it != equal_join_condition_.end(); ++it) {
        //        join_pair.push_back(new LogicalEqualJoin::JoinPair());
      }
      logic_plan = new LogicalEqualJoin(join_pair, args_lplan, next_lplan);

    } else {
      logic_plan = new LogicalCrossJoin(args_lplan, next_lplan);
    }
    if (normal_condition_.size() > 0) {
      vector<QNode*> condition;
      condition.clear();
      QNode* qnode = NULL;
      for (auto it = normal_condition_.begin(); it != normal_condition_.end();
           ++it) {
        ret = (*it)->GetLogicalPlan(qnode, logic_plan);
        if (eOK != ret) {
          LOG(ERROR) << "get normal condition upon from list, due to [err: "
                     << ret << " ] !" << endl;
          return ret;
        }
        assert(NULL != qnode);
        condition.push_back(qnode);
      }
    }
  } else {
    logic_plan = args_lplan;
  }
  return eOK;
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
  ++level;
  cout << setw(level * TAB_SIZE) << " "
       << "|equal join condition: |" << endl;
  for (auto it = equal_join_condition_.begin();
       it != equal_join_condition_.end(); ++it) {
    (*it)->Print(level + 1);
    cout << endl;
  }
  cout << setw(level * TAB_SIZE) << " "
       << "|normal condition: |" << endl;
  for (auto it = normal_condition_.begin(); it != normal_condition_.end();
       ++it) {
    (*it)->Print(level + 1);
  }
  cout << endl;
  level++;
  cout << setw(level * TAB_SIZE) << " "
       << "db_name: " << db_name_ << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "table_name: " << table_name_ << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "table_alias: " << table_alias_ << endl;
}
ErrorNo AstTable::SemanticAnalisys(SemanticContext* sem_cnxt) {
  TableDescriptor* tbl =
      Environment::getInstance()->getCatalog()->getTable(table_name_);
  if (NULL == tbl) {
    LOG(ERROR) << "table: " << table_name_ << " dosen't exist!" << endl;
    return eTableNotExist;
  }
  if (table_alias_ == "NULL") {
    table_alias_ = table_name_;
  }
  sem_cnxt->AddTable(table_alias_);

  std::vector<Attribute> attrs = tbl->getAttributes();
  // NOTE: PART.row_id should row_id
  for (auto it = attrs.begin(); it != attrs.end(); ++it) {
    sem_cnxt->AddTableColumn(table_alias_,
                             it->attrName.substr((it->attrName).find('.') + 1));
  }
  LOG(INFO) << table_name_ << " , " << table_alias_ << " semantic successfully"
            << endl;
  return eOK;
}
void AstTable::GetJoinedRoot(map<string, AstNode*> table_joined_root,
                             AstNode* joined_root) {
  assert(table_alias_ != "NULL");
  if (NULL == joined_root) {
    table_joined_root[table_alias_] = this;
  } else {
    table_joined_root[table_alias_] = joined_root;
  }
}
ErrorNo AstTable::PushDownCondition(PushDownConditionContext* pdccnxt) {
  pdccnxt->from_tables_.insert(table_alias_);
  pdccnxt->SetCondition(equal_join_condition_, normal_condition_);
  return eOK;
}
// TODO(FZH) diver table_name_ to LogicalScan
ErrorNo AstTable::GetLogicalPlan(LogicalOperator*& logic_plan) {
  ErrorNo ret = eOK;
  logic_plan = new LogicalScan(Environment::getInstance()
                                   ->getCatalog()
                                   ->getTable(table_name_)
                                   ->getProjectoin(0));
  if (equal_join_condition_.size() > 0) {
    LOG(ERROR) << "equal join condition shouldn't occur in a single table!"
               << endl;
    assert(false);
    return eEqualJoinCondiInATable;
  }
  if (normal_condition_.size() > 0) {
    vector<QNode*> condition;
    QNode* qnode = NULL;
    for (auto it = normal_condition_.begin(); it != normal_condition_.end();
         ++it) {
      ret = (*it)->GetLogicalPlan(qnode, logic_plan);
      if (eOK != ret) {
        LOG(ERROR) << "get normal condition upon a table, due to [err: " << ret
                   << " ] !" << endl;
        return ret;
      }
      assert(NULL != qnode);
      condition.push_back(qnode);
    }
    logic_plan = new LogicalFilter(logic_plan, condition);
  }
  return eOK;
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
  ++level;
  cout << setw(level * TAB_SIZE) << " "
       << "|equal join condition: |" << endl;
  for (auto it = equal_join_condition_.begin();
       it != equal_join_condition_.end(); ++it) {
    (*it)->Print(level + 1);
    cout << endl;
  }
  cout << setw(level * TAB_SIZE) << " "
       << "|normal condition: |" << endl;
  for (auto it = normal_condition_.begin(); it != normal_condition_.end();
       ++it) {
    (*it)->Print(level + 1);
  }
  cout << endl;
  if (subquery_ != NULL) {
    subquery_->Print(level);
  }
}
ErrorNo AstSubquery::SemanticAnalisys(SemanticContext* sem_cnxt) {
  SemanticContext sub_sem_cnxt;
  //  // subquery_alias_ == existed_table?
  //  if (NULL !=
  //      Environment::getInstance()->getCatalog()->getTable(subquery_alias_)) {
  //    LOG(ERROR) << "subquery's alias couldn't equal to table that's exist in
  //    DB"
  //               << endl;
  //    return eTableAliasEqualExistedTable;
  //  }

  // subquery_ is OK?
  ErrorNo ret = subquery_->SemanticAnalisys(&sub_sem_cnxt);
  if (eOK != ret) {
    return ret;
  }
  //  // subquery_alias in tables that occur in subquery?
  //  ret = sub_sem_cnxt.IsTableExist(subquery_alias_);
  //  if (eOK == ret) {
  //    return eTableAliasEqualLowerAlias;
  //  }

  // add this sub query alias into upper query's table list
  ret = sem_cnxt->AddTable(subquery_alias_);
  if (eOK != ret) {
    return ret;
  }

  multimap<string, string> column_to_table;
  ret = sub_sem_cnxt.GetAliasColumn(subquery_alias_, column_to_table);
  if (eOK != ret) {
    return ret;
  }

  return sem_cnxt->AddTableColumn(column_to_table);
}

void AstSubquery::GetJoinedRoot(map<string, AstNode*> table_joined_root,
                                AstNode* joined_root) {
  assert(subquery_alias_ != "NULL");
  if (NULL == joined_root) {
    table_joined_root[subquery_alias_] = this;
  } else {
    table_joined_root[subquery_alias_] = joined_root;
  }
}
ErrorNo AstSubquery::PushDownCondition(PushDownConditionContext* pdccnxt) {
  pdccnxt->from_tables_.insert(subquery_alias_);
  pdccnxt->SetCondition(equal_join_condition_, normal_condition_);
  return eOK;
}
// may be deliver subquery output schema
ErrorNo AstSubquery::GetLogicalPlan(LogicalOperator*& logic_plan) {
  return subquery_->GetLogicalPlan(logic_plan);
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
ErrorNo AstJoinCondition::SemanticAnalisys(SemanticContext* sem_cnxt) {
  if (NULL != condition_) {
    return condition_->SemanticAnalisys(sem_cnxt);
  }
  return eOK;
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
       << "|equal join condition: |" << endl;
  for (auto it = equal_join_condition_.begin();
       it != equal_join_condition_.end(); ++it) {
    (*it)->Print(level + 1);
    cout << endl;
  }
  cout << setw(level * TAB_SIZE) << " "
       << "|normal condition: |" << endl;
  for (auto it = normal_condition_.begin(); it != normal_condition_.end();
       ++it) {
    (*it)->Print(level + 1);
  }
  cout << endl;

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
ErrorNo AstJoin::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  SemanticContext join_sem_cnxt;
  if (NULL == left_table_) {
    LOG(ERROR) << "left table is null in join!" << endl;
    assert(false);
    return eLeftTableIsNULLInJoin;
  }
  if (NULL != right_table_) {
    LOG(ERROR) << "right table is null in join!" << endl;
    assert(false);
    return eRightTableIsNULLInJoin;
  }
  ret = left_table_->SemanticAnalisys(&join_sem_cnxt);
  if (eOK != ret) {
    return ret;
  }

  ret = right_table_->SemanticAnalisys(&join_sem_cnxt);
  if (eOK != ret) {
    return ret;
  }

  if (NULL != join_condition_) {
    ret = join_condition_->SemanticAnalisys(&join_sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  ret = sem_cnxt->AddTable(join_sem_cnxt.get_tables());
  ret = sem_cnxt->AddTableColumn(join_sem_cnxt.get_column_to_table());
  //  join_sem_cnxt.~SemanticContext();
  return ret;
}
void AstJoin::GetJoinedRoot(map<string, AstNode*> table_joined_root,
                            AstNode* joined_root) {
  left_table_->GetJoinedRoot(table_joined_root, this);
  right_table_->GetJoinedRoot(table_joined_root, this);
}
ErrorNo AstJoin::PushDownCondition(PushDownConditionContext* pdccnxt) {
  PushDownConditionContext* cur_pdccnxt = new PushDownConditionContext();
  cur_pdccnxt->sub_expr_info_ = pdccnxt->sub_expr_info_;
  if (NULL != join_condition_) {
    cur_pdccnxt->GetSubExprInfo(
        reinterpret_cast<AstJoinCondition*>(join_condition_)->condition_);
  }

  cur_pdccnxt->from_tables_.clear();
  PushDownConditionContext* child_pdccnxt = new PushDownConditionContext();
  child_pdccnxt->sub_expr_info_ = cur_pdccnxt->sub_expr_info_;
  child_pdccnxt->from_tables_.clear();
  left_table_->PushDownCondition(child_pdccnxt);
  cur_pdccnxt->from_tables_.insert(child_pdccnxt->from_tables_.begin(),
                                   child_pdccnxt->from_tables_.end());

  child_pdccnxt->from_tables_.clear();
  right_table_->PushDownCondition(child_pdccnxt);
  cur_pdccnxt->from_tables_.insert(child_pdccnxt->from_tables_.begin(),
                                   child_pdccnxt->from_tables_.end());

  cur_pdccnxt->SetCondition(equal_join_condition_, normal_condition_);

  pdccnxt->from_tables_.insert(cur_pdccnxt->from_tables_.begin(),
                               cur_pdccnxt->from_tables_.end());
  return eOK;
}
ErrorNo AstJoin::GetLogicalPlan(LogicalOperator*& logic_plan) {
  LogicalOperator* left_plan = NULL;
  LogicalOperator* right_plan = NULL;
  left_table_->GetLogicalPlan(left_plan);
  right_table_->GetLogicalPlan(right_plan);
  if (equal_join_condition_.size() > 0) {
    vector<LogicalEqualJoin::JoinPair> join_pair;
    join_pair.clear();
    // TODO(FZH)
    for (auto it = equal_join_condition_.begin();
         it != equal_join_condition_.end(); ++it) {
      //        join_pair.push_back(new LogicalEqualJoin::JoinPair());
    }
    logic_plan = new LogicalEqualJoin(join_pair, left_plan, right_plan);

  } else {
    logic_plan = new LogicalCrossJoin(left_plan, right_plan);
  }
  if (normal_condition_.size() > 0) {
    vector<QNode*> condition;
    // TODO(FZH)
    logic_plan = new LogicalFilter(logic_plan, condition);
  }
  return eOK;
}

AstWhereClause::AstWhereClause(AstNodeType ast_node_type, AstNode* expr)
    : AstNode(ast_node_type), expr_(expr) {}

AstWhereClause::~AstWhereClause() { delete expr_; }

void AstWhereClause::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|where clause| " << endl;
  if (expr_ != NULL) expr_->Print(level + 1);
}
ErrorNo AstWhereClause::SemanticAnalisys(SemanticContext* sem_cnxt) {
  if (NULL != expr_) {
    sem_cnxt->clause_type_ = SemanticContext::kWhereClause;
    ErrorNo ret = eOK;
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) return ret;
    sem_cnxt->clause_type_ = SemanticContext::kNone;
  }
  return eOK;
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
       << "|groupby list| " << expr_str_ << endl;
  if (expr_ != NULL) {
    expr_->Print(level + 1);
  }
  if (next_ != NULL) {
    next_->Print(level);
  }
}
ErrorNo AstGroupByList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != expr_) {
    // to limit that don't support expression in group by
    //    // don't support expression in group by list
    //    if (AST_COLUMN != expr_->ast_node_type()) {
    //      LOG(ERROR) << "group by list should be single column!" << endl;
    //      return eGroupByNotSupportColumn;
    //    }
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
    sem_cnxt->AddGroupByAttrs(expr_);  // collect group by expr node
  }
  if (NULL != next_) {
    ret = next_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  return eOK;
}
void AstGroupByList::RecoverExprName(string& name) {
  if (NULL != expr_) {
    expr_->RecoverExprName(name);
  }
  if (NULL != next_) {
    next_->RecoverExprName(name);
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
ErrorNo AstGroupByClause::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != groupby_list_) {
    sem_cnxt->clause_type_ = SemanticContext::kGroupByClause;
    ret = groupby_list_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
    sem_cnxt->clause_type_ = SemanticContext::kNone;
    return eOK;
  }
  return eGroupbyListIsNULL;
}
void AstGroupByClause::RecoverExprName(string& name) {
  if (NULL != groupby_list_) {
    groupby_list_->RecoverExprName(name);
  }
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
ErrorNo AstOrderByList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != expr_) {
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != next_) {
    ret = next_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  return eOK;
}
void AstOrderByList::RecoverExprName(string& name) {
  if (NULL != expr_) {
    expr_->RecoverExprName(name);
  }
  if (NULL != next_) {
    next_->RecoverExprName(name);
  }
}
void AstOrderByList::ReplaceAggregation(AstNode*& agg_column,
                                        set<AstNode*>& agg_node,
                                        bool is_select) {
  if (NULL != expr_) {
    agg_column = NULL;
    expr_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      delete expr_;
      expr_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != next_) {
    next_->ReplaceAggregation(agg_column, agg_node, is_select);
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
ErrorNo AstOrderByClause::SemanticAnalisys(SemanticContext* sem_cnxt) {
  if (NULL != orderby_list_) {
    sem_cnxt->clause_type_ = SemanticContext::kOrderByClause;
    ErrorNo ret = eOK;
    ret = orderby_list_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
    sem_cnxt->clause_type_ = SemanticContext::kNone;
    return eOK;
  }
  return eOK;
}
void AstOrderByClause::RecoverExprName(string& name) {
  if (NULL != orderby_list_) {
    orderby_list_->RecoverExprName(name);
  }
}
void AstOrderByClause::ReplaceAggregation(AstNode*& agg_column,
                                          set<AstNode*>& agg_node,
                                          bool is_select) {
  if (NULL != orderby_list_) {
    orderby_list_->ReplaceAggregation(agg_column, agg_node, is_select);
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
ErrorNo AstHavingClause::SemanticAnalisys(SemanticContext* sem_cnxt) {
  if (NULL != expr_) {
    sem_cnxt->clause_type_ = SemanticContext::kHavingClause;
    ErrorNo ret = eOK;
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
    sem_cnxt->clause_type_ = SemanticContext::kNone;
  }
  return eOK;
}
void AstHavingClause::RecoverExprName(string& name) {
  if (NULL != expr_) {
    expr_->RecoverExprName(name);
  }
}
void AstHavingClause::ReplaceAggregation(AstNode*& agg_column,
                                         set<AstNode*>& agg_node,
                                         bool is_select) {
  if (NULL != expr_) {
    expr_->ReplaceAggregation(agg_column, agg_node, is_select);
  }
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
ErrorNo AstLimitClause::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != offset_) {
    ret = offset_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != row_count_) {
    return row_count_->SemanticAnalisys(sem_cnxt);
  }
  return eOK;
}

AstColumn::AstColumn(AstNodeType ast_node_type, std::string relation_name,
                     std::string column_name)
    : AstNode(ast_node_type),
      relation_name_(relation_name),
      column_name_(column_name),
      next_(NULL) {}
AstColumn::AstColumn(AstNodeType ast_node_type, std::string relation_name,
                     std::string column_name, string expr_str)
    : AstNode(ast_node_type),
      relation_name_(relation_name),
      column_name_(column_name),
      next_(NULL) {
  expr_str_ = expr_str;
}

AstColumn::AstColumn(AstNodeType ast_node_type, std::string relation_name,
                     std::string column_name, AstNode* next)
    : AstNode(AST_COLUMN),
      relation_name_(relation_name),
      column_name_(column_name),
      next_(next) {}

AstColumn::~AstColumn() { delete next_; }

void AstColumn::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|column| " << expr_str_ << endl;
  cout << setw((level + 1) * TAB_SIZE) << " "
       << "relation name: " << relation_name_ << endl;
  cout << setw((level + 1) * TAB_SIZE) << " "
       << "column name: " << column_name_ << endl;

  if (next_ != NULL) {
    next_->Print(level);
  }
  level++;
}
ErrorNo AstColumn::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (AST_COLUMN_ALL_ALL == ast_node_type_) {
    if (SemanticContext::kSelectClause == sem_cnxt->clause_type_) {
      return eOK;
    } else {
      return eColumnAllShouldNotInOtherClause;
    }
  }
  if (AST_COLUMN_ALL == ast_node_type_) {
    if (SemanticContext::kSelectClause == sem_cnxt->clause_type_) {
      ret = sem_cnxt->IsTableExist(relation_name_);
      if (eOK != ret) {
        return eTableNotExistInTableColumnALL;
      }
    } else {
      return eColumnAllShouldNotInOtherClause;
    }
    return eOK;
  }
  ret = sem_cnxt->IsColumnExist(relation_name_, column_name_);
  if (eOK != ret) {
    LOG(ERROR) << "There are errors in ( " << relation_name_ << " , "
               << column_name_ << " )" << endl;
    return ret;
  }
  if (NULL != next_) {
    return next_->SemanticAnalisys(sem_cnxt);
  }
  return eOK;
}
void AstColumn::RecoverExprName(string& name) {
  string next_name = "";
  if (NULL != next_) {
    next_->RecoverExprName(next_name);
  }
  if (relation_name_ != "NULL") {
    expr_str_ = relation_name_ + "." + column_name_;
  } else {
    expr_str_ = column_name_;
  }
  if (NULL != next_) {
    name = expr_str_ + " , " + next_name;
  } else {
    name = expr_str_;
  }
  return;
}

/**
 * because GetRefTable from where clause, so there couldn't be AST_COLUMN_ALL or
 * AST_COLUMN_ALL_ALL
 * relation_name_=="NULL" is illegal, but "NULL_AGG" means for aggregation, so
 * it's legal
 */
void AstColumn::GetRefTable(set<string>& ref_table) {
  assert(AST_COLUMN_ALL != ast_node_type_ &&
         AST_COLUMN_ALL_ALL != ast_node_type_);
  assert(relation_name_ != "NULL");
  ref_table.insert(relation_name_);
}

ErrorNo AstColumn::GetLogicalPlan(QNode*& logic_expr,
                                  LogicalOperator* child_logic_plan) {
  ErrorNo ret = eOK;
  logic_expr = new QColcumns(
      relation_name_.c_str(), column_name_.c_str(),
      child_logic_plan->GetPlanContext()
          .GetAttribute(relation_name_, relation_name_ + "." + column_name_)
          .attrType->type,
      "");
  return eOK;
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
  if (having_clause_ != NULL) having_clause_->Print(level);
  if (orderby_clause_ != NULL) orderby_clause_->Print(level);
  if (limit_clause_ != NULL) limit_clause_->Print(level);
  if (select_into_clause_ != NULL) select_into_clause_->Print(level);
  cout << "------------select ast print over!------------------" << endl;
}
/**
 *  NOTE: the physical execution may be divide into 2 step_
 *  from-> where-> groupby-> select_aggregation->
 *  having->orderby-> limit-> select_expression
 */

ErrorNo AstSelectStmt::SemanticAnalisys(SemanticContext* sem_cnxt) {
  if (NULL == sem_cnxt) {
    SemanticContext sem;
    sem_cnxt = &sem;
  }
  ErrorNo ret = eOK;
  // check whether table exist or table alias conflict
  if (NULL != from_list_) {
    ret = from_list_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      LOG(ERROR) << "from clause has error" << endl;
      return ret;
    }
  } else {
    LOG(WARNING) << "from clause is NULL" << endl;
  }
  // check whether column exist
  // TODO(FZH) check every functions
  if (NULL != where_clause_) {
    ret = where_clause_->SemanticAnalisys(sem_cnxt);

    if (eOK != ret) {
      LOG(ERROR) << "where clause has error" << endl;
      return ret;
    }
  }
  // aggregation couldn't in group by clause
  // collect all group by attributes to rebuild schema
  if (NULL != groupby_clause_) {
    string name = "";
    groupby_clause_->RecoverExprName(name);
    ret = groupby_clause_->SemanticAnalisys(sem_cnxt);

    if (eOK != ret) {
      LOG(ERROR) << "groupby clause has error" << endl;
      return ret;
    }
    groupby_attrs_ = sem_cnxt->get_groupby_attrs();
  }

  sem_cnxt->PrintContext();
  // first recover select attr name
  // collect all aggregation functions
  agg_attrs_.clear();
  if (NULL != select_list_) {
    string name = "";
    select_list_->RecoverExprName(name);

    ret = select_list_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      LOG(ERROR) << "select list has error" << endl;
      return ret;
    }

    AstNode* agg_column = NULL;
    select_list_->ReplaceAggregation(agg_column, agg_attrs_, true);
    if (agg_attrs_.size() > 0) {
      sem_cnxt->RemoveMore(agg_attrs_);
    }
  } else {
    LOG(ERROR) << "select list is NULL" << endl;
    return eSelectClauseIsNULL;
  }
  // rebuild schema result from aggregation
  have_aggeragion_ = (NULL != groupby_clause_ || agg_attrs_.size() > 0);
  if (have_aggeragion_) {
    ErrorNo ret = sem_cnxt->RebuildTableColumn(agg_attrs_);
    if (eOK != ret) {
      LOG(ERROR) << "there are confiction in new schema after agg!" << endl;
      return ret;
    }
    sem_cnxt->PrintContext();

    // check whether other column except from aggregation funcs and groupby
    // expressions in select expressions
    ret = select_list_->SemanticAnalisys(sem_cnxt);

    if (eOK != ret) {
      return eAggSelectExprHaveOtherColumn;
    }
  }
  // having clause exist only if have aggregation
  if (NULL != having_clause_) {
    if (!have_aggeragion_) {
      return eHavingNotAgg;
    }
    string name = "";
    having_clause_->RecoverExprName(name);

    AstNode* agg_column = NULL;
    set<AstNode*> agg_node_having;
    having_clause_->ReplaceAggregation(agg_column, agg_node_having, false);

    ret = having_clause_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      LOG(ERROR) << "having clause has error" << endl;
      return ret;
    }
  }
  if (NULL != orderby_clause_) {
    ErrorNo ret = eOK;
    string name = "";
    orderby_clause_->RecoverExprName(name);

    AstNode* agg_column = NULL;
    set<AstNode*> agg_node_orderby;
    orderby_clause_->ReplaceAggregation(agg_column, agg_node_orderby, false);

    ret = orderby_clause_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      LOG(ERROR) << "orderby clause has error" << endl;
      return ret;
    }
  }
  if (NULL != limit_clause_) {
    ret = limit_clause_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      LOG(ERROR) << "limit clause has error" << endl;
      return ret;
    }
  }
  sem_cnxt->RebuildTableColumn();
  sem_cnxt->PrintContext();
  return ret;
}

ErrorNo AstSelectStmt::PushDownCondition(PushDownConditionContext* pdccnxt) {
  if (NULL == pdccnxt) {
    pdccnxt = new PushDownConditionContext();
  }
  if (NULL != where_clause_) {
    AstWhereClause* where_clause =
        reinterpret_cast<AstWhereClause*>(where_clause_);
    pdccnxt->GetSubExprInfo(where_clause->expr_);
  }
  from_list_->PushDownCondition(pdccnxt);

  //  from_list_->Print();
  return eOK;
}
// should support expression in aggregation
ErrorNo AstSelectStmt::GetLogicalPlanOfAggeration(LogicalOperator* logic_plan) {
  return eOK;
}
//#define SUPPORT
ErrorNo AstSelectStmt::GetLogicalPlan(LogicalOperator*& logic_plan) {
  ErrorNo ret = eOK;
  if (NULL != from_list_) {
    ret = from_list_->GetLogicalPlan(logic_plan);
    if (eOK != ret) {
      return ret;
    }
  }
#ifdef SUPPORT
  if (have_aggeragion_) {
    ret = GetLogicalPlanOfAggeration(logic_plan);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != having_clause_) {
    ret = having_clause_->GetLogicalPlan(logic_plan);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != orderby_clause_) {
    ret = orderby_clause_->GetLogicalPlan(logic_plan);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != select_list_) {
    ret = select_list_->GetLogicalPlan(logic_plan);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != limit_clause_) {
    cout << "not support limit now !" << endl;
    return eOK;
    ret = limit_clause_->GetLogicalPlan(logic_plan);
    if (eOK != ret) {
      return ret;
    }
  }
#endif
  return eOK;
}

//}  // namespace sql_parser
//}  // namespace claims
