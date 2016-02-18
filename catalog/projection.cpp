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
 * /Claims/catalog/projection.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "../catalog/projection.h"
#include <vector>
#include <string>

#include "../catalog/partitioner.h"
#include "../common/Schema/Schema.h"
#include "../common/Schema/SchemaFix.h"
namespace claims {
namespace catalog {
ProjectionDescriptor::ProjectionDescriptor(ProjectionID pid)
    : projection_id_(pid) {}

ProjectionDescriptor::ProjectionDescriptor(const string& name)
    : Projection_name_(name) {
  // Check if hdfsfile already exists
  // Meanwhile create a hsfsfile to store this column's data.
}

ProjectionDescriptor::~ProjectionDescriptor() {}
void ProjectionDescriptor::addAttribute(Attribute attr) {
  const ColumnID cid(projection_id_, column_list_.size());
  const Column col(attr, cid);
  column_list_.push_back(col);
}
bool ProjectionDescriptor::hasAttribute(const Attribute& attr) const {
  for (unsigned i = 0; i < column_list_.size(); i++) {
    if (column_list_[i].index == attr.index) return true;
  }
  return false;
}

void ProjectionDescriptor::DefinePartitonier(
    const unsigned& number_of_partitions, const Attribute& partition_key,
    PartitionFunction* partition_functin) {
  partitioner = new Partitioner(projection_id_, number_of_partitions,
                                partition_key, partition_functin);
}
Partitioner* ProjectionDescriptor::getPartitioner() const {
  return partitioner;
}
bool ProjectionDescriptor::isExist(const string& name) const {
  for (unsigned i = 0; i < column_list_.size(); i++) {
    if (column_list_[i].attrName == name) return true;
  }
  return false;
}
bool ProjectionDescriptor::AllPartitionBound() const {
  return partitioner->allPartitionBound();
}
std::vector<Attribute> ProjectionDescriptor::getAttributeList() const {
  std::vector<Attribute> ret;
  for (unsigned i = 0; i < this->column_list_.size(); i++) {
    ret.push_back((Attribute)column_list_[i]);
  }
  return ret;
}
Schema* ProjectionDescriptor::getSchema() const {
  /**
   * Only fixed schema is supported now.
   * TODO: support other schema.
   */
  const vector<Attribute> attributes = getAttributeList();
  std::vector<column_type> columns;
  for (unsigned i = 0; i < attributes.size(); i++) {
    columns.push_back(*attributes[i].attrType);
  }
  return new SchemaFix(columns);
}
int ProjectionDescriptor::getAttributeIndex(const Attribute& att) const {
  const vector<Attribute> attributes = getAttributeList();
  for (unsigned i = 0; i < attributes.size(); i++) {
    if (attributes[i] == att) {
      return i;
    }
  }
  return -1;
}

long int ProjectionDescriptor::getNumberOfTuplesOnPartition(
    const unsigned partition_off) const {
  return partitioner->getPartitionCardinality(partition_off);
}

unsigned int ProjectionDescriptor::getProjectionCost() const {
  unsigned int sum = 0;
  for (auto it = column_list_.begin(); it != column_list_.end(); ++it) {
    sum += it->attrType->get_length();
  }

  return sum;
}
} /* namespace catalog */
} /* namespace claims */
