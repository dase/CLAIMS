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

#include <assert.h>
#include <glog/logging.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "./ast_select_stmt.h"
#include "./ast_expr_node.h"
#include "../../logical_operator/logical_operator.h"
using claims::logical_operator::LogicalOperator;
using std::cout;
using std::setw;
using std::endl;
using std::string;
using std::endl;
using std::vector;
// namespace claims {
// namespace sql_parser {
AstNode::AstNode(AstNodeType ast_node_type) : ast_node_type_(ast_node_type) {}
AstNode::AstNode(AstNode* node)
    : ast_node_type_(node->ast_node_type_), expr_str_(node->expr_str_) {}
AstNode::~AstNode() {}
void AstNode::Print(int level) const {
  cout << setw(level * 8) << " "
       << "This is an AST_NODE!" << endl;
}
ErrorNo AstNode::SemanticAnalisys(SemanticContext* sem_cnxt) {
  LOG(WARNING) << "This is AstNode's semantic analysis" << endl;
  return eOK;
}

AstNodeType AstNode::ast_node_type() { return ast_node_type_; }

AstStmtList::AstStmtList(AstNodeType ast_node_type, AstNode* stmt,
                         AstNode* next)
    : AstNode(ast_node_type), stmt_(stmt), next_(next) {}

AstStmtList::~AstStmtList() {
  delete stmt_;
  delete next_;
}
void AstNode::RecoverExprName(string& name) { name = "AstNode"; }
void AstNode::ReplaceAggregation(AstNode*& agg_column, set<AstNode*>& agg_node,
                                 bool is_select) {
  LOG(INFO) << "this is in ast base node!" << endl;
}
void AstNode::GetSubExpr(vector<AstNode*>& sub_expr, bool is_top_and) {
  is_top_and = false;
  sub_expr.push_back(this);
  //  cout << "GetSubExpr ast node type is : " << ast_node_type_ << endl;
}
void AstNode::GetRefTable(set<string>& ref_table) { return; }
void GetJoinedRoot(map<string, AstNode*> table_joined_root,
                   AstNode* joined_root) {
  return;
}
ErrorNo AstNode::GetEqualJoinPair(vector<LogicalEqualJoin::JoinPair>& join_pair,
                                  LogicalOperator* left_plan,
                                  LogicalOperator* right_plan,
                                  const set<AstNode*>& equal_join_condition) {
  for (auto it = equal_join_condition.begin(); it != equal_join_condition.end();
       ++it) {
    AstExprCmpBinary* equal_condi = reinterpret_cast<AstExprCmpBinary*>(*it);
    AstColumn* left_node = reinterpret_cast<AstColumn*>(equal_condi->arg0_);
    AstColumn* right_node = reinterpret_cast<AstColumn*>(equal_condi->arg1_);
    Attribute attr0 = left_plan->GetPlanContext().GetAttribute(
        left_node->relation_name_ + "." + left_node->column_name_);
    Attribute attr1 = right_plan->GetPlanContext().GetAttribute(
        right_node->relation_name_ + "." + right_node->column_name_);
    if (attr0.attrName != "NULL" && attr1.attrName != "NULL") {
      join_pair.push_back(LogicalEqualJoin::JoinPair(attr0, attr1));
      continue;
    }
    Attribute attr3 = left_plan->GetPlanContext().GetAttribute(
        right_node->relation_name_ + "." + right_node->column_name_);
    Attribute attr4 = right_plan->GetPlanContext().GetAttribute(
        left_node->relation_name_ + "." + left_node->column_name_);
    if (attr3.attrName != "NULL" && attr4.attrName != "NULL") {
      join_pair.push_back(LogicalEqualJoin::JoinPair(attr3, attr4));
      continue;
    } else {
      LOG(ERROR) << "equal condition couldn't match separately!" << endl;
      assert(false);
      return eEqualJoinCondiNotMatch;
    }
  }
  return eOK;
}
ErrorNo AstNode::GetFilterCondition(vector<ExprNode*>& condition,
                                    const set<AstNode*>& normal_condition,
                                    LogicalOperator* logic_plan) {
  ErrorNo ret = eOK;
  ExprNode* expr_node = NULL;
  for (auto it = normal_condition.begin(); it != normal_condition.end(); ++it) {
    ret = (*it)->GetLogicalPlan(expr_node, logic_plan);
    if (eOK != ret) {
      LOG(ERROR) << "get normal condition upon from list, due to [err: " << ret
                 << " ] !" << endl;
      return ret;
    }
    assert(NULL != expr_node);
    condition.push_back(expr_node);
  }
  return eOK;
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
ErrorNo AstStmtList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != stmt_) {
    ret = stmt_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != next_) {
    LOG(ERROR) << "just support one statement now!" << endl;
    assert(false);
    return eOK;
    return next_->SemanticAnalisys(sem_cnxt);
  }
  return eOK;
}
ErrorNo AstStmtList::PushDownCondition(PushDownConditionContext* pdccnxt) {
  if (NULL != stmt_) {
    stmt_->PushDownCondition(pdccnxt);
  }
  if (NULL != next_) {
    next_->PushDownCondition(pdccnxt);
  }
  return eOK;
}
ErrorNo AstStmtList::GetLogicalPlan(LogicalOperator*& logic_plan) {
  return stmt_->GetLogicalPlan(logic_plan);
}
SemanticContext::SemanticContext() {
  tables_.clear();
  column_to_table_.clear();
  aggregation_.clear();
  groupby_attrs_.clear();
  select_attrs_.clear();
  agg_upper_ = NULL;
  clause_type_ = kNone;
  have_agg = false;
  select_expr_have_agg = false;
}

