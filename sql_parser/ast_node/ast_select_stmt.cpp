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
#include "../../common/expression/expr_node.h"
#include "../../common/expression/expr_column.h"
#include "../../common/expression/expr_unary.h"
#include "../../Environment.h"
#include "../../Catalog/Attribute.h"
#include "../../Catalog/table.h"
#include "../../logical_operator/logical_aggregation.h"
#include "../../logical_operator/logical_equal_join.h"
#include "../../logical_operator/logical_cross_join.h"
#include "../../logical_operator/logical_filter.h"
#include "../../logical_operator/logical_project.h"
#include "../../logical_operator/logical_scan.h"
#include "../../logical_operator/logical_sort.h"
#include "../../logical_operator/logical_subquery.h"
#include "../../logical_operator/logical_delete_filter.h"

#include "../ast_node/ast_expr_node.h"
#include "../ast_node/ast_node.h"

using claims::common::ExprColumn;
using claims::common::ExprNodeType;
using claims::common::ExprUnary;
using claims::logical_operator::LogicalCrossJoin;
using claims::logical_operator::LogicalEqualJoin;
using claims::logical_operator::LogicalFilter;
using claims::logical_operator::LogicalProject;
using claims::logical_operator::LogicalScan;
using claims::logical_operator::LogicalSort;
using claims::logical_operator::LogicalSubquery;
using claims::logical_operator::LogicalDeleteFilter;

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
  // should add selectexpr->astcolumn()
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
  ErrorNo ret = rSuccess;
  if (NULL != args_) {
    sem_cnxt->clause_type_ = SemanticContext::kSelectClause;
    sem_cnxt->select_expr_have_agg = false;
    ret = args_->SemanticAnalisys(sem_cnxt);
    sem_cnxt->select_expr_have_agg = false;
    sem_cnxt->clause_type_ = SemanticContext::kNone;
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (is_all_) {
    ret = sem_cnxt->AddSelectAttrs(args_);  // collect select expr node
    return rSuccess;
  }
  if (NULL != next_) {
    ret = next_->SemanticAnalisys(sem_cnxt);
    return ret;
  }
  return rSuccess;
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
    args_->ReplaceAggregation(agg_column, agg_node, is_select);
  }
  if (NULL != next_) {
    next_->ReplaceAggregation(agg_column, agg_node, is_select);
  }
  return;
}
ErrorNo AstSelectList::GetLogicalPlan(LogicalOperator*& logic_plan) {
  return rSuccess;
}
AstSelectExpr::AstSelectExpr(AstNodeType ast_node_type, std::string expr_alias,
                             AstNode* expr)
    : AstNode(ast_node_type),
      expr_alias_(expr_alias),
      expr_(expr),
      have_agg_func_(false),
      have_alias_(expr_alias != "NULL") {}

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
  ErrorNo ret = rSuccess;
  if (NULL != expr_) {
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
    // transfer the alias to expr, but if this expr has aggregation, it mustn't
    // give the expr_alias_ go expr_str_, because aggregation shouldn't be
    // aliased at this time, but to be aliased after ReplaceAggregation(), at
    // that time, the aggregation has changed to be one column, the
    // select_expr_have_agg = false.
    if (!sem_cnxt->select_expr_have_agg) {
      expr_->expr_str_ = expr_alias_;
    }
    ret = sem_cnxt->AddSelectAttrs(expr_);   // collect select expr attr
    sem_cnxt->select_expr_.push_back(this);  // collect select expr node
    if (rSuccess != ret) {
      return ret;
    }
  }
  return rSuccess;
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
    int agg_node_num = agg_node.size();
    expr_->ReplaceAggregation(agg_column, agg_node, is_select);
    have_agg_func_ = (agg_node.size() > agg_node_num);
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
  ErrorNo ret = rSuccess;
  if (NULL != args_) {
    ret = args_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != next_) {
    ret = next_->SemanticAnalisys(sem_cnxt);
    return ret;
  }
  sem_cnxt->clause_type_ = SemanticContext::kNone;
  return rSuccess;
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
  return rSuccess;
}
ErrorNo AstFromList::GetLogicalPlan(LogicalOperator*& logic_plan) {
  LogicalOperator* args_lplan = NULL;
  LogicalOperator* next_lplan = NULL;
  ErrorNo ret = rSuccess;
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
      ret = GetEqualJoinPair(join_pair, args_lplan, next_lplan,
                             equal_join_condition_);
      if (rSuccess != ret) {
        return ret;
      }
      logic_plan = new LogicalEqualJoin(join_pair, args_lplan, next_lplan);
    } else {
      logic_plan = new LogicalCrossJoin(args_lplan, next_lplan);
    }
    if (normal_condition_.size() > 0) {
      vector<ExprNode*> condition;
      condition.clear();
      ret = GetFilterCondition(condition, normal_condition_, logic_plan);
      if (rSuccess != ret) {
        return ret;
      }
      logic_plan = new LogicalFilter(logic_plan, condition);
    }
  } else {
    logic_plan = args_lplan;
  }
  return rSuccess;
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
  return rSuccess;
}
ErrorNo AstTable::PushDownCondition(PushDownConditionContext* pdccnxt) {
  pdccnxt->from_tables_.insert(table_alias_);
  pdccnxt->SetCondition(equal_join_condition_, normal_condition_);
  return rSuccess;
}
// TODO(FZH) diver table_name_ to LogicalScan
ErrorNo AstTable::GetLogicalPlan(LogicalOperator*& logic_plan) {
  ErrorNo ret = rSuccess;

  //
  // TODO(xiaozhou): judge del is null or not
  if (NULL !=
      Environment::getInstance()->getCatalog()->getTable(table_name_ +
                                                         "_DEL")) {
    LogicalOperator* base_table = new LogicalScan(Environment::getInstance()
                                                      ->getCatalog()
                                                      ->getTable(table_name_)
                                                      ->getProjectoin(0),
                                                  table_alias_);
    Attribute filter_base =
        base_table->GetPlanContext().GetAttribute(table_alias_ + ".row_id");
    LogicalOperator* del_table =
        new LogicalScan(Environment::getInstance()
                            ->getCatalog()
                            ->getTable(table_name_ + "_DEL")
                            ->getProjectoin(0),
                        table_alias_ + "_DEL");
    Attribute filter_del = del_table->GetPlanContext().GetAttribute(
        table_alias_ + "_DEL.row_id_DEL");

    assert(filter_base.attrName != "NULL");
    assert(filter_del.attrName != "NULL");
    vector<LogicalDeleteFilter::FilterPair> filter_pair;
    filter_pair.push_back(
        LogicalDeleteFilter::FilterPair(filter_del, filter_base));
    logic_plan = new LogicalDeleteFilter(filter_pair, del_table, base_table);

  } else {
    logic_plan = new LogicalScan(Environment::getInstance()
                                     ->getCatalog()
                                     ->getTable(table_name_)
                                     ->getProjectoin(0),
                                 table_alias_);
  }
  if (equal_join_condition_.size() > 0) {
    LOG(ERROR) << "equal join condition shouldn't occur in a single table!"
               << endl;
    assert(false);
    return eEqualJoinCondiInATable;
  }
  if (normal_condition_.size() > 0) {
    vector<ExprNode*> condition;
    ExprNode* qnode = NULL;
    for (auto it = normal_condition_.begin(); it != normal_condition_.end();
         ++it) {
      ret = (*it)->GetLogicalPlan(qnode, logic_plan);
      if (rSuccess != ret) {
        LOG(ERROR) << "get normal condition upon a table, due to [err: " << ret
                   << " ] !" << endl;
        return ret;
      }
      assert(NULL != qnode);
      condition.push_back(qnode);
    }

    logic_plan = new LogicalFilter(logic_plan, condition);
  }
  return rSuccess;
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
  //      Environment::getInstance()->getCatalog()->getTable(subquery_alias_))
  //      {
  //    LOG(ERROR) << "subquery's alias couldn't equal to table that's exist
  //    in
  //    DB"
  //               << endl;
  //    return eTableAliasEqualExistedTable;
  //  }

  // subquery_ is OK?
  ErrorNo ret = subquery_->SemanticAnalisys(&sub_sem_cnxt);
  if (rSuccess != ret) {
    return ret;
  }
  //  // subquery_alias in tables that occur in subquery?
  //  ret = sub_sem_cnxt.IsTableExist(subquery_alias_);
  //  if (eOK == ret) {
  //    return eTableAliasEqualLowerAlias;
  //  }

  // add this sub query alias into upper query's table list
  ret = sem_cnxt->AddTable(subquery_alias_);
  if (rSuccess != ret) {
    return ret;
  }
  // reconstruct the output attribute in subquery
  multimap<string, string> column_to_table;
  ret = sub_sem_cnxt.GetAliasColumn(subquery_alias_, column_to_table);
  if (rSuccess != ret) {
    return ret;
  }

  return sem_cnxt->AddTableColumn(column_to_table);
}

