/*
 * expr_ternary.h
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_EXPRESSION_EXPR_TERNARY_H_
#define COMMON_EXPRESSION_EXPR_TERNARY_H_
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "../../common/Schema/Schema.h"
#include "./expr_node.h"
#include "./expr_type_cast.h"
namespace claims {
namespace common {
// now ExprTernary just for substring
class ExprTernary : public ExprNode {
 public:
  OperType oper_type_;
  ExprNode* arg0_;
  ExprNode* arg1_;
  ExprNode* arg2_;
  DataTypeOperFunc data_type_oper_func_;
  ExprTernary(ExprNodeType expr_node_type, data_type actual_type, string alias,
              OperType oper_type, ExprNode* arg0, ExprNode* arg1,
              ExprNode* arg2);
  explicit ExprTernary(ExprTernary* expr_ternary);
  ExprTernary() {}
  ~ExprTernary() {
    delete arg0_;
    delete arg1_;
    delete arg2_;
  }
  void* ExprEvaluate(void* tuple, Schema* schema);
  void InitExprAtLogicalPlan(data_type return_type,
                             const std::map<std::string, int>& column_index,
                             Schema* schema);
  void InitExprAtPhysicalPlan();
  ExprNode* ExprCopy();

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(const Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<ExprNode>(*this) & oper_type_ &
        arg0_ & arg1_ & arg2_;
  }
};
}  // namespace common
}  // namespace claims
#endif  // COMMON_EXPRESSION_EXPR_TERNARY_H_
