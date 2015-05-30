/*
 * expr_node.h
 *  Created on: May 29, 2015 11:28:14 AM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_EXPRESSION_EXPR_NODE_H_
#define COMMON_EXPRESSION_EXPR_NODE_H_
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../../common/Schema/Schema.h"
#include "./expr_type_cast.h"
#include "../../configure.h"

#define MAX_ARGS 10
#define BASE_DATA_SIZE 64
typedef struct OperFuncInfoData {
    int args_num_;
    void* args_[MAX_ARGS];
    void*result_;
}*OperFuncInfo;
typedef void (*DataTypeOperFunc)(OperFuncInfo fcinfo);
typedef void (*AvgDivide)(void *sum_value, int64_t tuple_number, void *result);

enum ExprNodeType {
    EXPR_NODE,
    EXPR_UNARY
};
enum OperType {
    OPER_NONE,
};
class ExprNode {
 public:
    ExprNode(ExprNodeType expr_node_type, data_type actual_type,
             const char* alias);
    explicit ExprNode(ExprNode* expr);
    virtual ~ExprNode() {
        if (value_ != NULL)
            free(value_);
    }
    ExprNodeType expr_node_type_;
    data_type actual_type_, return_type_, get_type_;
    void* value_;  // store temporary calculation result
    int value_size_;  // the value size
    bool is_null_;
    TypeCastFunc TypeCastFunc_;
    std::string alias_;
    virtual void* ExprEvaluate(void *tuple, Schema *schema) = 0;
    virtual void InitExprAtLogicalPlan(
            data_type return_type,
            const std::map<std::string, int>&column_index, Schema* schema) = 0;
    virtual void InitExprAtPhysicalPlan() = 0;
    virtual ExprNode* ExprCoyp() = 0;

 private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(const Archive &ar, const unsigned int version) {
        ar & expr_node_type_ & actual_type_ & get_type_ & return_type_
                & value_size_ & is_null_ & alias_;
    }
};
#endif  // COMMON_EXPRESSION_EXPR_NODE_H_