SemanticContext::~SemanticContext() {}

ErrorNo SemanticContext::IsTableExist(const string table) {
  if (1 != tables_.count(table)) return eTableNotExist;
  return eOK;
}
/**
 * "NULL".column could be ambiguous or not exist, otherwise replace "NULL" with
 * unique name
 */
ErrorNo SemanticContext::IsColumnExist(string& table, const string column) {
  int table_counts = column_to_table_.count(column);
  if (table_counts == 0) {
    LOG(ERROR) << column << " doesn't exist in " << table << endl;
    return eColumnNotExist;
  } else if (table_counts > 1) {  // ambiguous
    if (table == "NULL") {        // NULL.col
      LOG(ERROR) << column << " is ambiguous!" << endl;
      return eColumnIsAmbiguous;
    } else {  // tbl.col
      auto table_range = column_to_table_.equal_range(column);
      for (auto it = table_range.first; it != table_range.second; ++it) {
        if (it->second == table) {
          return eOK;
        }
      }
      LOG(ERROR) << table << " don't have " << column << endl;
      return eTabelHaveNotColumn;
    }
  } else {  // unique col
    auto it = column_to_table_.find(column);
    if (table == "NULL") {  // NULL.col
      table = it->second;
      return eOK;
    } else {  // tbl.col
      if (it->second == table) {
        return eOK;
      } else {
        LOG(ERROR) << column << " doesn't exist in " << table << endl;
        return eColumnNotExist;
      }
    }
  }
}
ErrorNo SemanticContext::AddTable(set<string> table) {
  tables_.insert(table.begin(), table.end());
  return eOK;
}
ErrorNo SemanticContext::AddTable(string table) {
  tables_.insert(table);
  return eOK;
}
set<string> SemanticContext::get_tables() { return tables_; }

/**
 * if <table,column> doesn't exist in column_to_table_, so add into it,
 * otherwise, ignore it.
 */
ErrorNo SemanticContext::AddTableColumn(const string& table,
                                        const string& column) {
  if (table == "NULL") {
    LOG(ERROR) << "table name couldn't be 'NULL'!";
    return eTableillegal;
  } else {  // guarantee the <table,column> is unique
    int counts = 0;
    auto table_range = column_to_table_.equal_range(table);
    for (auto it = table_range.first; it != table_range.second; ++it) {
      if (it->second == table) {
        counts++;
      }
    }
    if (0 == counts) {
      column_to_table_.insert(make_pair(column, table));
    }
  }
  return eOK;
}

/**
 * check tb.col to add is ambiguous to existed tb.col
 */
ErrorNo SemanticContext::AddTableColumn(
    const multimap<string, string>& column_to_table) {
  for (auto it_column = column_to_table_.begin();
       it_column != column_to_table_.end(); ++it_column) {
    for (auto it = column_to_table.begin(); it != column_to_table.end(); ++it) {
      if (it_column->first == it->first && it_column->second == it->second) {
        return eColumnIsAmbiguousToExistedColumn;
      }
    }
  }
  column_to_table_.insert(column_to_table.begin(), column_to_table.end());
  return eOK;
}
void SemanticContext::GetTableAllColumn(string table,
                                        multimap<string, string>& new_columns) {
  for (auto it = column_to_table_.begin(); it != column_to_table_.end(); ++it) {
    if (table == it->second) {
      new_columns.insert(make_pair(it->first, it->second));
    }
  }
}
void SemanticContext::GetUniqueAggAttr(set<AstNode*>& new_set) {
  map<string, AstNode*> temp_map;
  bool exist = false;
  for (auto it = new_set.begin(); it != new_set.end(); ++it) {
    temp_map.insert(make_pair((*it)->expr_str_, (*it)));
  }
  new_set.clear();
  for (auto it = temp_map.begin(); it != temp_map.end(); ++it) {
    new_set.insert(it->second);
  }
}

