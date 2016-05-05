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
namespace claims {
namespace common {
class ExprUnary : public ExprNode {
 public:
  OperType oper_type_;
  ExprNode* arg0_;
  DataTypeOperFunc data_type_oper_func_;
  ExprUnary(ExprNodeType expr_node_type, data_type actual_type, string alias,
            OperType oper_type, ExprNode* arg0);
  explicit ExprUnary(ExprUnary* expr);
  ExprUnary() {}
  ~ExprUnary() { delete arg0_; }
  virtual void* ExprEvaluate(ExprEvalCnxt& eecnxt);

  virtual void InitExprAtLogicalPlan(LogicInitCnxt& licnxt);
  virtual void* ExprEvaluate(ExprEvalCnxt& eecnxt, void* last_value);
  virtual void* ExprEvaluate(void* value, void* last_value);

  virtual void InitExprAtPhysicalPlan();
  virtual ExprNode* ExprCopy();

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(const Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<ExprNode>(*this) & oper_type_ & arg0_;
  }
};
}  // namespace common
}  // namespace claims
#endif  // COMMON_EXPRESSION_EXPR_UNARY_H_
