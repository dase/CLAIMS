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
 * /CLAIMS/LogicalQueryPlan/logical_sort.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *
 * Description:
 *   This file mainly describe the Sort Operater.
 *
 */

#include "./logical_sort.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../Config.h"
#include "../IDsGenerator.h"
LogicalSort::LogicalSort(LogicalOperator *child,
                         std::vector<LogicalSort::OrderByAttr *> order_by_attr)
    : child_(child), order_by_attr_(order_by_attr) {}

LogicalSort::~LogicalSort() {
  if (NULL != child_) {
    delete child_;
    child_ = NULL;
  }
}

Dataflow LogicalSort::GetDataflow() {
  // Get the information from its child
  child_dataflow_ = child_->GetDataflow();
  Dataflow ret;
  ret.attribute_list_ = child_dataflow_.attribute_list_;
  ret.property_.commnication_cost = child_dataflow_.property_.commnication_cost;
  ret.property_.partitioner.setPartitionFunction(
      child_dataflow_.property_.partitioner.getPartitionFunction());
  ret.property_.partitioner.setPartitionKey(Attribute());

  NodeID location = 0;
  unsigned long data_cardinality = 0;
  PartitionOffset offset = 0;
  DataflowPartition par(offset, data_cardinality, location);
  vector<DataflowPartition> partition_list;
  partition_list.push_back(par);
  ret.property_.partitioner.setPartitionList(partition_list);
  return ret;
}

BlockStreamIteratorBase *LogicalSort::GetIteratorTree(
    const unsigned &blocksize) {
  Dataflow dataflow = GetDataflow();

  // Get all of the data from other nodes if needed.
  BlockStreamExpander::State expander_state;
  expander_state.block_count_in_buffer_ = EXPANDER_BUFFER_SIZE;
  expander_state.block_size_ = blocksize;
  expander_state.init_thread_count_ = Config::initial_degree_of_parallelism;
  expander_state.child_ = child_->GetIteratorTree(blocksize);
  expander_state.schema_ = GetSchema(child_dataflow_.attribute_list_);
  BlockStreamIteratorBase *expander_lower =
      new BlockStreamExpander(expander_state);

  ExpandableBlockStreamExchangeEpoll::State exchange_state;
  exchange_state.block_size_ = blocksize;
  exchange_state.child_ = expander_lower;
  exchange_state.exchange_id_ =
      IDsGenerator::getInstance()->generateUniqueExchangeID();
  exchange_state.schema_ = GetSchema(child_dataflow_.attribute_list_);
  vector<NodeID> lower_id_list =
      GetInvolvedNodeID(child_dataflow_.property_.partitioner);
  exchange_state.lower_id_list_ = lower_id_list;  // upper
  exchange_state.partition_schema_ = partition_schema::set_hash_partition(0);
  // TODO(admin): compute the upper_ip_list to do reduce side sort
  vector<NodeID> upper_ip_list;
  upper_ip_list.push_back(0);
  exchange_state.upper_id_list_ = upper_ip_list;  // lower
  BlockStreamIteratorBase *exchange =
      new ExpandableBlockStreamExchangeEpoll(exchange_state);

  BlockStreamSortIterator::State reducer_state;
  reducer_state.block_size_ = blocksize;
  reducer_state.child_ = exchange;
  // Actually we just need the column number in the end.
  for (unsigned i = 0; i < order_by_attr_.size(); i++) {
    reducer_state.orderbyKey_.push_back(
        GetOrderByKey(order_by_attr_[i]->table_name_));
    reducer_state.direction_.push_back(order_by_attr_[i]->direction_);
  }
  reducer_state.input_ = GetSchema(child_dataflow_.attribute_list_);
  BlockStreamIteratorBase *reducer_sort =
      new BlockStreamSortIterator(reducer_state);

  return reducer_sort;
}

int LogicalSort::GetOrderByKey(const char *table_name, const char *attr) {
  // Use table name and attribute name to get the number.
  for (unsigned attr_id = 0; attr_id < child_dataflow_.attribute_list_.size();
       attr_id++) {
    TableDescriptor *table = Catalog::getInstance()->getTable(
        child_dataflow_.attribute_list_[attr_id].table_id_);
    string tablename = table->getTableName();
    if ((tablename.compare(table_name) == 0) &&
        (child_dataflow_.attribute_list_[attr_id].attrName.compare(attr) == 0)) {
      return attr_id;
    }
  }
}

int LogicalSort::GetOrderByKey(const char *table_name) {
  for (unsigned attr_id = 0; attr_id < child_dataflow_.attribute_list_.size();
       attr_id++) {
    string _tablename(table_name);
    if (_tablename.compare(child_dataflow_.attribute_list_[attr_id].attrName) == 0) {
      return attr_id;
    }
  }
}
void LogicalSort::PrintOrderByAttr() const {
  cout << "OrderByAttr:" << endl;
  for (int i = 0; i < order_by_attr_.size(); i++) {
    printf("%s  %s\n", (const char *)order_by_attr_[i]->table_name_,
           order_by_attr_[i]->direction_ == 0 ? "ASC" : "DESC");
  }
}
void LogicalSort::Print(int level) const {
  PrintOrderByAttr();
  child_->Print(level + 1);
}