ErrorNo SemanticContext::AddNewTableColumn(set<AstNode*>& new_set,
                                           bool need_clear) {
  ErrorNo ret = eOK;
  multimap<string, string> new_columns;
  new_columns.clear();
  for (auto it = new_set.begin(); it != new_set.end(); ++it) {
    if (AST_COLUMN_ALL == (*it)->ast_node_type_) {
      // add the columns whose table=table.*
      AstColumn* col = reinterpret_cast<AstColumn*>(*it);
      GetTableAllColumn(col->relation_name_, new_columns);
    } else if (AST_COLUMN == (*it)->ast_node_type_) {
      // because the column may be aliased, so new column
      //                =
      // alias, if not aliased initially, the alias = column or
      //            table.column, so
      // should remove "table." from "table.column")
      //            attr=col->expr_str_(

      AstColumn* col = reinterpret_cast<AstColumn*>(*it);
      if (col->expr_str_ == (col->relation_name_ + "." + col->column_name_) ||
          col->expr_str_ == col->column_name_) {
        new_columns.insert(make_pair(col->column_name_, col->relation_name_));
      } else {  // the column is aliased
        new_columns.insert(make_pair(col->expr_str_, "NULL_MID"));
      }
    } else if (AST_COLUMN_ALL_ALL == (*it)->ast_node_type_) {
      // must just one *.* in select, and the columns_to_table_ couldn't change
      if (new_set.size() != 1) {
        LOG(ERROR) << "more columns in select in which has *.*" << endl;
        return eMoreColumnsInSelectHaveALLALL;
      }
      assert(new_set.size() == 1);
      return eOK;
    } else {  // expr_str_ = tb.col, alias, col and expr_name
      new_columns.insert(make_pair((*it)->expr_str_, "NULL_MID"));
    }
  }
  if (need_clear) {
    ClearColumn();
  }
  ret = AddTableColumn(new_columns);
  if (eOK != ret) {
    return ret;
  }
  return eOK;
}
ErrorNo SemanticContext::RebuildTableColumn(set<AstNode*>& aggregation) {
  ClearTable();
  ErrorNo ret = eOK;
  ret = AddNewTableColumn(aggregation, true);
  if (eOK != ret) {
    return ret;
  }
  ret = AddNewTableColumn(groupby_attrs_, false);
  if (eOK != ret) {
    return ret;
  }
  return eOK;
}
ErrorNo SemanticContext::RebuildTableColumn() {
  ClearTable();
  return AddNewTableColumn(select_attrs_, true);
}
/**
 * check <tb1.a,tb2.a> isn't ambiguous in subquery, but <alias.a,alias.a> is
 * ambiguous at upper level
 */
ErrorNo SemanticContext::GetAliasColumn(
    const string& alias, multimap<string, string>& column_to_table) {
  for (auto it = column_to_table_.begin(); it != column_to_table_.end(); ++it) {
    if (column_to_table_.count(it->first) > 1) {
      return eColumnIsAmbiguousAfterAlias;
    }
    column_to_table.insert(make_pair(it->first, alias));
  }
  return eOK;
}
ErrorNo SemanticContext::AddAggregation(AstNode* agg_node) {
  if (aggregation_.count(agg_node) > 0) {
    return eOK;
  }
  bool exist = false;
  for (auto it = aggregation_.begin(); it != aggregation_.end(); ++it) {
    if (agg_node->expr_str_ == "") {
      return eAggNodeExprStrIsNULL;
    }
    if (agg_node->expr_str_ == (*it)->expr_str_) {
      exist = true;
      break;
    }
  }
  if (exist) {
    LOG(INFO) << "eliminate one aggregation" << endl;
  } else {
    aggregation_.insert(agg_node);
  }
  return eOK;
}
ErrorNo SemanticContext::AddGroupByAttrs(AstNode* groupby_node) {
  if (groupby_attrs_.count(groupby_node) > 0) {
    return eOK;
  }
  bool exist = false;
  for (auto it = groupby_attrs_.begin(); it != groupby_attrs_.end(); ++it) {
    if (groupby_node->expr_str_ == "") {
      return eGroupbyNodeExprStrIsNULL;
    }
    if (groupby_node->expr_str_ == (*it)->expr_str_) {
      exist = true;
      break;
    }
  }
  if (exist) {
    LOG(INFO) << "eliminate one groupby node" << endl;
  } else {
    groupby_attrs_.insert(groupby_node);
  }
  return eOK;
}
ErrorNo SemanticContext::AddSelectAttrs(AstNode* select_node) {
  if (select_attrs_.count(select_node) > 0) {
    return eOK;
  }
  bool exist = false;
  // no need to check conflict in select
  //  if (select_node->expr_str_ == "") {
  //    return eSelectNodeExprStrIsNULL;
  //  }
  //  for (auto it = select_attrs_.begin(); it != select_attrs_.end(); ++it) {
  //    if (select_node->expr_str_ == (*it)->expr_str_) {
  //      exist = true;
  //      break;
  //    }
  //  }
  if (exist) {
    LOG(ERROR) << "alias confict in one select node" << endl;
    return eAliasConfictInSelectNode;
  } else {
    select_attrs_.insert(select_node);
  }
  return eOK;
}
set<AstNode*> SemanticContext::get_aggregation() { return aggregation_; }
set<AstNode*> SemanticContext::get_groupby_attrs() {
  GetUniqueAggAttr(groupby_attrs_);
  return groupby_attrs_;
}
set<AstNode*> SemanticContext::get_select_attrs() { return select_attrs_; }
multimap<string, string> SemanticContext::get_column_to_table() {
  return column_to_table_;
}

