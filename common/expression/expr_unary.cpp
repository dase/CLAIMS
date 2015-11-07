/*
 * expr_unary.cpp
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include "./expr_unary.h"
#include <malloc.h>
#include <iosfwd>
#include <string>
#include <map>
#include <vector>

#include "./data_type_oper.h"
#include "./expr_type_cast.h"
#include "../../configure.h"
namespace claims {

namespace common {
ExprUnary::ExprUnary(ExprNodeType expr_node_type, data_type actual_type,
                     string alias, OperType oper_type, ExprNode* arg0)
    : ExprNode(expr_node_type, actual_type, alias),
      oper_type_(oper_type),
      arg0_(arg0),
      data_type_oper_func_(NULL) {}
ExprUnary::ExprUnary(ExprUnary* expr)
    : ExprNode(expr),
      oper_type_(expr->oper_type_),
      arg0_(expr->arg0_->ExprCopy()),
      data_type_oper_func_(expr->data_type_oper_func_) {}
void* ExprUnary::ExprEvaluate(void* tuple, Schema* schema) {
  OperFuncInfoData oper_info;
  oper_info.args_[0] = arg0_->ExprEvaluate(tuple, schema);
  oper_info.args_num_ = 1;
  oper_info.result_ = value_;
  data_type_oper_func_(&oper_info);
  return type_cast_func_(oper_info.result_, value_);
}
void* ExprUnary::ExprEvaluate(void* tuple, Schema* schema, void* last_value) {
  OperFuncInfoData oper_info;
  oper_info.args_[1] = arg0_->ExprEvaluate(tuple, schema);
  oper_info.args_[0] = last_value;
  oper_info.args_num_ = 2;
  oper_info.result_ = last_value;
  data_type_oper_func_(&oper_info);
  return last_value;
}
void* ExprUnary::ExprEvaluate(void* value, void* last_value) {
  OperFuncInfoData oper_info;
  oper_info.args_[1] = value;
  oper_info.args_[0] = last_value;
  oper_info.args_num_ = 2;
  oper_info.result_ = last_value;
  data_type_oper_func_(&oper_info);
  return last_value;
}
void ExprUnary::InitExprAtLogicalPlan(
    data_type return_type, const std::map<std::string, int>& column_index,
    Schema* schema) {
  return_type_ = return_type;
  arg0_->InitExprAtLogicalPlan(get_type_, column_index, schema);
  value_size_ = arg0_->value_size_;
  is_null_ = arg0_->is_null_;
}

void ExprUnary::InitExprAtPhysicalPlan() {
  arg0_->InitExprAtPhysicalPlan();
  data_type_oper_func_ =
      DataTypeOper::data_type_oper_func_[get_type_][oper_type_];
  type_cast_func_ = ExprTypeCast::type_cast_func_[actual_type_][return_type_];
  value_ = memalign(cacheline_size, value_size_);
}

ExprNode* ExprUnary::ExprCopy() { return new ExprUnary(this); }
}  // namespace common
}  // namespace claims
