/*
 * expr_const.h
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_EXPRESSION_EXPR_CONST_H_
#define COMMON_EXPRESSION_EXPR_CONST_H_

#include <iosfwd>
#include "./expr_node.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "../../common/Schema/Schema.h"
#include "./expr_type_cast.h"
namespace claims {
namespace common {
class ExprConst : public ExprNode {
 public:
  std::string const_value_;
  ExprConst(ExprNodeType expr_node_type, data_type actual_type, string alias,
            string const_value);
  explicit ExprConst(ExprConst* expr_const);
  ExprConst() {}
  ~ExprConst() {}
  void* ExprEvaluate(ExprEvalCnxt& eecnxt);

  void InitExprAtLogicalPlan(LogicInitCnxt& licnxt);

  void InitExprAtPhysicalPlan();
  ExprNode* ExprCopy();

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(const Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<ExprNode>(*this) & const_value_;
  }
};
}  // namespace common
}  // namespace claims
#endif  // COMMON_EXPRESSION_EXPR_CONST_H_
