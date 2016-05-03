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
 *  /sql_parser/src/astnode/ast_drop_stmt.cpp
 *
 * ./sql_parser/ast_node/ast_expr_node.cpp
 *  Created on: May 22, 2015 11:36:17 AM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 */

#include <assert.h>
#include <glog/logging.h>
#include <iostream>  //  NOLINT
#include <iomanip>
#include <string>
#include <set>
#include <vector>
#include <bitset>
#include "../ast_node/ast_expr_node.h"
#include "./ast_select_stmt.h"
#include "../../common/data_type.h"
#include "../../common/expression/expr_node.h"
#include "../../common/expression/type_conversion_matrix.h"
#include "../../common/expression/expr_const.h"
#include "../../common/expression/expr_binary.h"
#include "../../common/expression/expr_case_when.h"
#include "../../common/expression/expr_date.h"
#include "../../common/expression/expr_in.h"
#include "../../common/expression/expr_ternary.h"
#include "../../common/expression/expr_unary.h"
#include "../../common/error_define.h"
using namespace claims::common;  // NOLINT

using claims::common::ExprBinary;
using claims::common::ExprCaseWhen;
using claims::common::ExprNodeType;
using claims::common::ExprConst;
using claims::common::ExprDate;
using claims::common::ExprIn;
using claims::common::ExprTernary;
using claims::common::ExprUnary;
using claims::common::OperType;
using claims::common::TypeConversionMatrix;
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::setw;
using std::bitset;
// namespace claims {
// namespace sql_parser {

AstExprConst::AstExprConst(AstNodeType ast_node_type, string expr_type,
                           string data)
    : AstNode(ast_node_type), expr_type_(expr_type), data_(data) {}
AstExprConst::AstExprConst(AstExprConst* node)
    : AstNode(node), expr_type_(node->expr_type_), data_(node->data_) {}
AstExprConst::~AstExprConst() {}
void AstExprConst::Print(int level) const {
  cout << setw(level++ * TAB_SIZE) << " "
       << "|const|" << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "const expr type: " << expr_type_ << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "const data: " << data_ << endl;
}
AstNode* AstExprConst::AstNodeCopy() { return new AstExprConst(this); }

// TODO(FZH) be strict to the type of const
RetCode AstExprConst::SemanticAnalisys(SemanticContext* sem_cnxt) {
  RetCode ret = rSuccess;
  if (expr_type_ == "CONST_INT") {
  } else if (expr_type_ == "CONST_DOUBLE") {
  } else if (expr_type_ == "CONST_STRING") {
  } else if (expr_type_ == "CONST_BOOL") {
  }
  return ret;
}
void AstExprConst::RecoverExprName(string& name) {
  name = data_;
  return;
}
void AstExprConst::ReplaceAggregation(AstNode*& agg_column,
                                      set<AstNode*>& agg_node,
                                      bool need_collect) {
  agg_column = NULL;
}

RetCode AstExprConst::GetLogicalPlan(ExprNode*& logic_expr,
                                     LogicalOperator* const left_lplan,
                                     LogicalOperator* const right_lplan) {
  data_type actual_type = t_string;
  if (expr_type_ == "CONST_INT") {
    if (atol(data_.c_str()) > INT_MAX) {
      actual_type = t_u_long;
    } else {
      actual_type = t_int;
    }
  } else if (expr_type_ == "CONST_BOOL") {
    actual_type = t_boolean;
  } else if (expr_type_ == "CONST_STRING") {
    actual_type = t_string;
  } else if (expr_type_ == "CONST_DOUBLE") {
    actual_type = t_double;
  } else if (expr_type_ == "CONST") {
    actual_type = t_string;
  } else {
    LOG(ERROR) << "no actual_type!" << endl;
    return rNoDataTypeInConst;
  }
  logic_expr = new ExprConst(ExprNodeType::t_qexpr, actual_type, data_, data_);
  return rSuccess;
}

AstExprUnary::AstExprUnary(AstNodeType ast_node_type, string expr_type,
                           AstNode* arg0)
    : AstNode(ast_node_type), expr_type_(expr_type), arg0_(arg0) {}
AstExprUnary::AstExprUnary(AstExprUnary* node)
    : AstNode(node), expr_type_(node->expr_type_) {
  if (NULL != node->arg0_) {
    arg0_ = node->arg0_->AstNodeCopy();
  } else {
    arg0_ = NULL;
  }
}
AstExprUnary::~AstExprUnary() { delete arg0_; }

void AstExprUnary::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|expr unary| " << expr_type_ << " @ " << expr_str_ << endl;
  if (arg0_ != NULL) {
    arg0_->Print(level + 1);
  }
}

AstNode* AstExprUnary::AstNodeCopy() { return new AstExprUnary(this); }

RetCode AstExprUnary::SemanticAnalisys(SemanticContext* sem_cnxt) {
  // agg couldn't in where or groupby
  if (expr_type_ == "SUM" || expr_type_ == "MAX" || expr_type_ == "MIN" ||
      expr_type_ == "AVG" || expr_type_ == "COUNT" ||
      expr_type_ == "COUNT_ALL") {
    if (SemanticContext::kWhereClause == sem_cnxt->clause_type_) {
      ELOG(rAggCouldNotInWhereClause, "");
      return rAggCouldNotInWhereClause;
    }
    if (SemanticContext::kGroupByClause == sem_cnxt->clause_type_) {
      ELOG(rAggCouldNotInGroupByClause, "");
      return rAggCouldNotInGroupByClause;
    }
  }
  if (expr_type_ == "COUNT_ALL") {
    sem_cnxt->select_expr_have_agg = true;

    expr_str_ = "COUNT(*)";
    return rSuccess;
  }
  if (NULL != arg0_) {
    // upper node have agg and this node is agg, then error
    bool here_have_agg =
        (expr_type_ == "SUM" || expr_type_ == "MAX" || expr_type_ == "MIN" ||
         expr_type_ == "AVG" || expr_type_ == "COUNT");

    if (sem_cnxt->have_agg && here_have_agg) {
      ELOG(rAggHaveAgg, "");
      return rAggHaveAgg;
    }
    sem_cnxt->have_agg = here_have_agg;
    if (sem_cnxt->have_agg) {
      sem_cnxt->select_expr_have_agg = true;
    }
    RetCode ret = arg0_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
    sem_cnxt->have_agg = false;
    return rSuccess;
  }
  ELOG(rUnaryArgNotExist, "");
  return rUnaryArgNotExist;
}

