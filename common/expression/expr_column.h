/*
 * expr_column.h
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_EXPRESSION_EXPR_COLUMN_H_
#define COMMON_EXPRESSION_EXPR_COLUMN_H_

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "../../common/Schema/Schema.h"
#include "./expr_type_cast.h"
#include "./expr_node.h"
namespace claims {
namespace common {
class ExprColumn : public ExprNode {
 public:
  int attr_id_;
  std::string table_name_;
  std::string column_name_;
  int table_id_;
  ExprColumn(ExprNodeType expr_node_type, data_type actual_type, string alias,
             string table_name, string column_name);
  explicit ExprColumn(ExprColumn* expr_column);
  ExprColumn() {}
  ~ExprColumn() {}
  void* ExprEvaluate(ExprEvalCnxt& eecnxt);

  void InitExprAtLogicalPlan(LogicInitCnxt& licnxt);

  void InitExprAtPhysicalPlan();
  ExprNode* ExprCopy();
  void GetUniqueAttr(set<string>& attrs);

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(const Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<ExprNode>(*this) & table_name_ &
        column_name_ & attr_id_ & table_id_;
  }
};
}  // namespace common
}  // namespace claims
#endif  // COMMON_EXPRESSION_EXPR_COLUMN_H_
