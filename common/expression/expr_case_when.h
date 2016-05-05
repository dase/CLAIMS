/*
 * expr_case_when.h
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_EXPRESSION_EXPR_CASE_WHEN_H_
#define COMMON_EXPRESSION_EXPR_CASE_WHEN_H_

#include "./expr_node.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "../../common/Schema/Schema.h"
#include "./expr_type_cast.h"
namespace claims {
namespace common {
class ExprCaseWhen : public ExprNode {
 public:
  std::vector<ExprNode*> case_when_;
  std::vector<ExprNode*> case_then_;
  ExprCaseWhen(ExprNodeType expr_node_type, data_type actual_type, string alias,
               const std::vector<ExprNode*>& mycase,
               const std::vector<ExprNode*>& mywhen);
  explicit ExprCaseWhen(ExprCaseWhen* expr);
  ExprCaseWhen() {}
  ~ExprCaseWhen() {
    for (int i = 0; i < case_when_.size(); i++) {
      delete case_when_[i];
    }
    for (int i = 0; i < case_then_.size(); i++) {
      delete case_then_[i];
    }
    case_when_.clear();
    case_then_.clear();
  }

  void* ExprEvaluate(ExprEvalCnxt& eecnxt);

  void InitExprAtLogicalPlan(LogicInitCnxt& licnxt);

  void InitExprAtPhysicalPlan();
  ExprNode* ExprCopy();

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(const Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<ExprNode>(*this) & case_when_ &
        case_then_;
  }
};

}  // namespace common
}  // namespace claims
#endif  // COMMON_EXPRESSION_EXPR_CASE_WHEN_H_