void AstExprUnary::RecoverExprName(string& name) {
  if (expr_type_ == "COUNT_ALL") {
    expr_str_ = "COUNT(*)";
    name = expr_str_;
    return;
  }
  string arg_name;
  if (NULL != arg0_) {
    arg0_->RecoverExprName(arg_name);
    if (expr_str_ == "+" || expr_str_ == "-" || expr_str_ == "!" ||
        expr_str_ == "NOT") {
      expr_str_ = expr_type_ + arg_name;
    } else if (expr_str_.substr(0, 2) == "IS") {
      expr_str_ = arg_name + " " + expr_type_;
    } else {
      expr_str_ = expr_type_ + "(" + arg_name + ")";
    }
    name = expr_str_;
  }
  return;
}

void AstExprUnary::ReplaceAggregation(AstNode*& agg_column,
                                      set<AstNode*>& agg_node,
                                      bool need_collect) {
  // like a leaf node
  if (expr_type_ == "COUNT_ALL" || expr_type_ == "SUM" || expr_type_ == "MAX" ||
      expr_type_ == "MIN" || expr_type_ == "AVG" || expr_type_ == "COUNT") {
    if (need_collect) {
      agg_node.insert(this);
    }
    agg_column = new AstColumn(AST_COLUMN, "NULL_MID", expr_str_, expr_str_);
    return;
  } else {
    agg_column = NULL;
    arg0_->ReplaceAggregation(agg_column, agg_node, need_collect);
    if (NULL != agg_column) {
      if (!need_collect) {
        delete arg0_;
      }
      arg0_ = agg_column;
    }
    agg_column = NULL;
  }
  return;
}
void AstExprUnary::GetRefTable(set<string>& ref_table) {
  if (NULL != arg0_) {
    arg0_->GetRefTable(ref_table);
  }
}

RetCode AstExprUnary::GetLogicalPlan(ExprNode*& logic_expr,
                                     LogicalOperator* const left_lplan,
                                     LogicalOperator* const right_lplan) {
  data_type get_type, actual_type;
  OperType oper;
  oper = OperType::oper_none;
  if (expr_type_ == "+") {
  } else if (expr_type_ == "-") {
    oper = OperType::oper_negative;
  } else if (expr_type_ == "!" || expr_type_ == "NOT") {
    oper = OperType::oper_not;
  } else if (expr_type_ == "IS_NULL") {
  } else if (expr_type_ == "IS_NOT_NULL") {
  } else if (expr_type_ == "IS_BOOL") {
  } else if (expr_type_ == "IS_NOT_BOOL") {
  } else if (expr_type_ == "EXSIST") {
  } else if (expr_type_ == "NOT_EXSIST") {
  } else if (expr_type_ == "COUNT_ALL") {
    oper = OperType::oper_agg_count;
  } else if (expr_type_ == "COUNT") {
    oper = OperType::oper_agg_count;

  } else if (expr_type_ == "MAX") {
    oper = OperType::oper_max;

  } else if (expr_type_ == "MIN") {
    oper = OperType::oper_min;

  } else if (expr_type_ == "AVG") {
    oper = OperType::oper_agg_avg;

  } else if (expr_type_ == "SUM") {
    oper = OperType::oper_agg_sum;
  }
  if (oper == OperType::oper_none) {
    LOG(ERROR) << "not support now!" << endl;
    assert(false);
  }
  ExprNode* child_logic_expr = NULL;
  RetCode ret = rSuccess;
  // count(*) = count(1)
  if (expr_type_ == "COUNT_ALL" || expr_type_ == "COUNT") {
    child_logic_expr =
        new ExprConst(ExprNodeType::t_qexpr, t_u_long, "COUNT(1)", "1");
  } else {
    ret = arg0_->GetLogicalPlan(child_logic_expr, left_lplan, right_lplan);
  }
  if (rSuccess != ret) {
    return ret;
  }
  assert(NULL != child_logic_expr);
  logic_expr =
      new ExprUnary(ExprNodeType::t_qexpr_unary, child_logic_expr->actual_type_,
                    expr_str_, oper, child_logic_expr);
  return rSuccess;
}
RetCode AstExprUnary::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  if (NULL != arg0_) {
    arg0_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(arg0_);
    select_alias_solver->DeleteOldNode();
  }
  return rSuccess;
}
AstExprCalBinary::AstExprCalBinary(AstNodeType ast_node_type,
                                   std::string expr_type, AstNode* arg0,
                                   AstNode* arg1)
    : AstNode(ast_node_type), expr_type_(expr_type), arg0_(arg0), arg1_(arg1) {}
AstExprCalBinary::AstExprCalBinary(AstExprCalBinary* node)
    : AstNode(node), expr_type_(node->expr_type_) {
  if (NULL != node->arg0_) {
    arg0_ = node->arg0_->AstNodeCopy();
  } else {
    arg0_ = NULL;
  }
  if (NULL != node->arg1_) {
    arg1_ = node->arg1_->AstNodeCopy();
  } else {
    arg1_ = NULL;
  }
}
AstExprCalBinary::~AstExprCalBinary() {
  delete arg0_;
  delete arg1_;
}
AstNode* AstExprCalBinary::AstNodeCopy() { return new AstExprCalBinary(this); }