ErrorNo AstSubquery::PushDownCondition(PushDownConditionContext* pdccnxt) {
  pdccnxt->from_tables_.insert(subquery_alias_);
  pdccnxt->SetCondition(equal_join_condition_, normal_condition_);
  return rSuccess;
}
// may be deliver subquery output schema
ErrorNo AstSubquery::GetLogicalPlan(LogicalOperator*& logic_plan) {
  ErrorNo ret = rSuccess;
  ret = subquery_->GetLogicalPlan(logic_plan);
  if (rSuccess != ret) {
    return ret;
  }
  logic_plan = new LogicalSubquery(logic_plan, subquery_alias_);
  return rSuccess;
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
  return rSuccess;
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
  ErrorNo ret = rSuccess;
  SemanticContext join_sem_cnxt;
  if (NULL == left_table_) {
    LOG(ERROR) << "left table is null in join!" << endl;
    assert(false);
    return eLeftTableIsNULLInJoin;
  }
  if (NULL == right_table_) {
    LOG(ERROR) << "right table is null in join!" << endl;
    assert(false);
    return eRightTableIsNULLInJoin;
  }
  ret = left_table_->SemanticAnalisys(&join_sem_cnxt);
  if (rSuccess != ret) {
    return ret;
  }

  ret = right_table_->SemanticAnalisys(&join_sem_cnxt);
  if (rSuccess != ret) {
    return ret;
  }

  if (NULL != join_condition_) {
    ret = join_condition_->SemanticAnalisys(&join_sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  ret = sem_cnxt->AddTable(join_sem_cnxt.get_tables());
  ret = sem_cnxt->AddTableColumn(join_sem_cnxt.get_column_to_table());
  //  join_sem_cnxt.~SemanticContext();
  return ret;
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
  return rSuccess;
}
ErrorNo AstJoin::GetLogicalPlan(LogicalOperator*& logic_plan) {
  LogicalOperator* left_plan = NULL;
  LogicalOperator* right_plan = NULL;
  ErrorNo ret = rSuccess;
  ret = left_table_->GetLogicalPlan(left_plan);
  if (rSuccess != ret) {
    return ret;
  }
  ret = right_table_->GetLogicalPlan(right_plan);
  if (rSuccess != ret) {
    return ret;
  }
  if (equal_join_condition_.size() > 0) {
    vector<LogicalEqualJoin::JoinPair> join_pair;
    join_pair.clear();
    ret = GetEqualJoinPair(join_pair, left_plan, right_plan,
                           equal_join_condition_);
    if (rSuccess != ret) {
      return ret;
    }
    logic_plan = new LogicalEqualJoin(join_pair, left_plan, right_plan);
  } else {
    logic_plan = new LogicalCrossJoin(left_plan, right_plan);
  }
  if (normal_condition_.size() > 0) {
    vector<ExprNode*> condition;
    condition.clear();
    ret = GetFilterCondition(condition, normal_condition_, logic_plan);
    if (rSuccess != ret) {
      return ret;
    }
    logic_plan = new LogicalFilter(logic_plan, condition);
  }
  return rSuccess;
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
    ErrorNo ret = rSuccess;
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) return ret;
    sem_cnxt->clause_type_ = SemanticContext::kNone;
  }
  return rSuccess;
}
void AstWhereClause::RecoverExprName(string& name) {
  if (NULL != expr_) {
    expr_->RecoverExprName(name);
  }
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
  ErrorNo ret = rSuccess;
  if (NULL != expr_) {
    // to limit that don't support expression in group by
    //    // don't support expression in group by list
    //    if (AST_COLUMN != expr_->ast_node_type()) {
    //      LOG(ERROR) << "group by list should be single column!" << endl;
    //      return eGroupByNotSupportColumn;
    //    }
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
    sem_cnxt->AddGroupByAttrs(expr_);  // collect group by expr node
  }
  if (NULL != next_) {
    ret = next_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  return rSuccess;
}
void AstGroupByList::RecoverExprName(string& name) {
  if (NULL != expr_) {
    expr_->RecoverExprName(name);
  }
  if (NULL != next_) {
    next_->RecoverExprName(name);
  }
}
// for select a+b as A, count(*) from TB group by A, should to be "select A,
// count(*) from TB group by a+b as A".
ErrorNo AstGroupByList::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  ErrorNo ret = rSuccess;
  if (NULL != expr_) {
    AstSelectExpr* select_expr_i = NULL;
    if (AST_COLUMN == expr_->ast_node_type()) {
      AstColumn* column = reinterpret_cast<AstColumn*>(expr_);
      for (int i = 0; i < select_alias_solver->select_expr_.size(); ++i) {
        select_expr_i = reinterpret_cast<AstSelectExpr*>(
            select_alias_solver->select_expr_[i]);
        if (select_expr_i->have_alias_ && !select_expr_i->have_agg_func_ &&
            select_expr_i->expr_alias_ == column->column_name_) {
          // even through is false, but for avoiding ignore solve it again
          select_expr_i->have_agg_func_ = true;
          // should give alias to expr_str_
          //          select_expr_i->expr_->expr_str_ =
          //          select_expr_i->expr_alias_;
          swap(select_expr_i->expr_, expr_);
          break;
        }
      }
    }
  }
  if (NULL != next_) {
    ret = next_->SolveSelectAlias(select_alias_solver);
  }
  return ret;
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
  ErrorNo ret = rSuccess;
  if (NULL != groupby_list_) {
    sem_cnxt->clause_type_ = SemanticContext::kGroupByClause;
    ret = groupby_list_->SemanticAnalisys(sem_cnxt);
    sem_cnxt->clause_type_ = SemanticContext::kNone;
    if (rSuccess != ret) {
      return ret;
    }
    return rSuccess;
  }
  return eGroupbyListIsNULL;
}
void AstGroupByClause::RecoverExprName(string& name) {
  if (NULL != groupby_list_) {
    groupby_list_->RecoverExprName(name);
  }
}
ErrorNo AstGroupByClause::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  if (NULL != groupby_list_) {
    ErrorNo ret = groupby_list_->SolveSelectAlias(select_alias_solver);
    if (rSuccess != ret) {
      return ret;
    }
  }
  return rSuccess;
}

