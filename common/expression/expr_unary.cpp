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
#include <string>
#include <map>
#include <vector>
#include "../../configure.h"
ExprUnary::ExprUnary(ExprNodeType expr_node_type, data_type actual_type,
                     const char* alias, OperType oper_type, ExprNode* arg0)
        : ExprNode(expr_node_type, actual_type, alias),
          oper_type_(oper_type),
          arg0_(arg0),
          DataTypeOperFunc_(NULL) {
}
ExprUnary::ExprUnary(ExprUnary* expr)
        : ExprNode(expr),
          oper_type_(expr->oper_type_),
          arg0_(expr->arg0_->ExprCoyp()),
          DataTypeOperFunc_(expr->DataTypeOperFunc_) {
}
void* ExprUnary::ExprEvaluate(void* tuple, Schema* schema) {
    OperFuncInfoData oper_info;
    oper_info.args_[0] = arg0_->ExprEvaluate(tuple, schema);
    oper_info.args_num_ = 1;
    oper_info.result_ = value_;
    DataTypeOperFunc_(&oper_info);
    return TypeCastFunc_(oper_info.result_, value_);
}

void ExprUnary::InitExprAtLogicalPlan(
        data_type return_type, const std::map<std::string, int>&column_index,
        Schema* schema) {
    return_type_ = return_type;
    arg0_->InitExprAtLogicalPlan(get_type_, column_index, schema);
    value_size_ = arg0_->value_size_;
    is_null_ = arg0_->is_null_;
}

void ExprUnary::InitExprAtPhysicalPlan() {
    arg0_->InitExprAtPhysicalPlan();
    DataTypeOperFunc_ =
            DataTypeOper::data_type_oper_func_[actual_type_][oper_type_];
    TypeCastFunc_ = ExprTypeCast::type_cast_func_[actual_type_][return_type_];
    value_ = memalign(cacheline_size, value_size_);
}

ExprNode* ExprUnary::ExprCoyp() {
    return new ExprUnary(this);
}
