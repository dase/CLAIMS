/*
 * expr_const.cpp
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include "./expr_const.h"
#include <malloc.h>
#include <string.h>
#include <algorithm>
#include <iosfwd>
#include <string>
#include <iostream>
#include <map>
using std::string;
namespace claims {
namespace common {
ExprConst::ExprConst(ExprNodeType expr_node_type, data_type actual_type,
                     string alias, string const_value)
    : ExprNode(expr_node_type, actual_type, alias), const_value_(const_value) {}
ExprConst::ExprConst(ExprConst* expr)
    : ExprNode(expr), const_value_(expr->const_value_) {}
void* ExprConst::ExprEvaluate(ExprEvalCnxt& eecnxt) { return value_; }

void ExprConst::InitExprAtLogicalPlan(LogicInitCnxt& licnxt) {
  return_type_ = licnxt.return_type_;
  value_size_ = max(static_cast<int>(const_value_.size()), BASE_DATA_SIZE);
  is_null_ = false;
}

/*
 * for const, the value has been type_casted at InitExprAtPhysicalPlan(),
 *  so later, just to return the value_ is ok, because this can avoid
 * type_casted
 *  everytime.
 */
void ExprConst::InitExprAtPhysicalPlan() {
  value_ = memalign(cacheline_size, value_size_);
  strcpy(static_cast<char*>(value_), const_value_.c_str());
  ExprTypeCast::type_cast_func_[t_string][return_type_](value_, value_);
}

ExprNode* ExprConst::ExprCopy() { return new ExprConst(this); }
}  // namespace common
}  // namespace claims
