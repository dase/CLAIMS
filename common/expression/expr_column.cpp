/*
 * expr_column.cpp
 *  Created on: May 30, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include "./expr_column.h"

#include <glog/logging.h>
#include <malloc.h>
#include <algorithm>
#include <iosfwd>
#include <string>
#include <iostream>
#include <map>

#include "./expr_node.h"
#include "./expr_type_cast.h"
using std::string;
namespace claims {
namespace common {
ExprColumn::ExprColumn(ExprNodeType expr_node_type, data_type actual_type,
                       string alias, string table_name, string column_name)
    : ExprNode(expr_node_type, actual_type, alias),
      table_name_(table_name),
      column_name_(column_name),
      table_id_(0),
      attr_id_(0) {
  get_type_ = actual_type;
}
ExprColumn::ExprColumn(ExprColumn* expr)
    : ExprNode(expr),
      attr_id_(expr->attr_id_),
      table_id_(expr->table_id_),
      table_name_(expr->table_name_),
      column_name_(expr->column_name_) {}

void* ExprColumn::ExprEvaluate(ExprEvalCnxt& eecnxt) {
  void* result = eecnxt.schema[table_id_]->getColumnAddess(
      attr_id_, eecnxt.tuple[table_id_]);
  return type_cast_func_(result, value_);
}
// checking the column belongs to witch table
void ExprColumn::InitExprAtLogicalPlan(LogicInitCnxt& licnxt) {
  return_type_ = licnxt.return_type_;
  auto it = licnxt.column_id0_.find(table_name_ + "." + column_name_);
  if (it != licnxt.column_id0_.end()) {
    attr_id_ = it->second;
    table_id_ = 0;
    if (return_type_ == t_string) {
      value_size_ = std::max(licnxt.schema0_->getcolumn(attr_id_).get_length(),
                             static_cast<unsigned int>(BASE_DATA_SIZE));
    } else if (return_type_ == t_decimal) {
      value_size_ = licnxt.schema0_->getcolumn(attr_id_).size;
    } else {
      value_size_ = licnxt.schema0_->getcolumn(attr_id_).get_length();
    }
    is_null_ = false;
  } else {
    auto it = licnxt.column_id1_.find(table_name_ + "." + column_name_);
    if (it != licnxt.column_id1_.end()) {
      attr_id_ = it->second;
      table_id_ = 1;
      if (return_type_ == t_string) {
        value_size_ =
            std::max(licnxt.schema1_->getcolumn(attr_id_).get_length(),
                     static_cast<unsigned int>(BASE_DATA_SIZE));
      } else if (return_type_ == t_decimal) {
        value_size_ = licnxt.schema1_->getcolumn(attr_id_).size;
      } else {
        value_size_ = licnxt.schema1_->getcolumn(attr_id_).get_length();
      }
      is_null_ = false;
    } else {
      LOG(ERROR) << "[ " << table_name_ + "." + column_name_
                 << " ] doesn't exist in column_id_map during initalize "
                    "column at logical plan" << endl;
      assert(false);
    }
  }
}

void ExprColumn::InitExprAtPhysicalPlan() {
  type_cast_func_ = ExprTypeCast::type_cast_func_[actual_type_][return_type_];
  value_ = memalign(cacheline_size, value_size_);
}

ExprNode* ExprColumn::ExprCopy() { return new ExprColumn(this); }

void ExprColumn::GetUniqueAttr(set<string>& attrs) {
  attrs.insert(table_name_ + "." + column_name_);
}

}  // namespace common
}  // namespace claims