void AstExprCalBinary::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|expr binary| " << expr_type_ << " @ " << expr_str_ << endl;
  if (arg0_ != NULL) arg0_->Print(level + 1);
  if (arg1_ != NULL) arg1_->Print(level + 1);
}
RetCode AstExprCalBinary::SemanticAnalisys(SemanticContext* sem_cnxt) {
  RetCode ret = rSuccess;
  if (NULL != arg0_) {
    ret = arg0_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != arg1_) {
    ret = arg1_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }

  return rSuccess;
}
// cal>cmp>bool
// if right child have the same level, add ()
void AstExprCalBinary::RecoverExprName(string& name) {
  expr_str_ = "";
  string arg0_name = "";
  string arg1_name = "";
  if (NULL != arg0_) {
    arg0_->RecoverExprName(arg0_name);
    if ((ast_node_type_ == AST_EXPR_CAL_BINARY &&
         (arg0_->ast_node_type() == AST_EXPR_BOOL_BINARY ||
          arg0_->ast_node_type() == AST_EXPR_CMP_BINARY)) ||
        (ast_node_type_ == AST_EXPR_CMP_BINARY &&
         arg0_->ast_node_type() == AST_EXPR_BOOL_BINARY)) {
      arg0_name = "(" + arg0_name + ")";
    }
    expr_str_ = arg0_name;
  }
  if (ast_node_type_ == AST_EXPR_BOOL_BINARY) {
    expr_str_ = expr_str_ + " " + expr_type_ + " ";
  } else {
    expr_str_ = expr_str_ + expr_type_;
  }
  if (NULL != arg1_) {
    arg1_->RecoverExprName(arg1_name);
    if ((ast_node_type_ == AST_EXPR_CAL_BINARY &&
         (arg1_->ast_node_type() == AST_EXPR_BOOL_BINARY ||
          arg1_->ast_node_type() == AST_EXPR_CMP_BINARY ||
          arg1_->ast_node_type() == AST_EXPR_CAL_BINARY)) ||
        (ast_node_type_ == AST_EXPR_CMP_BINARY &&
         (arg1_->ast_node_type() == AST_EXPR_BOOL_BINARY ||
          arg1_->ast_node_type() == AST_EXPR_CMP_BINARY)) ||
        (ast_node_type_ == AST_EXPR_BOOL_BINARY &&
         arg1_->ast_node_type() == AST_EXPR_BOOL_BINARY)) {
      arg1_name = "(" + arg1_name + ")";
    }
    expr_str_ = expr_str_ + arg1_name;
  }
  name = expr_str_;
  return;
}

void AstExprCalBinary::ReplaceAggregation(AstNode*& agg_column,
                                          set<AstNode*>& agg_node,
                                          bool need_collect) {
  if (NULL != arg0_) {
    agg_column = NULL;
    arg0_->ReplaceAggregation(agg_column, agg_node, need_collect);
    // this agg shouldn't delete in select clause, because they are collected
    // for build aggregation
    if (NULL != agg_column) {
      if (!need_collect) {
        delete arg0_;
      }
      arg0_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != arg1_) {
    agg_column = NULL;
    arg1_->ReplaceAggregation(agg_column, agg_node, need_collect);
    if (NULL != agg_column) {
      if (!need_collect) {
        delete arg1_;
      }
      arg1_ = agg_column;
    }
    agg_column = NULL;
  }
}

void AstExprCalBinary::GetSubExpr(vector<AstNode*>& sub_expr, bool is_top_and) {
  if (is_top_and && expr_type_ == "AND") {
    arg0_->GetSubExpr(sub_expr, is_top_and);
    arg1_->GetSubExpr(sub_expr, is_top_and);
  } else {
    assert(expr_type_ != "AND");
    is_top_and = false;
    sub_expr.push_back(this);
  }
}
void AstExprCalBinary::GetRefTable(set<string>& ref_table) {
  if (NULL != arg0_) {
    arg0_->GetRefTable(ref_table);
  }
  if (NULL != arg1_) {
    arg1_->GetRefTable(ref_table);
  }
}

RetCode AstExprCalBinary::GetLogicalPlan(ExprNode*& logic_expr,
                                         LogicalOperator* const left_lplan,
                                         LogicalOperator* const right_lplan) {
  ExprNode* left_expr_node = NULL;
  ExprNode* right_expr_node = NULL;
  RetCode ret = rSuccess;
  ret = arg0_->GetLogicalPlan(left_expr_node, left_lplan, right_lplan);
  if (rSuccess != ret) {
    return ret;
  }
  ret = arg1_->GetLogicalPlan(right_expr_node, left_lplan, right_lplan);
  if (rSuccess != ret) {
    return ret;
  }
  data_type get_type = TypeConversionMatrix::type_conversion_matrix
      [left_expr_node->actual_type_][right_expr_node->actual_type_];
  data_type actual_type = get_type;
  if (ast_node_type_ == AST_EXPR_BOOL_BINARY) {
    get_type = t_boolean;
    actual_type = t_boolean;
  } else {
    if (left_expr_node->actual_type_ == t_boolean &&
        right_expr_node->actual_type_ == t_boolean) {
      get_type = t_smallInt;
      actual_type = t_smallInt;
    }
  }
  OperType oper = OperType::oper_equal;
  if (expr_type_ == "+") {
    oper = OperType::oper_add;
  } else if (expr_type_ == "-") {
    oper = OperType::oper_minus;
  } else if (expr_type_ == "*") {
    oper = OperType::oper_multiply;
  } else if (expr_type_ == "/") {
    oper = OperType::oper_divide;
  } else if (expr_type_ == "%" || expr_type_ == "MOD") {
    oper = OperType::oper_mod;
  } else if (expr_type_ == "AND") {
    oper = OperType::oper_and;
  } else if (expr_type_ == "OR") {
    oper = OperType::oper_or;
  } else if (expr_type_ == "XOR") {
    oper = OperType::oper_xor;
  } else if (expr_type_ == "|") {
    oper = OperType::oper_or;
  } else if (expr_type_ == "^") {
    oper = OperType::oper_xor;
  } else if (expr_type_ == "LIKE") {
    oper = OperType::oper_like;
    actual_type = t_boolean;
  } else if (expr_type_ == "NOT_LIKE") {
    oper = OperType::oper_not_like;
    actual_type = t_boolean;
  } else {
    oper = OperType::oper_none;
  }
  logic_expr = new ExprBinary(ExprNodeType::t_qexpr_cal, actual_type, get_type,
                              expr_str_, oper, left_expr_node, right_expr_node);
  return rSuccess;
}

RetCode AstExprCalBinary::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  if (NULL != arg0_) {
    arg0_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(arg0_);
    select_alias_solver->DeleteOldNode();
  }
  if (NULL != arg1_) {
    arg1_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(arg1_);
    select_alias_solver->DeleteOldNode();
  }
  return rSuccess;
}
AstExprCmpBinary::AstExprCmpBinary(AstNodeType ast_node_type, string expr_type,
                                   AstNode* arg0, AstNode* arg1)
    : AstNode(ast_node_type),
      expr_type_(expr_type),
      cmp_para_(""),
      arg0_(arg0),
      arg1_(arg1) {}
