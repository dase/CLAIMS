/*
 * expr_date.cpp
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include "./expr_date.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <map>

ExprDate::ExprDate(ExprNodeType expr_node_type, data_type actual_type,
                   const char* alias, OperType oper_type, ExprNode* arg0,
                   ExprNode* arg1)
        : ExprNode(expr_node_type, actual_type, alias),
          oper_type_(oper_type),
          arg0_(arg0),
          arg1_(arg1),
          arg1_return_type_(actual_type),
          DataTypeOperFunc_(NULL) {
}
ExprDate::ExprDate(ExprDate* expr)
        : ExprNode(expr),
          oper_type_(expr->oper_type_),
          arg0_(expr->arg0_->ExprCoyp()),
          arg1_(expr->arg1_->ExprCoyp()),
          arg1_return_type_(expr->arg1_return_type_),
          DataTypeOperFunc_(expr->DataTypeOperFunc_) {
}
void* ExprDate::ExprEvaluate(void* tuple, Schema* schema) {
    OperFuncInfoData oper_info;
    oper_info.args_[0] = arg0_->ExprEvaluate(tuple, schema);
    oper_info.args_[1] = arg1_->ExprEvaluate(tuple, schema);
    oper_info.args_num_ = 2;
    oper_info.result_ = value_;
    DataTypeOperFunc_(&oper_info);
    return TypeCastFunc_(oper_info.result_, value_);
}

void ExprDate::InitExprAtLogicalPlan(
        data_type return_type, const std::map<std::string, int>&column_index,
        Schema* schema) {
    return_type_ = return_type;
    // difference from ExprBinary
    arg0_->InitExprAtLogicalPlan(actual_type_, column_index, schema);   //
    arg1_->InitExprAtLogicalPlan(arg1_return_type_, column_index, schema);  //
    value_size_ = std::max(arg0_->value_size_, arg1_->value_size_);
    is_null_ = (arg0_->is_null_ || arg1_->is_null_);
}

void ExprDate::InitExprAtPhysicalPlan() {
    arg0_->InitExprAtPhysicalPlan();
    arg1_->InitExprAtPhysicalPlan();
    DataTypeOperFunc_ =
            DataTypeOper::data_type_oper_func_[actual_type_][oper_type_];
    TypeCastFunc_ = ExprTypeCast::type_cast_func_[actual_type_][return_type_];
    value_ = memalign(cacheline_size, value_size_);
}

ExprNode* ExprDate::ExprCoyp() {
    return new ExprDate(this);
}
