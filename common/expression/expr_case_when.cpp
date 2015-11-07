/*
 * expr_case_when.cpp
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include "./expr_case_when.h"

#include <malloc.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <map>
#include <vector>
namespace claims {
namespace common {
ExprCaseWhen::ExprCaseWhen(ExprNodeType expr_node_type, data_type actual_type,
                           const char* alias,
                           const std::vector<ExprNode*>& mywhen,
                           const std::vector<ExprNode*>& mythen)
    : ExprNode(expr_node_type, actual_type, alias),
      mywhen_(mywhen),
      mythen_(mythen) {}
ExprCaseWhen::ExprCaseWhen(ExprCaseWhen* expr)
    : ExprNode(expr), mywhen_(expr->mywhen_), mythen_(expr->mythen_) {
  for (int i = 0; i < mywhen_.size(); i++) {
    mywhen_[i] = mywhen_[i]->ExprCopy();
  }
  for (int i = 0; i < mythen_.size(); i++) {
    mythen_[i] = mythen_[i]->ExprCopy();
  }
}
void* ExprCaseWhen::ExprEvaluate(void* tuple, Schema* schema) {
  ExprNode* then = mythen_[mythen_.size() - 1];
  void* result;
  for (int i = 0; i < mywhen_.size(); i++) {
    if (*static_cast<bool*>(mywhen_[i]->ExprEvaluate(tuple, schema)) == true) {
      then = mythen_[i];
      break;
    }
  }  // mythen_ shouldn't be NULL, checked before
  result = then->ExprEvaluate(tuple, schema);
  return type_cast_func_(result, value_);
}

void ExprCaseWhen::InitExprAtLogicalPlan(
    data_type return_type, const std::map<std::string, int>& column_index,
    Schema* schema) {
  return_type_ = return_type;
  value_size_ = 0;
  is_null_ = false;
  for (int i = 0; i < mywhen_.size(); i++) {
    mywhen_[i]->InitExprAtLogicalPlan(t_boolean, column_index, schema);
  }
  for (int i = 0; i < mythen_.size(); i++) {
    mythen_[i]->InitExprAtLogicalPlan(mythen_[i]->actual_type_, column_index,
                                      schema);
    value_size_ = std::max(value_size_, mythen_[i]->value_size_);
    is_null_ = (is_null_ || mythen_[i]->is_null_);
  }
}

void ExprCaseWhen::InitExprAtPhysicalPlan() {
  for (int i = 0; i < mywhen_.size(); i++) {
    mywhen_[i]->InitExprAtPhysicalPlan();
  }
  for (int i = 0; i < mythen_.size(); i++) {
    mythen_[i]->InitExprAtPhysicalPlan();
  }
  type_cast_func_ = ExprTypeCast::type_cast_func_[actual_type_][return_type_];
  value_ = memalign(cacheline_size, value_size_);
}

ExprNode* ExprCaseWhen::ExprCopy() { return new ExprCaseWhen(this); }
}  // namespace common
}  // namespace claims
