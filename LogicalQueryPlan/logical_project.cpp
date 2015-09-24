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
 * /CLAIMS/LogicalQueryPlan/project.cpp
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
#include "./logical_project.h"
#include "../common/ids.h"
#include "../common/data_type.h"
#include "../common/Expression/initquery.h"
#include "../common/log/logging.h"

// namespace claims {
// namespace logical_query_plan {

LogicalProject::LogicalProject(LogicalOperator* child,
                               vector<QNode*> expression_tree)
    : child_(child),
      expression_tree_(expression_tree),
      dataflow_(NULL) {
  setOperatortype(l_project);
}

LogicalProject::~LogicalProject() {
  if (NULL != dataflow_) {
    delete dataflow_;
    dataflow_ = NULL;
  }
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
}
// construct a dataflow from child
Dataflow LogicalProject::GetDataflow() {
  if (NULL != dataflow_) return *dataflow_;
  Dataflow ret;
  // get the dataflow of child
  const Dataflow child_dataflow = child_->GetDataflow();
  /**
   * set commnication_cost and partitioner of the dataflow to be returned
   * from dataflow of child
   */
  ret.property_.commnication_cost = child_dataflow.property_.commnication_cost;
  ret.property_.partitioner = child_dataflow.property_.partitioner;
  std::vector<Attribute> ret_attrs;
  // construct an input schema from attribute list of child
  Schema* input_ = getSchema(child_dataflow.attribute_list_);
  // get the index of attributes in child dataflow
  SetColumnID(child_dataflow);
  /**
   * if the expression type is compare,then the new column will be boolean
   * type,
   * else will be it's actual type according to the variable
   */
  for (int i = 0; i < expression_tree_.size(); ++i) {
    if (expression_tree_[i]->type == t_qexpr_cmp) {
      InitExprAtLogicalPlan(expression_tree_[i], t_boolean, column_id_, input_);
    } else {
      InitExprAtLogicalPlan(expression_tree_[i],
                            expression_tree_[i]->actual_type, column_id_,
                            input_);
    }
  }
  // clean the attribute list of dataflow to be returned
  ret_attrs.clear();

  /**
   * if the return type is a String value,then we have to calculate the length
   * of the string
   * else just construct a column having the same type as the return type
   */
  for (int i = 0; i < expression_tree_.size(); ++i) {
    column_type* column = NULL;
    if (t_string == expression_tree_[i]->return_type ||
       t_decimal == expression_tree_[i]->return_type) {
      column = new column_type(expression_tree_[i]->return_type,
                               expression_tree_[i]->length);
    } else {
      column = new column_type(expression_tree_[i]->return_type);
    }
    // set TableID
    const unsigned kTableID = INTERMEIDATE_TABLEID;
    // construct attribute
    Attribute attr_alais(kTableID, i, expression_tree_[i]->alias, column->type,
                         column->size);
    // construct an attribute list
    ret_attrs.push_back(attr_alais);
  }
  // set the attribute list of the dataflow to be returned
  ret.attribute_list_ = ret_attrs;
  dataflow_ = new Dataflow();
  // set the dataflow to be returned
  *dataflow_ = ret;
  return ret;
}

/**
 * Traverse the attribute_list_，
 * store the attribute name and index into colindex_.
 */
void LogicalProject::SetColumnID(Dataflow dataflow) {
  for (int i = 0; i < dataflow.attribute_list_.size(); i++) {
    column_id_[dataflow.attribute_list_[i].attrName] = i;
  }
}

// get dataflow and child physical plan from child ,
BlockStreamIteratorBase* LogicalProject::GetIteratorTree(
    const unsigned &block_size) {
  GetDataflow();
  const Dataflow child_dataflow = child_->GetDataflow();
  BlockStreamIteratorBase* child = child_->GetIteratorTree(block_size);
  BlockStreamProjectIterator::State state;

  // assign some attributes to the state
  state.block_size_ = block_size;
  state.child_ = child;
  state.input_ = getSchema(child_dataflow.attribute_list_);
  state.output_ = GetOutputSchema();
  state.exprTree_ = expression_tree_;
  return new BlockStreamProjectIterator(state);
}

// construct a schema from attribute list of dataflow
Schema* LogicalProject::GetOutputSchema() {
  Schema* schema = getSchema(dataflow_->attribute_list_);
  return schema;
}

// Print the whole logical operation tree
void LogicalProject::Print(int level) const {
  printf("project:\n");
  LOG(INFO) << "project:\n" << endl;
  for (int i = 0; i < expression_tree_.size(); i++) {
    printf("%s\n", expression_tree_[i]->alias.c_str());
    LOG(INFO) << expression_tree_[i]->alias.c_str() << endl;
  }
  child_->Print(level + 1);
}

//}  // namespace logical_query_plan
//}  // namespace claims
