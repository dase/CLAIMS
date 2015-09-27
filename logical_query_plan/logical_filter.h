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
 * /CLAIMS/logical_query_plan/filter.h
 *
 *  Created on: Sep 21, 2015
 *      Author: tonglanxuan
 *       Email: lxtong0526@163.com
 * 
 * Description: Structures the LogicalFilter class, which functions as logical
 *              operator "filter".
 *
 */

#ifndef LOGICALQUERYPLAN_FILTER_H_
#define LOGICALQUERYPLAN_FILTER_H_

#include <vector>
#include <map>
#include <string>
#include "../common/AttributeComparator.h"
#include "../common/ExpressionCalculator.h"
#include "../common/ExpressionItem.h"
#include "../common/Expression/qnode.h"
#include "../logical_query_plan/logical_operator.h"

// namespace claims {
// namespace logical_query_plan {


/**
 * @brief Method description: Logical Operator "filter", is used to screen out
 *                            the info we don't need, so as to lessen time cost
 */
class LogicalFilter : public LogicalOperator {
 public:
  /**
   * @brief Method description: Construction.
   * @param child : Its child logical operator.
   * @param qual: Pointing to the root of the expression tree.
   */
  LogicalFilter(LogicalOperator* child, vector<QNode*> qual);

  /**
   * @brief Method description: Destruction.
   */

  virtual ~LogicalFilter();

  /**
   * @brief Method description: To get the dataflow from its child logical
   *                            operator, operates on it, thus give it to
   *                            its father logical operator.
   * @return Dataflow
   */
  Dataflow GetDataflow();
  /**
   * @brief Method description: To get the Iterator Tree from its child logical
   *                            operator, generates a state for establishing a
   *                            physical execution plan, finally give it to its
   *                            father logical operator.
   * @param blocksize: The size of block in the CLAIMS project.
   * @return BlockStreamIteratorBase*: A block stream iterator pointer,
   *         pointing to the root node of filter's iterator tree.
   */
  BlockStreamIteratorBase* GetIteratorTree(const unsigned& blocksize);
  bool GetOptimalPhysicalPlan(Requirement requirement,
                              PhysicalPlanDescriptor& physical_plan_descriptor,
                              const unsigned& block_size = 4096 * 1024);
  /**
   * @brief Method description: To print information of qual_.
   * @param level: As an index.
   */
  void Print(int level = 0) const;

 private:
  /**
   * @brief Method description: Not used in the current version and remained to
   *                            be rewrite. To help make getDataflow more
   *                            efficient.
   * @param partition_id: The id of partition.
   * @param DataflowPartitioningDescriptor
   * @return  A boolean value. Returning TRUE indicates the partition of that
   *          partition_id can be hash pruned.
   */
  bool CanBeHashPruned(unsigned partition_id,
                       const DataflowPartitioningDescriptor&) const;
  /**
   * @brief Method description: Default returning 1 in the current version and
   *                            remained to be rewrite. To predict the
   *                            cardinality of the table.
   * @return  A float number indicating the coefficient.
   */
  float PredictSelectivity() const;
  /**
   * @brief Method description: To save the index of dataflow.attribute_list_
   *                            into column_id_.
   * @param dataflow
   */
  void set_column_id(const Dataflow& dataflow);

 private:
  LogicalOperator* child_;
  map<string, int> column_id_;
  vector<QNode*> condi_;
};

//}   // namespace logical_query_plan
//}   // namespace claims



#endif  //  LOGICALQUERYPLAN_FILTER_H_
