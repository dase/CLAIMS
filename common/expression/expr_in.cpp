/*
 * expr_in.cpp
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include "./expr_in.h"

#include <malloc.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <map>
#include <vector>
namespace claims {
namespace common {
ExprIn::ExprIn(ExprNodeType expr_node_type, data_type actual_type, string alias,
               const std::vector<ExprBinary*> cmp_expr,
               const std::vector<vector<ExprNode*> > in_right)
    : ExprNode(expr_node_type, actual_type, alias),
      cmp_expr_(cmp_expr),
      right_node_(in_right) {}
ExprIn::ExprIn(ExprIn* expr)
    : ExprNode(expr),
      cmp_expr_(expr->cmp_expr_),
      right_node_(expr->right_node_) {
  for (int i = 0; i < cmp_expr_.size(); i++) {
    cmp_expr_[i] = cmp_expr_[i]->ExprCopy();
  }
  for (int i = 0; i < right_node_.size(); i++) {
    for (int j = 0; j < right_node_[i].size(); j++) {
      right_node_[i][j] = right_node_[i][j]->ExprCopy();
    }
  }
}

void* ExprIn::ExprItemEvaluate(ExprEvalCnxt& eecnxt, ExprBinary* cmp_expr,
                               ExprNode* right_node) {
  OperFuncInfoData oper_info;
  oper_info.args_[0] = cmp_expr->arg0_->ExprEvaluate(eecnxt);
  oper_info.args_[1] = right_node->ExprEvaluate(eecnxt);
  oper_info.args_num_ = 2;
  oper_info.result_ = value_;
  cmp_expr->data_type_oper_func_(&oper_info);
  //    return TypeCastFunc_(oper_info.result_, value_);
  return oper_info.result_;
}

void* ExprIn::ExprEvaluate(ExprEvalCnxt& eecnxt) {
  bool result = false;
  bool tmp_result = true;
  for (int i = 0; i < right_node_.size() && !result; ++i) {
    tmp_result = true;
    for (int j = 0; j < right_node_[i].size() && tmp_result; ++j) {
      tmp_result = *(static_cast<bool*>(
          ExprItemEvaluate(eecnxt, cmp_expr_[j], right_node_[i][j])));
    }
    result = tmp_result;
  }
  return type_cast_func_(&result, value_);
}

void ExprIn::InitExprAtLogicalPlan(LogicInitCnxt& licnxt) {
  return_type_ = licnxt.return_type_;
  value_size_ = BASE_DATA_SIZE;
  is_null_ = false;

  licnxt.return_type_ = t_boolean;
  for (int i = 0; i < cmp_expr_.size(); i++) {
    cmp_expr_[i]->InitExprAtLogicalPlan(licnxt);
  }

  for (int i = 0; i < right_node_.size(); i++) {
    for (int j = 0; j < right_node_[i].size(); j++) {
      licnxt.return_type_ = cmp_expr_[j]->get_type_;
      right_node_[i][j]->InitExprAtLogicalPlan(licnxt);
    }
  }
}

void ExprIn::InitExprAtPhysicalPlan() {
  for (int i = 0; i < cmp_expr_.size(); i++) {
    cmp_expr_[i]->InitExprAtPhysicalPlan();
  }
  for (int i = 0; i < right_node_.size(); i++) {
    for (int j = 0; j < right_node_[i].size(); j++) {
      right_node_[i][j]->InitExprAtPhysicalPlan();
    }
  }
  type_cast_func_ = ExprTypeCast::type_cast_func_[actual_type_][return_type_];
  value_ = memalign(cacheline_size, value_size_);
}

ExprNode* ExprIn::ExprCopy() { return new ExprIn(this); }

void ExprIn::GetUniqueAttr(set<string>& attrs) {
  for (int i = 0, size = cmp_expr_.size(); i < size; ++i) {
    cmp_expr_[i]->GetUniqueAttr(attrs);
  }
  for (int i = 0; i < right_node_.size(); i++) {
    for (int j = 0; j < right_node_[i].size(); j++) {
      right_node_[i][j]->GetUniqueAttr(attrs);
    }
  }
}

}  // namespace common
}  // namespace claims
