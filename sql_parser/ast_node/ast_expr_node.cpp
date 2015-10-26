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

#include "../ast_node/ast_expr_node.h"

#include <assert.h>
#include <glog/logging.h>

#include <iostream>  //  NOLINT
#include <iomanip>
#include <string>
#include <bitset>

#include "./ast_select_stmt.h"
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::setw;
using std::bitset;

AstExprConst::AstExprConst(AstNodeType ast_node_type, string expr_type,
                           string data)
    : AstNode(ast_node_type), expr_type_(expr_type), data_(data) {}

AstExprConst::~AstExprConst() {}
void AstExprConst::Print(int level) const {
  cout << setw(level++ * TAB_SIZE) << " "
       << "|const|" << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "const expr type: " << expr_type_ << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "const data: " << data_ << endl;
}
// TODO(FZH) be strict to the type of const
ErrorNo AstExprConst::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
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
                                      set<AstNode*>& agg_node, bool is_select) {
  agg_column = NULL;
}

AstExprUnary::AstExprUnary(AstNodeType ast_node_type, string expr_type,
                           AstNode* arg0)
    : AstNode(ast_node_type), expr_type_(expr_type), arg0_(arg0) {}

AstExprUnary::~AstExprUnary() { delete arg0_; }

void AstExprUnary::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|expr unary| " << expr_type_ << " @ " << expr_str_ << endl;
  if (arg0_ != NULL) {
    arg0_->Print(level + 1);
  }
}
ErrorNo AstExprUnary::SemanticAnalisys(SemanticContext* sem_cnxt) {
  // agg couldn't in where or groupby
  if (expr_type_ == "SUM" || expr_type_ == "MAX" || expr_type_ == "MIN" ||
      expr_type_ == "AVG" || expr_type_ == "COUNT" ||
      expr_type_ == "COUNT_ALL") {
    if (SemanticContext::kWhereClause == sem_cnxt->clause_type_) {
      return eAggCouldNotInWhereClause;
    }
    if (SemanticContext::kGroupByClause == sem_cnxt->clause_type_) {
      return eAggCouldNotInGroupByClause;
    }
  }
  if (expr_type_ == "COUNT_ALL") {
    expr_str_ = "COUNT(*)";
    return eOK;
  }
  if (NULL != arg0_) {
    // upper node have agg and this node is agg, then error
    if (sem_cnxt->have_agg &&
        ((expr_type_ == "SUM" || expr_type_ == "MAX" || expr_type_ == "MIN" ||
          expr_type_ == "AVG" || expr_type_ == "COUNT"))) {
      return eAggHaveAgg;
    }
    sem_cnxt->have_agg =
        (expr_type_ == "SUM" || expr_type_ == "MAX" || expr_type_ == "MIN" ||
         expr_type_ == "AVG" || expr_type_ == "COUNT");

    ErrorNo ret = arg0_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
    sem_cnxt->have_agg = false;
    return eOK;
  }
  LOG(ERROR) << "arg0 is NULL in unary expr!" << endl;
  return eUnaryArgNotExist;
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
                                      set<AstNode*>& agg_node, bool is_select) {
  // like a leaf node
  if (expr_type_ == "COUNT_ALL" || expr_type_ == "SUM" || expr_type_ == "MAX" ||
      expr_type_ == "MIN" || expr_type_ == "AVG" || expr_type_ == "COUNT") {
    agg_column = new AstColumn(AST_COLUMN, "NULL_AGG", expr_str_, expr_str_);
    if (is_select) {
      agg_node.insert(this);
    }
    return;
  } else {
    agg_column = NULL;
    arg0_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      if (!is_select) {
        delete arg0_;
      }
      arg0_ = agg_column;
    }
    agg_column = NULL;
  }
  return;
}
AstExprCalBinary::AstExprCalBinary(AstNodeType ast_node_type,
                                   std::string expr_type, AstNode* arg0,
                                   AstNode* arg1)
    : AstNode(ast_node_type), expr_type_(expr_type), arg0_(arg0), arg1_(arg1) {}

AstExprCalBinary::~AstExprCalBinary() {
  delete arg0_;
  delete arg1_;
}

