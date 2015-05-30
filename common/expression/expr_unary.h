/*
 * expr_unary.h
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_EXPRESSION_EXPR_UNARY_H_
#define COMMON_EXPRESSION_EXPR_UNARY_H_
#include <string>
#include <map>
#include "./expr_node.h"

class ExprUnary : public ExprNode {
 public:
    OperType oper_type_;
    ExprNode* arg0_;
    DataTypeOperFunc DataTypeOperFunc_;
    ExprUnary(ExprNodeType expr_node_type, data_type actual_type,
              const char* alias, OperType oper_type, ExprNode* arg0);
    explicit ExprUnary(ExprUnary* expr);
    ~ExprUnary() {
        delete arg0_;
    }
    void* ExprEvaluate(void *tuple, Schema *schema);
    void InitExprAtLogicalPlan(data_type return_type,
                               const std::map<std::string, int>& column_index,
                               Schema* schema);
    void InitExprAtPhysicalPlan();
    ExprNode* ExprCoyp();

 private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(const Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<ExprNode>(*this) & oper_type_
                & arg0_;
    }
};

#endif /* COMMON_EXPRESSION_EXPR_UNARY_H_ */