AstOrderByList::AstOrderByList(AstNodeType ast_node_type, AstNode* expr,
                               int orderby_type, AstNode* next)
    : AstNode(ast_node_type),
      expr_(expr),
      orderby_direction_(orderby_type == 0 ? "ASC" : "DESC"),
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
       << "orderby direction: " << orderby_direction_ << endl;
  if (next_ != NULL) {
    next_->Print(level);
  }
}
ErrorNo AstOrderByList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = rSuccess;
  if (NULL != expr_) {
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != next_) {
    ret = next_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  return rSuccess;
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
ErrorNo AstOrderByList::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  if (NULL != expr_) {
    expr_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(expr_);
    select_alias_solver->DeleteOldNode();
  }
  if (NULL != next_) {
    next_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(next_);
    select_alias_solver->DeleteOldNode();
  }
  return rSuccess;
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
    ErrorNo ret = rSuccess;
    ret = orderby_list_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
    sem_cnxt->clause_type_ = SemanticContext::kNone;
    return rSuccess;
  }
  return rSuccess;
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
ErrorNo AstOrderByClause::GetLogicalPlan(LogicalOperator*& logic_plan) {
  vector<pair<ExprNode*, int>> orderby_expr;
  ExprNode* tmp_expr = NULL;
  int direction = 0;
  AstOrderByList* orderby = orderby_list_;
  while (NULL != orderby) {
    orderby->expr_->GetLogicalPlan(tmp_expr, logic_plan);
    direction = orderby->orderby_direction_ == "ASC" ? 0 : 1;
    orderby_expr.push_back(make_pair(tmp_expr, direction));
    orderby = orderby->next_;
  }
  logic_plan = new LogicalSort(logic_plan, orderby_expr);
  return rSuccess;
}
ErrorNo AstOrderByClause::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  if (NULL != orderby_list_) {
    orderby_list_->SolveSelectAlias(select_alias_solver);
  }
  return rSuccess;
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
    ErrorNo ret = rSuccess;
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
    sem_cnxt->clause_type_ = SemanticContext::kNone;
  }
  return rSuccess;
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
ErrorNo AstHavingClause::GetLogicalPlan(LogicalOperator*& logic_plan) {
  if (NULL != expr_) {
    vector<ExprNode*> having_expr;
    ExprNode* expr = NULL;
    expr_->GetLogicalPlan(expr, logic_plan);
    having_expr.push_back(expr);
    logic_plan = new LogicalFilter(logic_plan, having_expr);
  }
  return rSuccess;
}
ErrorNo AstHavingClause::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  if (NULL != expr_) {
    expr_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(expr_);
    select_alias_solver->DeleteOldNode();
  }
  return rSuccess;
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
  ErrorNo ret = rSuccess;
  if (NULL != offset_) {
    ret = offset_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != row_count_) {
    return row_count_->SemanticAnalisys(sem_cnxt);
  }
  return rSuccess;
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
      column_name_(column_name) {
  if (NULL != next) {
    next_ = next->AstNodeCopy();
  } else {
    next_ = NULL;
  }
}
AstColumn::AstColumn(AstColumn* node)
    : AstNode(node),
      relation_name_(node->relation_name_),
      column_name_(node->column_name_) {
  if (NULL != node->next_) {
    next_ = node->next_->AstNodeCopy();
  } else {
    next_ = NULL;
  }
}
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
  ErrorNo ret = rSuccess;
  if (AST_COLUMN_ALL_ALL == ast_node_type_) {
    if (SemanticContext::kSelectClause == sem_cnxt->clause_type_) {
      return rSuccess;
    } else {
      return eColumnAllShouldNotInOtherClause;
    }
  }
  if (AST_COLUMN_ALL == ast_node_type_) {
    if (SemanticContext::kSelectClause == sem_cnxt->clause_type_) {
      ret = sem_cnxt->IsTableExist(relation_name_);
      if (rSuccess != ret) {
        return eTableNotExistInTableColumnALL;
      }
    } else {
      return eColumnAllShouldNotInOtherClause;
    }
    return rSuccess;
  }
  ret = sem_cnxt->IsColumnExist(relation_name_, column_name_);
  if (rSuccess != ret) {
    LOG(ERROR) << "There are errors in ( " << relation_name_ << " , "
               << column_name_ << " )" << endl;
    return ret;
  }
  if (NULL != next_) {
    return next_->SemanticAnalisys(sem_cnxt);
  }
  return rSuccess;
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
 * because GetRefTable from where clause, so there couldn't be AST_COLUMN_ALL
 * or
 * AST_COLUMN_ALL_ALL
 * relation_name_=="NULL" is illegal, but "NULL_MID" means for aggregation, so
 * it's legal
 */