AstExprCmpBinary::AstExprCmpBinary(AstNodeType ast_node_type, string cmp_para,
                                   int cmp_type, AstNode* arg0, AstNode* arg1)
    : AstNode(ast_node_type), cmp_para_(cmp_para), arg0_(arg0), arg1_(arg1) {
  switch (cmp_type) {
    case 1: {
      expr_type_ = "<";
      break;
    }
    case 2: {
      expr_type_ = ">";
      break;
    }
    case 3: {
      expr_type_ = "!=";
      break;
    }
    case 4: {
      expr_type_ = "=";
      break;
    }
    case 5: {
      expr_type_ = "<=";
      break;
    }
    case 6: {
      expr_type_ = ">=";
      break;
    }
    case 12: {
      expr_type_ = "<=>";
      break;
    }
    default: {
      expr_type_ = "error type";
      assert(false);
    }
  }
}
AstExprCmpBinary::AstExprCmpBinary(AstExprCmpBinary* node)
    : AstNode(node), expr_type_(node->expr_type_), cmp_para_(node->cmp_para_) {
  if (NULL != node->arg0_) {
    arg0_ = node->arg0_->AstNodeCopy();
  } else {
    arg0_ = NULL;
  }
  if (NULL != node->arg1_) {
    arg1_ = node->arg1_->AstNodeCopy();
  } else {
    arg1_ = NULL;
  }
}
AstExprCmpBinary::~AstExprCmpBinary() {
  delete arg0_;
  delete arg1_;
}
AstNode* AstExprCmpBinary::AstNodeCopy() { return new AstExprCmpBinary(this); }