void SemanticContext::ClearColumn() { column_to_table_.clear(); }

void SemanticContext::ClearTable() { tables_.clear(); }

void SemanticContext::PrintContext(string flag) {
  cout << "~~~~~~~~~~~~~~~~" << flag << "~~~~~~~~~~~~~~~~~~" << endl;
  cout << "++++print Tables++++  " << tables_.size() << endl;
  for (auto it = tables_.begin(); it != tables_.end(); ++it) {
    cout << (*it) << endl;
  }
  cout << "----print Columns----  " << column_to_table_.size() << endl;
  for (auto it = column_to_table_.begin(); it != column_to_table_.end(); ++it) {
    cout << it->first << "  " << it->second << endl;
  }
  cout << endl;
  cout << "---------------------\n" << endl;
}

PushDownConditionContext::PushDownConditionContext() { from_tables_.clear(); }
bool PushDownConditionContext::IsTableSubSet(set<string>& expr_tables,
                                             set<string>& from_tables) {
  for (auto it = expr_tables.begin(); it != expr_tables.end(); ++it) {
    if (from_tables.count(*it) == 0) {
      return false;
    }
  }
  return true;
}
void PushDownConditionContext::SetCondition(set<AstNode*>& equal_join_condi,
                                            set<AstNode*>& normal_condi) {
  for (int i = 0; i < sub_expr_info_.size(); ++i) {
    if (sub_expr_info_[i]->is_set == false &&
        IsTableSubSet(sub_expr_info_[i]->ref_table_, from_tables_)) {
      if (kIsEqualCondition == sub_expr_info_[i]->sub_expr_type_) {
        equal_join_condi.insert(sub_expr_info_[i]->sub_expr_);
      } else {
        normal_condi.insert(sub_expr_info_[i]->sub_expr_);
      }
      sub_expr_info_[i]->is_set = true;
    }
  }
}
bool PushDownConditionContext::IsEqualJoinCondition(AstNode* sub_expr) {
  if (sub_expr->ast_node_type() == AST_EXPR_CMP_BINARY) {
    AstExprCmpBinary* cmp_expr = reinterpret_cast<AstExprCmpBinary*>(sub_expr);
    if (cmp_expr->arg0_->ast_node_type() == AST_COLUMN &&
        cmp_expr->arg1_->ast_node_type() == AST_COLUMN &&
        cmp_expr->expr_type_ == "=") {
      return true;
    }
  }
  return false;
}
SubExprType PushDownConditionContext::GetSubExprType(AstNode* sub_expr,
                                                     int ref_table_num) {
  if (ref_table_num == 0) {
    return kNoneTable;
  } else if (ref_table_num == 1) {
    return kSingleTable;
  } else if (ref_table_num == 2 && IsEqualJoinCondition(sub_expr)) {
    return kIsEqualCondition;
  }
  return kMoreTable;
}

void PushDownConditionContext::GetSubExprInfo(AstNode* expr) {
  vector<AstNode*> sub_expr_;
  sub_expr_.clear();
  expr->GetSubExpr(sub_expr_, true);
  set<string> ref_table;
  SubExprType sub_expr_type;
  for (int i = 0; i < sub_expr_.size(); ++i) {
    ref_table.clear();
    sub_expr_[i]->GetRefTable(ref_table);
    sub_expr_type = GetSubExprType(sub_expr_[i], ref_table.size());
    sub_expr_info_.push_back(
        new SubExprInfo(sub_expr_[i], ref_table, sub_expr_type));
  }
}
//}  // namespace sql_parser
//}  // namespace claims