void AstColumn::GetRefTable(set<string>& ref_table) {
  assert(AST_COLUMN_ALL != ast_node_type_ &&
         AST_COLUMN_ALL_ALL != ast_node_type_);
  assert(relation_name_ != "NULL");
  ref_table.insert(relation_name_);
}

ErrorNo AstColumn::GetLogicalPlan(ExprNode*& logic_expr,
                                  LogicalOperator* child_logic_plan) {
  logic_expr = new ExprColumn(
      ExprNodeType::t_qcolcumns,
      child_logic_plan->GetPlanContext()
          .GetAttribute(relation_name_, relation_name_ + "." + column_name_)
          .attrType->type,
      expr_str_, relation_name_, column_name_);
  return rSuccess;
}
ErrorNo AstColumn::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  ErrorNo ret = rSuccess;
  AstSelectExpr* select_expr_i = NULL;
  for (int i = 0; i < select_alias_solver->select_expr_.size(); ++i) {
    select_expr_i =
        reinterpret_cast<AstSelectExpr*>(select_alias_solver->select_expr_[i]);
    if (select_expr_i->have_alias_ &&
        select_expr_i->expr_alias_ == column_name_) {
      select_alias_solver->new_node_ = (select_expr_i->expr_)->AstNodeCopy();
      assert(select_alias_solver->new_node_ != NULL);
      select_alias_solver->old_node_ = this;
      break;
    }
  }
  return rSuccess;
}
AstNode* AstColumn::AstNodeCopy() { return new AstColumn(this); }

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
 *  from-> where-> groupby-> select_aggregation->select_expression->
 *  having->orderby-> limit
 */

