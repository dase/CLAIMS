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
void* ExprColumn::ExprEvaluate(void* tuple, Schema* schema) {
  void* result = schema->getColumnAddess(attr_id_, tuple);
  return type_cast_func_(result, value_);
}

void ExprColumn::InitExprAtLogicalPlan(
    data_type return_type, const std::map<std::string, int>& column_index,
    Schema* schema) {
  return_type_ = return_type;
  auto it = column_index.find(table_name_ + "." + column_name_);
  if (it != column_index.end()) {
    attr_id_ = it->second;
  } else {
    LOG(ERROR) << "[ " << table_name_ + "." + column_name_
               << " ] doesn't exist in column_index_map during initalize "
                  "column at logical plan" << endl;
    assert(false);
  }

  // now column_name_ like A.a, but may be change to a.
  if (return_type_ == t_string) {
    value_size_ = std::max(schema->getcolumn(attr_id_).get_length(),
                           static_cast<unsigned int>(BASE_DATA_SIZE));
  } else if (return_type_ == t_decimal) {
  	value_size_ = schema->getcolumn(attr_id_).size;
  } else {
    value_size_ = schema->getcolumn(attr_id_).get_length();

  }
  is_null_ = false;
}

void ExprColumn::InitExprAtPhysicalPlan() {
  type_cast_func_ = ExprTypeCast::type_cast_func_[actual_type_][return_type_];
  value_ = memalign(cacheline_size, value_size_);
}

ExprNode* ExprColumn::ExprCopy() { return new ExprColumn(this); }
}  // namespace common
}  // namespace claims