void AstExprCmpBinary::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|expr binary| " << expr_type_ << " @ " << expr_str_ << endl;
  if (arg0_ != NULL) arg0_->Print(level + 1);
  if (arg1_ != NULL) arg1_->Print(level + 1);
}
RetCode AstExprCmpBinary::SemanticAnalisys(SemanticContext* sem_cnxt) {
  RetCode ret = rSuccess;
  if (expr_type_ == "EXPR_IN_SELECT") {
    ret = claims::common::rNotSupport;
    return ret;
  }
  if (NULL != arg0_) {
    ret = arg0_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != arg1_) {
    ret = arg1_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }

  return rSuccess;
}
void AstExprCmpBinary::RecoverExprName(string& name) {
  expr_str_ = "";
  string arg0_name = "";
  string arg1_name = "";
  if (NULL != arg0_) {
    arg0_->RecoverExprName(arg0_name);
    expr_str_ = arg0_name;
  }
  expr_str_ = expr_str_ + expr_type_;
  if (NULL != arg1_) {
    arg1_->RecoverExprName(arg1_name);
    expr_str_ = expr_str_ + arg1_name;
  }
  name = expr_str_;
  return;
}
void AstExprCmpBinary::ReplaceAggregation(AstNode*& agg_column,
                                          set<AstNode*>& agg_node,
                                          bool need_collect) {
  if (NULL != arg0_) {
    agg_column = NULL;
    arg0_->ReplaceAggregation(agg_column, agg_node, need_collect);
    if (NULL != agg_column) {
      if (!need_collect) {
        delete arg0_;
      }
      arg0_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != arg1_) {
    agg_column = NULL;
    arg1_->ReplaceAggregation(agg_column, agg_node, need_collect);
    if (NULL != agg_column) {
      if (!need_collect) {
        delete arg1_;
      }
      arg1_ = agg_column;
    }
    agg_column = NULL;
  }
}
void AstExprCmpBinary::GetRefTable(set<string>& ref_table) {
  if (NULL != arg0_) {
    arg0_->GetRefTable(ref_table);
  }
  if (NULL != arg1_) {
    arg1_->GetRefTable(ref_table);
  }
}

RetCode AstExprCmpBinary::GetLogicalPlan(ExprNode*& logic_expr,
                                         LogicalOperator* const left_lplan,
                                         LogicalOperator* const right_lplan) {
  ExprNode* left_expr_node = NULL;
  ExprNode* right_expr_node = NULL;
  data_type get_type = t_boolean;
  OperType oper = OperType::oper_none;
  RetCode ret = rSuccess;
  if (expr_type_ == "<") {
    oper = OperType::oper_less;
  } else if (expr_type_ == ">") {
    oper = OperType::oper_great;
  } else if (expr_type_ == "=") {
    oper = OperType::oper_equal;
  } else if (expr_type_ == ">=") {
    oper = OperType::oper_great_equal;
  } else if (expr_type_ == "<=") {
    oper = OperType::oper_less_equal;
  } else if (expr_type_ == "!=") {
    oper = OperType::oper_not_equal;
  } else if (expr_type_ == "<=>") {
    oper = OperType::oper_not_equal;
  }
  if (OperType::oper_none != oper) {
    ret = arg0_->GetLogicalPlan(left_expr_node, left_lplan, right_lplan);
    if (rSuccess != ret) {
      return ret;
    }
    ret = arg1_->GetLogicalPlan(right_expr_node, left_lplan, right_lplan);
    if (rSuccess != ret) {
      return ret;
    }
    get_type = TypeConversionMatrix::type_conversion_matrix
        [left_expr_node->actual_type_][right_expr_node->actual_type_];
    logic_expr =
        new ExprBinary(ExprNodeType::t_qexpr_cmp, t_boolean, get_type,
                       expr_str_, oper, left_expr_node, right_expr_node);
    return ret;
  }
  if (expr_type_ == "EXPR_IN_LIST") {
    // e.g. a in (2,3,4)
    vector<ExprNode*> left_node, tmp_node;
    vector<vector<ExprNode*> > right_node;
    // just one expr at left
    ret = arg0_->GetLogicalPlan(left_expr_node, left_lplan, right_lplan);
    if (rSuccess != ret) {
      return ret;
    }
    left_node.push_back(left_expr_node);
    // one column per tuple
    for (AstNode* lnode = arg1_; lnode != NULL;) {
      AstExprList* list_node = reinterpret_cast<AstExprList*>(lnode);
      ret = list_node->expr_->GetLogicalPlan(right_expr_node, left_lplan,
                                             right_lplan);
      if (rSuccess != ret) {
        return ret;
      }
      tmp_node.push_back(right_expr_node);
      right_node.push_back(tmp_node);
      tmp_node.clear();
      lnode = list_node->next_;
    }
    // construct equal expression per column at left
    vector<ExprBinary*> cmp_node;
    for (int i = 0; i < left_node.size(); ++i) {
      get_type = TypeConversionMatrix::type_conversion_matrix
          [left_node[i]->actual_type_][right_node[0][i]->actual_type_];
      ExprBinary* cmp_expr = new ExprBinary(
          ExprNodeType::t_qexpr_cmp, t_boolean, get_type, "in_equal_node",
          OperType::oper_equal, left_node[i], right_node[0][i]);
      cmp_node.push_back(cmp_expr);
    }
    logic_expr = new ExprIn(ExprNodeType::t_qexpr_in, t_boolean, expr_str_,
                            cmp_node, right_node);
  } else if (expr_type_ == "LIST_IN_LIST") {
    // e.g. (a,b) in ((1,2.3)(2,4.3))
    vector<ExprNode*> left_node, tmp_node;
    vector<vector<ExprNode*> > right_node;
    // collect several left expressions
    for (AstNode* lnode = arg0_; lnode != NULL;) {
      AstExprList* list_node = reinterpret_cast<AstExprList*>(lnode);
      ret = list_node->expr_->GetLogicalPlan(left_expr_node, left_lplan,
                                             right_lplan);
      if (rSuccess != ret) {
        return ret;
      }
      left_node.push_back(left_expr_node);
      lnode = list_node->next_;
    }
    // more columns in one tuple
    for (AstNode* lnode = arg1_; lnode != NULL;) {
      AstExprList* list_node = reinterpret_cast<AstExprList*>(lnode);
      for (AstNode* llnode = list_node->expr_; llnode != NULL;) {
        AstExprList* llist_node = reinterpret_cast<AstExprList*>(llnode);
        ret = llist_node->expr_->GetLogicalPlan(right_expr_node, left_lplan,
                                                right_lplan);
        if (rSuccess != ret) {
          return ret;
        }
        tmp_node.push_back(right_expr_node);
        llnode = llist_node->next_;
      }
      right_node.push_back(tmp_node);
      tmp_node.clear();
      lnode = list_node->next_;
    }
    // construct equal expression per column at left
    vector<ExprBinary*> cmp_node;
    for (int i = 0; i < left_node.size(); ++i) {
      get_type = TypeConversionMatrix::type_conversion_matrix
          [left_node[i]->actual_type_][right_node[0][i]->actual_type_];
      ExprBinary* cmp_expr = new ExprBinary(
          ExprNodeType::t_qexpr_cmp, t_boolean, get_type, "in_equal_node",
          OperType::oper_equal, left_node[i], right_node[0][i]);
      cmp_node.push_back(cmp_expr);
    }
    logic_expr = new ExprIn(ExprNodeType::t_qexpr_in, t_boolean, expr_str_,
                            cmp_node, right_node);
  } else {
    LOG(WARNING) << "the expression isn't support now!" << endl;
    assert(false);
  }
  return rSuccess;
}

RetCode AstExprCmpBinary::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  if (NULL != arg0_) {
    arg0_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(arg0_);
    select_alias_solver->DeleteOldNode();
  }
  if (NULL != arg1_) {
    arg1_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(arg1_);
    select_alias_solver->DeleteOldNode();
  }
  return rSuccess;
}

AstExprList::AstExprList(AstNodeType ast_node_type, AstNode* expr,
                         AstNode* next)
    : AstNode(ast_node_type), expr_(expr), next_(next) {}
AstExprList::AstExprList(AstExprList* node) : AstNode(node) {
  if (NULL != node->expr_) {
    expr_ = node->expr_->AstNodeCopy();
  } else {
    expr_ = NULL;
  }
  if (NULL != node->next_) {
    next_ = node->next_->AstNodeCopy();
  } else {
    next_ = NULL;
  }
}
AstExprList::~AstExprList() {
  delete expr_;
  delete next_;
}
AstNode* AstExprList::AstNodeCopy() { return new AstExprList(this); }