void AstExprCalBinary::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|expr binary| " << expr_type_ << " @ " << expr_str_ << endl;
  if (arg0_ != NULL) arg0_->Print(level + 1);
  if (arg1_ != NULL) arg1_->Print(level + 1);
}
ErrorNo AstExprCalBinary::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != arg0_) {
    ret = arg0_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != arg1_) {
    ret = arg1_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }

  return eOK;
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
                                          bool is_select) {
  if (NULL != arg0_) {
    agg_column = NULL;
    arg0_->ReplaceAggregation(agg_column, agg_node, is_select);
    // this agg shouldn't delete in select clause, because they are collected
    // for build aggregation
    if (NULL != agg_column) {
      if (!is_select) {
        delete arg0_;
      }
      arg0_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != arg1_) {
    agg_column = NULL;
    arg1_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      if (!is_select) {
        delete arg1_;
      }
      arg1_ = agg_column;
    }
    agg_column = NULL;
  }
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
    default: { expr_type_ = "error type"; }
  }
}
AstExprCmpBinary::~AstExprCmpBinary() {
  delete arg0_;
  delete arg1_;
}
void AstExprCmpBinary::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|expr binary| " << expr_type_ << " @ " << expr_str_ << endl;
  if (arg0_ != NULL) arg0_->Print(level + 1);
  if (arg1_ != NULL) arg1_->Print(level + 1);
}
ErrorNo AstExprCmpBinary::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != arg0_) {
    ret = arg0_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != arg1_) {
    ret = arg1_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }

  return eOK;
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
                                          bool is_select) {
  if (NULL != arg0_) {
    agg_column = NULL;
    arg0_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      if (!is_select) {
        delete arg0_;
      }
      arg0_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != arg1_) {
    agg_column = NULL;
    arg1_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      if (!is_select) {
        delete arg1_;
      }
      arg1_ = agg_column;
    }
    agg_column = NULL;
  }
}
AstExprList::AstExprList(AstNodeType ast_node_type, AstNode* expr,
                         AstNode* next)
    : AstNode(ast_node_type), expr_(expr), next_(next) {}

AstExprList::~AstExprList() {
  delete expr_;
  delete next_;
}

void AstExprList::Print(int level) const {
  // cout << "level= " << level << endl;
  cout << setw(level * TAB_SIZE) << " "
       << "|expr list| " << expr_str_ << endl;
  if (expr_ != NULL) expr_->Print(level);
  if (next_ != NULL) next_->Print(level);
}
ErrorNo AstExprList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != expr_) {
    ret = expr_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != next_) {
    return next_->SemanticAnalisys(sem_cnxt);
  }
  return eOK;
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
                                     set<AstNode*>& agg_node, bool is_select) {
  if (NULL != expr_) {
    agg_column = NULL;
    expr_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      if (!is_select) {
        delete expr_;
      }
      expr_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != next_) {
    agg_column = NULL;
    next_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      if (!is_select) {
        delete next_;
      }
      next_ = agg_column;
    }
    agg_column = NULL;
  }
}
AstExprFunc::AstExprFunc(AstNodeType ast_node_type, std::string expr_type,
                         AstNode* arg0, AstNode* arg1, AstNode* arg2)
    : AstNode(ast_node_type),
      expr_type_(expr_type),
      arg0_(arg0),
      arg1_(arg1),
      arg2_(arg2) {}

AstExprFunc::~AstExprFunc() {
  delete arg0_;
  delete arg1_;
  delete arg2_;
}

void AstExprFunc::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|expr function| " << expr_type_ << " @ " << expr_str_ << endl;
  if (arg0_ != NULL) arg0_->Print(level + 1);
  if (arg1_ != NULL) arg1_->Print(level + 2);
  if (arg2_ != NULL) arg2_->Print(level + 3);
}
ErrorNo AstExprFunc::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != arg0_) {
    ret = arg0_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != arg1_) {
    ret = arg1_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != arg2_) {
    ret = arg2_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  return eOK;
}

void AstExprFunc::RecoverExprName(string& name) {
  string arg0_name = "";
  string arg1_name = "";
  string arg2_name = "";
  expr_str_ = expr_type_ + "(";
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
                                     set<AstNode*>& agg_node, bool is_select) {
  if (NULL != arg0_) {
    agg_column = NULL;
    arg0_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      if (!is_select) {
        delete arg0_;
      }
      arg0_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != arg1_) {
    agg_column = NULL;
    arg1_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      if (!is_select) {
        delete arg1_;
      }
      arg1_ = agg_column;
    }
    agg_column = NULL;
  }
  if (NULL != arg2_) {
    agg_column = NULL;
    arg2_->ReplaceAggregation(agg_column, agg_node, is_select);
    if (NULL != agg_column) {
      if (!is_select) {
        delete arg2_;
      }
      arg2_ = agg_column;
    }
    agg_column = NULL;
  }
}
