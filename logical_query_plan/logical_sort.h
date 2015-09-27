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
 * /CLAIMS/logical_query_plan/logical_sort.h
 *
 *  Created on: Sep 21, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *
 * Description:
 *   This file mainly describe the Sort Operater.
 *
 */

#ifndef LOGICAL_QUERY_PLAN_LOGICAL_SORT_H_
#define LOGICAL_QUERY_PLAN_LOGICAL_SORT_H_
#include <string.h>
#include <vector>
#include <iostream>
#include "../Catalog/Catalog.h"
#include "../logical_query_plan/logical_operator.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamSortIterator.h"
/**
 * @brief The LogicalSort contains the information of attributes to be
 * sorted. And it describe how to generate Sort Operator.
 * @details LogicalSort have inner struct 'OrderByAttr' which contains the
 * tablename and direction of sort(ASC or DESC).
 * getIteratorTree() generates the logical plan.getIteratorTree() uses
 * getDataflow() to get the data from its child.
 * When we achieve sort logical plan, getOrderKey() help us return the column
 * number of the attributes we need to sort.
 */
class LogicalSort : public LogicalOperator {
 public:
  /**
   * @brief Description: OrderByAttr contains the table information and its sort
   * direction.We use ASC as default.
   */
  struct OrderByAttr {
    //  const char *tbl_;
    //  const char *attr_;
    const char *table_name_;
    int direction_;
    OrderByAttr(const char *table_name, int direction)
        : table_name_(table_name), direction_(direction) {
      //  printf("ta=%x len=%d ta_=%x len=%d\n",ta,strlen(ta),ta_,strlen(ta_));
      //  cout<<"order by att str ta=  "<<ta<<" @@@ta_= "<<ta_<<endl;
    }
    OrderByAttr(const char *table_name, const char *attribute_name) {}
  };

  /**
   * @brief Method description: Create a Sort operator.
   * @param LogicalOperator *child_. The pointer of its child.
   * @param vector<LogicalSort::OrderByAttr *> order_by_attr_. This contains the
   * attributes list to be sorted.
   */
  LogicalSort(LogicalOperator *child,
              vector<LogicalSort::OrderByAttr *> order_by_attr);
  virtual ~LogicalSort();
  /**
   * @brief Method description: Get the schema and child information.
   * @details The mostly important member is "property" and "attributes
   * list".
   */
  Dataflow GetDataflow();
  /**
   * @brief Method description: Generate logical query plan and return physical
   * operator instance.
   * @param const unsigned &blocksize.
   * @return BlockStreamIteratorBase *
   * @details  Besides do these initialization we notice that "Sort" is a block
   * operator, so we need to get all data from other nodes.
   */
  BlockStreamIteratorBase *GetIteratorTree(const unsigned &blocksize);
  /**
   * @brief Method description: Get the column number of the given table name
   * and attributes name.
   * @param const char*
   * @param const char*
   * @return  int
   */
  // TODO(yuyang): change const char* to string
  int GetOrderByKey(const char *, const char *);
  int GetOrderByKey(const char *);

  void PrintOrderByAttr() const;
  void Print(int level = 0) const;
  virtual bool GetOptimalPhysicalPlan(
      Requirement requirement, PhysicalPlanDescriptor &physical_plan_descriptor,
      const unsigned &block_size = 4096 * 1024) {}

 private:
  vector<OrderByAttr *> order_by_attr_;
  Dataflow child_dataflow_;
  LogicalOperator *child_;
};

#endif  // LOGICAL_QUERY_PLAN_LOGICAL_SORT_H_