ErrorNo AstSelectStmt::SemanticAnalisys(SemanticContext* sem_cnxt) {
  if (NULL == sem_cnxt) {
    SemanticContext sem;
    sem_cnxt = &sem;
  }
  ErrorNo ret = rSuccess;
  // check whether table exist or table alias conflict
  if (NULL != from_list_) {
    ret = from_list_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      LOG(ERROR) << "from clause has error" << endl;
      return ret;
    }
  } else {
    LOG(WARNING) << "from clause is NULL" << endl;
  }
  // check whether column exist
  // TODO(FZH) check every functions
  if (NULL != where_clause_) {
    string name = "";
    where_clause_->RecoverExprName(name);
    ret = where_clause_->SemanticAnalisys(sem_cnxt);

    if (rSuccess != ret) {
      LOG(ERROR) << "where clause has error" << endl;
      return ret;
    }
  }
#ifdef PRINTCONTEXT
  sem_cnxt->PrintContext("after scan");
#endif
  // first recover select attr name
  // collect all aggregation functions
  agg_attrs_.clear();
  if (NULL != select_list_) {
    string name = "";
    select_list_->RecoverExprName(name);

    ret = select_list_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      LOG(ERROR) << "select list has error" << endl;
      return ret;
    }
    // collecting aggregation and replace a aggregation expression to one
    // column
    AstNode* agg_column = NULL;
    select_list_->ReplaceAggregation(agg_column, agg_attrs_, true);

    if (agg_attrs_.size() > 0) {
      sem_cnxt->GetUniqueAggAttr(agg_attrs_);
    }

  } else {
    LOG(ERROR) << "select list is NULL" << endl;
    return eSelectClauseIsNULL;
  }
  // aggregation couldn't in group by clause
  // collect all group by attributes to rebuild schema
  if (NULL != groupby_clause_) {
    string name = "";
    groupby_clause_->RecoverExprName(name);
    // mustn't change the order of upper clause and below
    SelectAliasSolver* select_alias_solver =
        new SelectAliasSolver(sem_cnxt->select_expr_);
    groupby_clause_->SolveSelectAlias(select_alias_solver);
    ret = groupby_clause_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      LOG(ERROR) << "groupby clause has error" << endl;
      return ret;
    }
    groupby_attrs_ = sem_cnxt->get_groupby_attrs();
  }

  have_aggeragion_ = (NULL != groupby_clause_ || agg_attrs_.size() > 0);

  if (have_aggeragion_) {
    // rebuild schema result from aggregation
    ErrorNo ret = sem_cnxt->RebuildTableColumn(agg_attrs_);
    if (rSuccess != ret) {
      LOG(ERROR) << "there are confiction in new schema after agg!" << endl;
      return ret;
    }
#ifdef PRINTCONTEXT
    sem_cnxt->PrintContext("after aggregation");
#endif
  }
  // first replace alias to original expression
  // having clause exist only if have aggregation
  if (NULL != having_clause_) {
    if (!have_aggeragion_) {
      return eHavingNotAgg;
    }
    SelectAliasSolver* select_alias_solver =
        new SelectAliasSolver(sem_cnxt->select_expr_);
    having_clause_->SolveSelectAlias(select_alias_solver);

    string name = "";
    having_clause_->RecoverExprName(name);

    AstNode* agg_column = NULL;
    set<AstNode*> agg_node_having;
    having_clause_->ReplaceAggregation(agg_column, agg_node_having, false);

    ret = having_clause_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      LOG(ERROR) << "having clause has error" << endl;
      return ret;
    }
  }
  if (NULL != orderby_clause_) {
    ErrorNo ret = rSuccess;
    SelectAliasSolver* select_alias_solver =
        new SelectAliasSolver(sem_cnxt->select_expr_);
    orderby_clause_->SolveSelectAlias(select_alias_solver);

    string name = "";
    orderby_clause_->RecoverExprName(name);

    AstNode* agg_column = NULL;
    set<AstNode*> agg_node_orderby;
    orderby_clause_->ReplaceAggregation(agg_column, agg_node_orderby, false);

    ret = orderby_clause_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      LOG(ERROR) << "orderby clause has error" << endl;
      return ret;
    }
  }
  if (NULL != limit_clause_) {
    ret = limit_clause_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      LOG(ERROR) << "limit clause has error" << endl;
      return ret;
    }
  }
  if (have_aggeragion_) {
    AstNode* agg_column = NULL;
    select_list_->ReplaceAggregation(agg_column, agg_attrs_, false);
    // check whether other column except from aggregation funcs and groupby
    // expressions in select expressions
    sem_cnxt->ClearSelectAttrs();
    ret = select_list_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return eAggSelectExprHaveOtherColumn;
    }
  }
  // for select clause rebuild table column
  sem_cnxt->RebuildTableColumn();
