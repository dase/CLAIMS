/*
 * expr_in.h
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_EXPRESSION_EXPR_IN_H_
#define COMMON_EXPRESSION_EXPR_IN_H_

#include "./expr_node.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "./expr_binary.h"
#include "../../common/Schema/Schema.h"
#include "./expr_type_cast.h"
namespace claims {
namespace common {
class ExprIn : public ExprNode {
 public:
  std::vector<ExprBinary*> cmp_expr_;
  std::vector<std::vector<ExprNode*> > right_node_;
  ExprIn(ExprNodeType expr_node_type, data_type actual_type, string alias,
         const std::vector<ExprBinary*> cmp_expr,
         const std::vector<vector<ExprNode*> > in_right);
  explicit ExprIn(ExprIn* expr_in);
  ExprIn() {}
  ~ExprIn() {
    for (int i = 0; i < cmp_expr_.size(); i++) {
      delete cmp_expr_[i];
    }
    for (int i = 0; i < right_node_.size(); i++) {
      for (int j = 0; j < right_node_[i].size(); j++) {
        delete right_node_[i][j];
      }
    }
    cmp_expr_.clear();
    right_node_.clear();
  }
  void* ExprEvaluate(ExprEvalCnxt& eecnxt);

  void InitExprAtLogicalPlan(LogicInitCnxt& licnxt);

  void* ExprItemEvaluate(ExprEvalCnxt& eecnxt, ExprBinary* cmp_expr,
                         ExprNode* right_node);

  void InitExprAtPhysicalPlan();
  ExprNode* ExprCopy();
  void GetUniqueAttr(set<string>& attrs);

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(const Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<ExprNode>(*this) & cmp_expr_ &
        right_node_;
  }
};
}  // namespace common
}  // namespace claims
#endif  //  COMMON_EXPRESSION_EXPR_IN_H_
