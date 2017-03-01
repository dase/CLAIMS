#include <atomic>

/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * /CLAIMS/logical_operator/project.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: casa, ChenLingyun
 *       Email: geekchenlingyun@outlook.com
 *
 * Description:
 * it's a function which takes an input (e.g. a database row) and
 * produces an output (e.g. one of the columns from the row, or
 * perhaps some calculation based on multiple columns).
 */
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <vector>
#include <map>
#include <string>
#include "../common/expression/expr_column.h"
#include "../logical_operator/logical_project.h"
#include "../logical_operator/logical_operator.h"
#include "../common/ids.h"
#include "../common/data_type.h"
#include "../common/Expression/initquery.h"
#include "../common/log/logging.h"
#include "../common/expression/expr_node.h"
#include "../physical_operator/physical_project.h"

using claims::common::ExprColumn;
using claims::common::ExprNode;
using claims::common::LogicInitCnxt;
using claims::physical_operator::PhysicalProject;
namespace claims {
namespace logical_operator {
LogicalProject::LogicalProject(LogicalOperator* child,
                               vector<QNode*> expression_tree)
    : LogicalOperator(kLogicalProject),
      child_(child),
      expression_tree_(expression_tree),
      plan_context_(NULL) {}
LogicalProject::LogicalProject(LogicalOperator* child,
                               vector<ExprNode*> expr_list)
    : LogicalOperator(kLogicalProject),
      child_(child),
      expr_list_(expr_list),
      plan_context_(NULL) {}
LogicalProject::~LogicalProject() {
  if (NULL != plan_context_) {
    delete plan_context_;
    plan_context_ = NULL;
  }
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
}
// construct a PlanContext from child
PlanContext LogicalProject::GetPlanContext() {
  lock_->acquire();
  //  if (NULL != plan_context_) {
  //    lock_->release();
  //    return *plan_context_;
  //  }
  PlanContext ret;
  // get the PlanContext of child
  const PlanContext child_plan_context = child_->GetPlanContext();
  /**
   * set commnication_cost and partitioner of the PlanContext to be returned
   * from PlanContext of child
   */
  ret.commu_cost_ = child_plan_context.commu_cost_;
  ret.plan_partitioner_ = child_plan_context.plan_partitioner_;
  std::vector<Attribute> ret_attrs;
  // construct an input schema from attribute list of child
  Schema* input_schema = GetSchema(child_plan_context.attribute_list_);
// get the index of attributes in child PlanContext
/**
 * if the expression type is compare,then the new column will be boolean
 * type,
 * else will be it's actual type according to the variable
 */

#ifdef NEWCONDI
  for (int i = 0; i < expression_tree_.size(); ++i) {
    if (expression_tree_[i]->type == t_qexpr_cmp) {
      InitExprAtLogicalPlan(expression_tree_[i], t_boolean, column_id_,
                            input_schema);
    } else {
      InitExprAtLogicalPlan(expression_tree_[i],
                            expression_tree_[i]->actual_type, column_id_,
                            input_schema);
    }
  }
  // clean the attribute list of PlanContext to be returned
  ret_attrs.clear();

  /**
   * if the return type is a String value,then we have to calculate the length
   * of the string
   * else just construct a column having the same type as the return type
   */
  for (int i = 0; i < expression_tree_.size(); ++i) {
    column_type* column = NULL;
    if (t_string == expression_tree_[i]->return_type_ ||
        t_decimal == expression_tree_[i]->return_type_) {
      column = new column_type(expression_tree_[i]->return_type_,
                               expression_tree_[i]->length);
    } else {
      column = new column_type(expression_tree_[i]->return_type_);
    }
    // set TableID
    const unsigned kTableID = INTERMEIDATE_TABLEID;
    // construct attribute
    Attribute attr_alais(kTableID, i, expression_tree_[i]->alias, column->type,
                         column->size);
    // construct an attribute list
    ret_attrs.push_back(attr_alais);
  }
#else
  ret_attrs.clear();
  LogicInitCnxt licnxt;
  licnxt.schema0_ = input_schema;
  int mid_table_id = 0;
  if (plan_context_ == NULL) {
    mid_table_id = MIDINADE_TABLE_ID++;
  } else {
    mid_table_id =
        plan_context_->plan_partitioner_.get_partition_key().table_id_;
  }
  GetColumnToId(child_plan_context.attribute_list_, licnxt.column_id0_);
  for (int i = 0; i < expr_list_.size(); ++i) {
    licnxt.return_type_ = expr_list_[i]->actual_type_;
    expr_list_[i]->InitExprAtLogicalPlan(licnxt);
    ret_attrs.push_back(expr_list_[i]->ExprNodeToAttr(i, mid_table_id));

    // update partition key
    if (t_qcolcumns == expr_list_[i]->expr_node_type_) {
      ExprColumn* column = reinterpret_cast<ExprColumn*>(expr_list_[i]);
      if (ret.plan_partitioner_.get_partition_key().attrName ==
          column->table_name_ + "." + column->column_name_) {
        ret.plan_partitioner_.set_partition_key(ret_attrs[i]);
      }
    }
  }

#endif
  // set the attribute list of the PlanContext to be returned
  ret.attribute_list_ = ret_attrs;
  plan_context_ = new PlanContext();
  // set the PlanContext to be returned
  *plan_context_ = ret;
  lock_->release();
  return ret;
}

// get PlanContext and child physical plan from child ,
PhysicalOperatorBase* LogicalProject::GetPhysicalPlan(
    const unsigned& block_size) {
  GetPlanContext();
  const PlanContext child_plan_context = child_->GetPlanContext();
  PhysicalOperatorBase* child = child_->GetPhysicalPlan(block_size);
  PhysicalProject::State state;

  // assign some attributes to the state_
  state.block_size_ = block_size;
  state.child_ = child;
  state.schema_input_ = GetSchema(child_plan_context.attribute_list_);
  state.schema_output_ = GetOutputSchema();
  state.expr_tree_ = expression_tree_;
  state.expr_list_ = expr_list_;
  return new PhysicalProject(state);
}

// construct a schema from attribute list of PlanContext
Schema* LogicalProject::GetOutputSchema() {
  Schema* schema = GetSchema(plan_context_->attribute_list_);
  return schema;
}

// Print the whole logical operation tree
void LogicalProject::Print(int level) const {
  cout << setw(level * kTabSize) << " "
       << "Project:" << endl;
  LOG(INFO) << "Project:\n" << endl;
#ifdef NEWCONDI
  for (int i = 0; i < expression_tree_.size(); i++) {
    printf("%s\n", expression_tree_[i]->alias.c_str());
    LOG(INFO) << expression_tree_[i]->alias.c_str() << endl;
  }
#else
  GetPlanContext();
  cout << setw(level * kTabSize) << " "
       << "[Partition info: "
       << plan_context_->plan_partitioner_.get_partition_key().attrName
       << " table_id= "
       << plan_context_->plan_partitioner_.get_partition_key().table_id_
       << " column_id= "
       << plan_context_->plan_partitioner_.get_partition_key().index << " ]"
       << endl;
  ++level;
  for (int i = 0; i < expr_list_.size(); ++i) {
    cout << setw(level * kTabSize) << " " << expr_list_[i]->alias_ << endl;
  }
  --level;
#endif
  child_->Print(level);
}
void LogicalProject::PruneProj(set<string>& above_attrs) {
  for (int i = 0, size = expr_list_.size(); i < size; ++i) {
    expr_list_[i]->GetUniqueAttr(above_attrs);
  }
  child_->PruneProj(above_attrs);
}

}  // namespace logical_operator
}  // namespace claims