#ifdef PRINTCONTEXT
  sem_cnxt->PrintContext("after select");
#endif

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
  return rSuccess;
}
// should support expression in aggregation
ErrorNo AstSelectStmt::GetLogicalPlanOfAggeration(
    LogicalOperator*& logic_plan) {
  vector<ExprNode*> group_by_attrs;
  vector<ExprUnary*> aggregation_attrs;
  ExprNode* tmp_expr = NULL;
  ErrorNo ret = rSuccess;
  for (auto it = groupby_attrs_.begin(); it != groupby_attrs_.end(); ++it) {
    ret = (*it)->GetLogicalPlan(tmp_expr, logic_plan);
    if (rSuccess != ret) {
      return ret;
    }
    group_by_attrs.push_back(tmp_expr);
  }
  for (auto it = agg_attrs_.begin(); it != agg_attrs_.end(); ++it) {
    ret = (*it)->GetLogicalPlan(tmp_expr, logic_plan);
    if (rSuccess != ret) {
      return ret;
    }
    aggregation_attrs.push_back(reinterpret_cast<ExprUnary*>(tmp_expr));
  }
  logic_plan =
      new LogicalAggregation(group_by_attrs, aggregation_attrs, logic_plan);
  return rSuccess;
}

ErrorNo AstSelectStmt::GetLogicalPlanOfProject(LogicalOperator*& logic_plan) {
  AstSelectList* select_list = reinterpret_cast<AstSelectList*>(select_list_);
  vector<ExprNode*> expr_list;
  vector<AstNode*> ast_expr;
  ExprNode* tmp_expr = NULL;
  ErrorNo ret = rSuccess;
  ast_expr.clear();
  expr_list.clear();
  while (NULL != select_list) {
    if (select_list->is_all_) {  // select * from tb;
      return rSuccess;
    }
    AstSelectExpr* select_expr =
        reinterpret_cast<AstSelectExpr*>(select_list->args_);
    switch (select_expr->expr_->ast_node_type()) {
      case AST_COLUMN_ALL_ALL: {
        return rSuccess;
      } break;
      case AST_COLUMN_ALL: {
        AstColumn* column = reinterpret_cast<AstColumn*>(select_expr->expr_);
        vector<Attribute> attrs = Environment::getInstance()
                                      ->getCatalog()
                                      ->getTable(column->relation_name_)
                                      ->getAttributes();
        for (auto it = attrs.begin(); it != attrs.end(); ++it) {
          ast_expr.push_back(new AstColumn(
              AST_COLUMN, column->relation_name_,
              it->attrName.substr(it->attrName.find('.') + 1), it->attrName));
        }
      } break;
      default: { ast_expr.push_back(select_expr->expr_); }
    }

    if (NULL != select_list->next_) {
      select_list = reinterpret_cast<AstSelectList*>(select_list->next_);
    } else {
      select_list = NULL;
    }
  }
  for (int i = 0; i < ast_expr.size(); ++i) {
    ret = ast_expr[i]->GetLogicalPlan(tmp_expr, logic_plan);
    if (rSuccess != ret) {
      return rSuccess;
    }
    expr_list.push_back(tmp_expr);
  }
  logic_plan = new LogicalProject(logic_plan, expr_list);
  return rSuccess;
}

// #define SUPPORT
ErrorNo AstSelectStmt::GetLogicalPlan(LogicalOperator*& logic_plan) {
  ErrorNo ret = rSuccess;
  if (NULL != from_list_) {
    ret = from_list_->GetLogicalPlan(logic_plan);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (have_aggeragion_) {
    ret = GetLogicalPlanOfAggeration(logic_plan);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != having_clause_) {
    ret = having_clause_->GetLogicalPlan(logic_plan);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != orderby_clause_) {
    ret = orderby_clause_->GetLogicalPlan(logic_plan);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != select_list_) {
    ret = GetLogicalPlanOfProject(logic_plan);
    if (rSuccess != ret) {
      return ret;
    }
  }
#ifdef SUPPORT
  if (NULL != limit_clause_) {
    cout << "not support limit now !" << endl;
    return rSuccess;
    ret = limit_clause_->GetLogicalPlan(logic_plan);
    if (rSuccess != ret) {
      return ret;
    }
  }
#endif
  return rSuccess;
}

//}  // namespace sql_parser
//}  // namespace claims