void AstExprList::Print(int level) const {
  // cout << "level= " << level << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "|expr list| " << expr_str_ << endl;
  if (expr_ != NULL) expr_->Print(level);
  if (next_ != NULL) next_->Print(level);
}
RetCode AstExprList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  RetCode ret = rSuccess;
  if (NULL != expr_) {
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != next_) {
    return next_->SemanticAnalisys(sem_cnxt);
  }
  return rSuccess;
}
void AstExprList::RecoverExprName(string& name) {
  string expr_name = "";
  string next_name = "";
  if (NULL != expr_) {
    expr_->RecoverExprName(expr_name);
    expr_str_ = expr_name;
  }
  if (NULL != next_) {
    next_->RecoverExprName(next_name);
    expr_str_ = expr_str_ + "," + next_name;
  }
  name = expr_str_;
  return;
}
void AstExprList::ReplaceAggregation(AstNode*& agg_column,
                                     set<AstNode*>& agg_node,
                                     bool need_collect) {
  if (NULL != expr_) {
    agg_column = NULL;
    expr_->ReplaceAggregation(agg_column, agg_node, need_collect);
    if (NULL != agg_column) {
      if (!need_collect) {
        delete expr_;
      }
      expr_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != next_) {
    agg_column = NULL;
    next_->ReplaceAggregation(agg_column, agg_node, need_collect);
    if (NULL != agg_column) {
      if (!need_collect) {
        delete next_;
      }
      next_ = agg_column;
    }
    agg_column = NULL;
  }
}
void AstExprList::GetRefTable(set<string>& ref_table) {
  if (NULL != expr_) {
    expr_->GetRefTable(ref_table);
  }
  if (NULL != next_) {
    next_->GetRefTable(ref_table);
  }
}

RetCode AstExprList::SolveSelectAlias(
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
AstExprFunc::AstExprFunc(AstNodeType ast_node_type, std::string expr_type,
                         AstNode* arg0, AstNode* arg1, AstNode* arg2)
    : AstNode(ast_node_type),
      expr_type_(expr_type),
      arg0_(arg0),
      arg1_(arg1),
      arg2_(arg2) {}
AstExprFunc::AstExprFunc(AstExprFunc* node)
    : AstNode(node), expr_type_(node->expr_type_) {
  if (NULL != node->arg0_) {
    arg0_ = node->arg0_->AstNodeCopy();
  } else {
    arg0_ = NULL;
  }
  if (NULL != node->arg1_) {
    arg1_ = node->arg1_->AstNodeCopy();
  } else {
    arg1_ = NULL;
  }
  if (NULL != node->arg2_) {
    arg2_ = node->arg2_->AstNodeCopy();
  } else {
    arg2_ = NULL;
  }
}
AstExprFunc::~AstExprFunc() {
  delete arg0_;
  delete arg1_;
  delete arg2_;
}
AstNode* AstExprFunc::AstNodeCopy() { return new AstExprFunc(this); }

void AstExprFunc::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|expr function| " << expr_type_ << " @ " << expr_str_ << endl;
  if (arg0_ != NULL) arg0_->Print(level + 1);
  if (arg1_ != NULL) arg1_->Print(level + 2);
  if (arg2_ != NULL) arg2_->Print(level + 3);
}
RetCode AstExprFunc::SemanticAnalisys(SemanticContext* sem_cnxt) {
  RetCode ret = rSuccess;
  if (NULL != arg0_) {
    ret = arg0_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != arg1_) {
    ret = arg1_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  if (NULL != arg2_) {
    ret = arg2_->SemanticAnalisys(sem_cnxt);
    if (rSuccess != ret) {
      return ret;
    }
  }
  return rSuccess;
}

void AstExprFunc::RecoverExprName(string& name) {
  string arg0_name = "";
  string arg1_name = "";
  string arg2_name = "";
  if (expr_type_.substr(0, 4) == "SUBS") {
    expr_str_ = "SUBSTR(";
  } else if (expr_type_.substr(0, 4) == "TRIM") {
    expr_str_ = "TRIM(";
  } else {
    expr_str_ = expr_type_ + "(";
  }
  if (NULL != arg0_) {
    arg0_->RecoverExprName(arg0_name);
    expr_str_ = expr_str_ + arg0_name;
  }
  if (NULL != arg1_) {
    arg1_->RecoverExprName(arg1_name);
    if (NULL == arg0_) {
      expr_str_ = expr_str_ + arg1_name;
    } else {
      expr_str_ = expr_str_ + "," + arg1_name;
    }
  }
  if (NULL != arg2_) {
    arg2_->RecoverExprName(arg2_name);
    if (NULL == arg0_ && NULL == arg1_) {
      expr_str_ = expr_str_ + arg2_name;
    } else {
      expr_str_ = expr_str_ + "," + arg2_name;
    }
  }
  expr_str_ = expr_str_ + ")";
  if (expr_type_ == "BETWEEN_AND") {
    expr_str_ = arg0_name + " BETWEEN " + arg1_name + " AND " + arg2_name;
  }
  name = expr_str_;
  return;
}
void AstExprFunc::ReplaceAggregation(AstNode*& agg_column,
                                     set<AstNode*>& agg_node,
                                     bool need_collect) {
  if (NULL != arg0_) {
    agg_column = NULL;
    arg0_->ReplaceAggregation(agg_column, agg_node, need_collect);
    if (NULL != agg_column) {
      if (!need_collect) {
        delete arg0_;
      }
      arg0_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != arg1_) {
    agg_column = NULL;
    arg1_->ReplaceAggregation(agg_column, agg_node, need_collect);
    if (NULL != agg_column) {
      if (!need_collect) {
        delete arg1_;
      }
      arg1_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != arg2_) {
    agg_column = NULL;
    arg2_->ReplaceAggregation(agg_column, agg_node, need_collect);
    if (NULL != agg_column) {
      if (!need_collect) {
        delete arg2_;
      }
      arg2_ = agg_column;
    }
    agg_column = NULL;
  }
}
void AstExprFunc::GetRefTable(set<string>& ref_table) {
  if (NULL != arg0_) {
    arg0_->GetRefTable(ref_table);
  }
  if (NULL != arg1_) {
    arg1_->GetRefTable(ref_table);
  }
  if (NULL != arg2_) {
    arg2_->GetRefTable(ref_table);
  }
}

RetCode AstExprFunc::GetLogicalPlan(ExprNode*& logic_expr,
                                    LogicalOperator* const left_lplan,
                                    LogicalOperator* const right_lplan) {
  ExprNode* arg0_logic_expr = NULL;
  ExprNode* arg1_logic_expr = NULL;
  ExprNode* arg2_logic_expr = NULL;

  if (expr_type_ == "UPPER") {
    if (NULL != arg0_) {
      arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg1_) {
      arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg2_) {
      arg2_->GetLogicalPlan(arg2_logic_expr, left_lplan, right_lplan);
    }
    logic_expr = new ExprUnary(ExprNodeType::t_qexpr_unary, t_string, expr_str_,
                               OperType::oper_upper, arg0_logic_expr);
  } else if (expr_type_ == "SUBSTRING_EXPR_EXPR" ||
             expr_type_ == "SUBSTRING_EXPR_FROM_EXPR") {
    if (NULL != arg0_) {
      arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg1_) {
      arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg2_) {
      arg2_->GetLogicalPlan(arg2_logic_expr, left_lplan, right_lplan);
    }
    arg2_logic_expr = new ExprConst(ExprNodeType::t_qexpr, t_int, string("64"),
                                    string("64"));  // 64 is the size of value
    logic_expr =
        new ExprTernary(ExprNodeType::t_qexpr_ternary, t_string, expr_str_,
                        OperType::oper_substring, arg0_logic_expr,
                        arg1_logic_expr, arg2_logic_expr);
  } else if (expr_type_ == "SUBSTRING_EXPR_EXPR_EXPR" ||
             expr_type_ == "SUBSTRING_EXPR_FROM_EXPR_FOR_EXPR") {
    if (NULL != arg0_) {
      arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg1_) {
      arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg2_) {
      arg2_->GetLogicalPlan(arg2_logic_expr, left_lplan, right_lplan);
    }
    logic_expr =
        new ExprTernary(ExprNodeType::t_qexpr_ternary, t_string, expr_str_,
                        OperType::oper_substring, arg0_logic_expr,
                        arg1_logic_expr, arg2_logic_expr);
  } else if (expr_type_ == "TRIM_TRAILING") {
    if (NULL != arg0_) {
      arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg1_) {
      arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg2_) {
      arg2_->GetLogicalPlan(arg2_logic_expr, left_lplan, right_lplan);
    }
    logic_expr = new ExprBinary(ExprNodeType::t_qexpr_cal, t_string, t_string,
                                expr_str_, OperType::oper_trailing_trim,
                                arg0_logic_expr, arg1_logic_expr);
  } else if (expr_type_ == "TRIM_LEADING") {
    if (NULL != arg0_) {
      arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg1_) {
      arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg2_) {
      arg2_->GetLogicalPlan(arg2_logic_expr, left_lplan, right_lplan);
    }
    logic_expr = new ExprBinary(ExprNodeType::t_qexpr_cal, t_string, t_string,
                                expr_str_, OperType::oper_leading_trim,
                                arg0_logic_expr, arg1_logic_expr);
  } else if (expr_type_ == "TRIM_BOTH") {
    if (NULL != arg0_) {
      arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg1_) {
      arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg2_) {
      arg2_->GetLogicalPlan(arg2_logic_expr, left_lplan, right_lplan);
    }
    if (NULL == arg0_) {
      arg0_logic_expr =
          new ExprConst(ExprNodeType::t_qexpr, t_string, string(" "),
                        string(" "));  // trim both ' '
    }
    assert(arg0_logic_expr != NULL && arg1_logic_expr != NULL);
    logic_expr = new ExprBinary(ExprNodeType::t_qexpr_cal, t_string, t_string,
                                expr_str_, OperType::oper_both_trim,
                                arg0_logic_expr, arg1_logic_expr);
  } else if (expr_type_ == "BETWEEN_AND") {
    if (NULL != arg0_) {
      arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg1_) {
      arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
    }
    if (NULL != arg2_) {
      arg2_->GetLogicalPlan(arg2_logic_expr, left_lplan, right_lplan);
    }
    data_type get_type = TypeConversionMatrix::type_conversion_matrix
        [arg0_logic_expr->actual_type_][arg1_logic_expr->actual_type_];
    ExprNode* left_node = new ExprBinary(
        ExprNodeType::t_qexpr_cmp, t_boolean, get_type, "BA_arg0>=arg1",
        OperType::oper_great_equal, arg0_logic_expr, arg1_logic_expr);
    ExprNode* right_node = new ExprBinary(
        ExprNodeType::t_qexpr_cmp, t_boolean, get_type, "BA_arg0<=arg2",
        OperType::oper_less_equal, arg0_logic_expr, arg2_logic_expr);
    logic_expr =
        new ExprBinary(ExprNodeType::t_qexpr_cal, t_boolean, t_boolean,
                       expr_str_, OperType::oper_and, left_node, right_node);
  } else if (expr_type_ == "CASE2_ELSE") {
    std::vector<ExprNode*> case_when;
    std::vector<ExprNode*> case_then;
    for (AstNode* it = arg1_; it != NULL;) {
      AstExprFunc* fnode = reinterpret_cast<AstExprFunc*>(it);
      fnode->arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
      fnode->arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
      case_when.push_back(arg0_logic_expr);
      case_then.push_back(arg1_logic_expr);
      it = fnode->arg2_;
    }
    arg2_->GetLogicalPlan(arg2_logic_expr, left_lplan, right_lplan);
    case_then.push_back(arg2_logic_expr);
    logic_expr = new ExprCaseWhen(ExprNodeType::t_qexpr_case_when,
                                  case_then[0]->actual_type_, expr_str_,
                                  case_when, case_then);
  } else if (expr_type_ == "CASE2") {
    std::vector<ExprNode*> case_when;
    std::vector<ExprNode*> case_then;
    for (AstNode* it = arg1_; it != NULL;) {
      AstExprFunc* fnode = reinterpret_cast<AstExprFunc*>(it);
      fnode->arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
      fnode->arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
      case_when.push_back(arg0_logic_expr);
      case_then.push_back(arg1_logic_expr);
      it = fnode->arg2_;
    }
    assert(arg2_ == NULL);
    assert(case_then[0]->actual_type_ == t_string);
    arg2_logic_expr = new ExprConst(ExprNodeType::t_qexpr,
                                    case_then[0]->actual_type_, "NULL", "NULL");
    case_then.push_back(arg2_logic_expr);
    logic_expr = new ExprCaseWhen(ExprNodeType::t_qexpr_case_when,
                                  case_then[0]->actual_type_, expr_str_,
                                  case_when, case_then);

  } else if (expr_type_ == "CASE1") {
    std::vector<ExprNode*> case_when;
    std::vector<ExprNode*> case_then;
    ExprNode* arg_logic_expr = NULL;
    assert(arg0_ != NULL);
    arg0_->GetLogicalPlan(arg_logic_expr, left_lplan, right_lplan);
    for (AstNode* it = arg1_; it != NULL;) {
      AstExprFunc* fnode = reinterpret_cast<AstExprFunc*>(it);
      fnode->arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
      fnode->arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
      data_type get_type = TypeConversionMatrix::type_conversion_matrix
          [arg_logic_expr->actual_type_][arg0_logic_expr->actual_type_];
      ExprBinary* tmp_node = new ExprBinary(
          ExprNodeType::t_qexpr_cal, t_boolean, get_type, "case when =",
          OperType::oper_equal, arg_logic_expr, arg0_logic_expr);
      case_when.push_back(tmp_node);
      case_then.push_back(arg1_logic_expr);
      it = fnode->arg2_;
    }
    assert(arg2_ == NULL);
    assert(case_then[0]->actual_type_ == t_string);
    arg2_logic_expr = new ExprConst(ExprNodeType::t_qexpr,
                                    case_then[0]->actual_type_, "NULL", "NULL");
    case_then.push_back(arg2_logic_expr);
    logic_expr = new ExprCaseWhen(ExprNodeType::t_qexpr_case_when,
                                  case_then[0]->actual_type_, expr_str_,
                                  case_when, case_then);
  } else if (expr_type_ == "CASE1_ELSE") {
    std::vector<ExprNode*> case_when;
    std::vector<ExprNode*> case_then;
    ExprNode* arg_logic_expr = NULL;
    assert(arg0_ != NULL);
    arg0_->GetLogicalPlan(arg_logic_expr, left_lplan, right_lplan);
    for (AstNode* it = arg1_; it != NULL;) {
      AstExprFunc* fnode = reinterpret_cast<AstExprFunc*>(it);
      fnode->arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
      fnode->arg1_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
      data_type get_type = TypeConversionMatrix::type_conversion_matrix
          [arg_logic_expr->actual_type_][arg0_logic_expr->actual_type_];
      ExprBinary* tmp_node = new ExprBinary(
          ExprNodeType::t_qexpr_cal, t_boolean, get_type, "case when =",
          OperType::oper_equal, arg_logic_expr, arg0_logic_expr);
      case_when.push_back(tmp_node);
      case_then.push_back(arg1_logic_expr);
      it = fnode->arg2_;
    }
    arg2_->GetLogicalPlan(arg2_logic_expr, left_lplan, right_lplan);
    case_then.push_back(arg2_logic_expr);
    logic_expr = new ExprCaseWhen(ExprNodeType::t_qexpr_case_when,
                                  case_then[0]->actual_type_, expr_str_,
                                  case_when, case_then);

  } else if (expr_type_ == "DATE_ADD") {
    arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
    AstExprFunc* fnode = reinterpret_cast<AstExprFunc*>(arg1_);
    fnode->arg0_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
    if (fnode->expr_type_ == "INTERVAL_DAY") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_day, expr_str_,
          OperType::oper_date_add_day, arg0_logic_expr, arg1_logic_expr);
    } else if (fnode->expr_type_ == "INTERVAL_WEEK") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_week, expr_str_,
          OperType::oper_date_add_week, arg0_logic_expr, arg1_logic_expr);
    } else if (fnode->expr_type_ == "INTERVAL_MONTH") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_month, expr_str_,
          OperType::oper_date_add_month, arg0_logic_expr, arg1_logic_expr);
    } else if (fnode->expr_type_ == "INTERVAL_YEAR") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_year, expr_str_,
          OperType::oper_date_add_year, arg0_logic_expr, arg1_logic_expr);
    } else if (fnode->expr_type_ == "INTERVAL_QUARTER") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_quarter, expr_str_,
          OperType::oper_date_add_month, arg0_logic_expr, arg1_logic_expr);
    } else {
      LOG(WARNING) << fnode->expr_type_ << " isn't supported now!" << endl;
      assert(false);
    }
  } else if (expr_type_ == "DATE_SUB") {
    arg0_->GetLogicalPlan(arg0_logic_expr, left_lplan, right_lplan);
    AstExprFunc* fnode = reinterpret_cast<AstExprFunc*>(arg1_);
    fnode->arg0_->GetLogicalPlan(arg1_logic_expr, left_lplan, right_lplan);
    if (fnode->expr_type_ == "INTERVAL_DAY") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_day, expr_str_,
          OperType::oper_date_sub_day, arg0_logic_expr, arg1_logic_expr);
    } else if (fnode->expr_type_ == "INTERVAL_WEEK") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_week, expr_str_,
          OperType::oper_date_sub_week, arg0_logic_expr, arg1_logic_expr);
    } else if (fnode->expr_type_ == "INTERVAL_MONTH") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_month, expr_str_,
          OperType::oper_date_sub_month, arg0_logic_expr, arg1_logic_expr);
    } else if (fnode->expr_type_ == "INTERVAL_YEAR") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_year, expr_str_,
          OperType::oper_date_sub_year, arg0_logic_expr, arg1_logic_expr);
    } else if (fnode->expr_type_ == "INTERVAL_QUARTER") {
      logic_expr = new ExprDate(
          ExprNodeType::t_qexpr_date_add_sub, t_date, t_date_quarter, expr_str_,
          OperType::oper_date_sub_month, arg0_logic_expr, arg1_logic_expr);
    } else {
      LOG(WARNING) << fnode->expr_type_ << " isn't supported now!" << endl;
      assert(false);
    }
  } else {
    LOG(WARNING) << expr_type_ << " this expression isn't supported now!"
                 << endl;
    assert(false);
  }
  return rSuccess;
}
RetCode AstExprFunc::SolveSelectAlias(
    SelectAliasSolver* const select_alias_solver) {
  if (NULL != arg0_) {
    arg0_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(arg0_);
    select_alias_solver->DeleteOldNode();
  }
  if (NULL != arg1_) {
    arg1_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(arg1_);
    select_alias_solver->DeleteOldNode();
  }
  if (NULL != arg2_) {
    arg2_->SolveSelectAlias(select_alias_solver);
    select_alias_solver->SetNewNode(arg2_);
    select_alias_solver->DeleteOldNode();
  }
  return rSuccess;
}
//}  // namespace sql_parser
//}  // namespace claims
