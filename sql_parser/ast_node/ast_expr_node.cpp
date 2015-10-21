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
#include <glog/logging.h>

#include <iostream>  //  NOLINT
#include <iomanip>
#include <string>
#include <bitset>

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
  if (expr_type_.compare("CONST_INT") == 0) {
  } else if (expr_type_.compare("CONST_DOUBLE") == 0) {
  } else if (expr_type_.compare("CONST_STRING") == 0) {
  } else if (expr_type_.compare("CONST_BOOL") == 0) {
  }
  return ret;
}
AstExprUnary::AstExprUnary(AstNodeType ast_node_type, string expr_type,
                           AstNode* arg0)
    : AstNode(ast_node_type), expr_type_(expr_type), arg0_(arg0) {}

AstExprUnary::~AstExprUnary() { delete arg0_; }

void AstExprUnary::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " "
       << "|expr unary| " << expr_type_ << endl;
  if (arg0_ != NULL) {
    arg0_->Print(level + 1);
  }
}
ErrorNo AstExprUnary::SemanticAnalisys(SemanticContext* sem_cnxt) {
  if (NULL != arg0_) {
    return arg0_->SemanticAnalisys(sem_cnxt);
  }
  LOG(ERROR) << "arg0 is NULL in unary expr!" << endl;
  return eArgNotExist;
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
       << "|expr binary| " << expr_type_ << endl;
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
    return arg1_->SemanticAnalisys(sem_cnxt);
  }
  return eOK;
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
       << "|expr binary| " << cmp_para_ << "  " << expr_type_ << endl;
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
    return arg1_->SemanticAnalisys(sem_cnxt);
  }
  return eOK;
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
       << "|expr list| " << endl;
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
       << "|expr function| " << expr_type_ << endl;
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
    return arg2_->SemanticAnalisys(sem_cnxt);
  }
  return eOK;
}
