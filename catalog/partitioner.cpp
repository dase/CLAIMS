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
 * /Claims/catalog/partitioner.cpp
 *
 *  Created on: 2013-10-14
 *      Author: wangli
 *  Renamed on: Oct 26, 2015
 *      Author: yukai
 *       Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "../catalog/partitioner.h"

#include <assert.h>
#include <stdio.h>
#include <cmath>
#include <vector>

#include "../catalog/catalog.h"
#include "../Environment.h"
#include "../utility/print_tool.h"

namespace claims {
namespace catalog {

const int CHUNKSIZE_IN_MB = 64;
const int BLOCKSIZE_IN_KB = 64;

Partitioner::Partitioner(ProjectionID projection_id,
                         unsigned number_of_partitions,
                         PartitionFunction* partitioning_functin)
    : projection_id_(projection_id),
      number_of_partitions_(number_of_partitions),
      partition_key_(0),
      partition_function_(partitioning_functin),
      mode_(OneToOne) {}
Partitioner::Partitioner(ProjectionID projection_id,
                         unsigned number_of_partitions,
                         const Attribute& partition_key,
                         PartitionFunction* partitioning_function)
    : projection_id_(projection_id),
      number_of_partitions_(number_of_partitions),
      partition_function_(partitioning_function),
      mode_(OneToOne) {
  partition_key_ = new Attribute(partition_key);
  for (unsigned i = 0; i < number_of_partitions; i++) {
    PartitionID pid(projection_id_, i);
    partition_info_list.push_back(new OneToOnePartitionInfo(pid));
  }
}

Partitioner::~Partitioner() {
  // TODO Auto-generated destructor stub
}
unsigned Partitioner::getNumberOfPartitions() const {
  return partition_function_->getNumberOfPartitions();
}

bool Partitioner::bindPartitionToNode(PartitionOffset partition_id,
                                      NodeID target_node) {
  return partition_info_list[partition_id]->bind_all_blocks(target_node);
}

void Partitioner::unbindPartitionToNode(PartitionOffset partition_id) {
  partition_info_list[partition_id]->unbind_all_blocks();
}

void Partitioner::RegisterPartition(unsigned partition_key,
                                    unsigned number_of_chunks) {
  assert(partition_key < partition_function_->getNumberOfPartitions());

  partition_info_list[partition_key]->hdfs_file_name =
      partition_info_list[partition_key]->partition_id_.getName();
  partition_info_list[partition_key]->number_of_blocks =
      number_of_chunks * 1024;
}

void Partitioner::RegisterPartitionWithNumberOfBlocks(
    unsigned partition_offset, unsigned long number_of_blocks) {
  assert(partition_offset < partition_function_->getNumberOfPartitions());

  partition_info_list[partition_offset]->hdfs_file_name =
      partition_info_list[partition_offset]->partition_id_.getName();
  partition_info_list[partition_offset]->number_of_blocks = number_of_blocks;
}

void Partitioner::UpdatePartitionWithNumberOfChunksToBlockManager(
    unsigned partition_offset, unsigned long number_of_blocks) {
  assert(partition_offset < partition_function_->getNumberOfPartitions());

  unsigned number_of_chunks = ceil((number_of_blocks) / (float)1024);
  if (mode_ == OneToOne) {
    if (partition_info_list[partition_offset]->is_all_blocks_bound()) {
      NodeID node_id = partition_info_list[partition_offset]->get_location();
      BlockManagerMaster::getInstance()->SendBindingMessage(
          partition_info_list[partition_offset]->partition_id_,
          number_of_chunks, MEMORY, node_id);
    }
  } else {
    // TODO(AnyOne): determine the OneToMany binding's append NodeID
    cout << "Need to be done: determine the OneToMany binding append NodeID!\n";
    assert(false);
  }
}

void Partitioner::print() {
  //  hashmap<PartitionID,NodeID>::iterator it=partitionid_to_nodeid_.begin();
  //  while(it!=partitionid_to_nodeid_.end()){
  //    printf("<%d, %d>\n",it->first,it->second);
  //    it++;
  //  }
}
bool Partitioner::hasSamePartitionLocation(
    const Partitioner& target_partition) const {
  if (mode_ == OneToMany || target_partition.get_binding_mode_() == OneToMany) {
    /** in the current version, any the location detection in OneToMany mode is
     * ommited.*/
    return false;
  }
  if (getNumberOfPartitions() != target_partition.getNumberOfPartitions())
    return false;
  for (unsigned i = 0; i < getNumberOfPartitions(); i++) {
    if (!partition_info_list[i]->is_colocated(
            *target_partition.partition_info_list[i])) {
      return false;
    }
  }
  return true;
}
unsigned Partitioner::getPartitionDataSize(unsigned partitoin_index) const {
  return partition_info_list[partitoin_index]->number_of_blocks *
         BLOCKSIZE_IN_KB / 1024;
}
unsigned long Partitioner::getPartitionCardinality(
    unsigned partition_index) const {
  return partition_info_list[partition_index]->number_of_tuples_;
}
unsigned Partitioner::getPartitionBlocks(unsigned partitoin_index) const {
  return partition_info_list[partitoin_index]->number_of_blocks;
}
unsigned Partitioner::getPartitionChunks(unsigned partition_index) const {
  return ceil(partition_info_list[partition_index]->number_of_blocks / 1024.0f);
}
NodeID Partitioner::getPartitionLocation(unsigned partition_offset) const {
  if (partition_info_list[partition_offset]->get_mode() == OneToOne) {
    return partition_info_list[partition_offset]->get_location();
  } else {
    return -1;
  }
}

PartitionFunction::PartitionStyle Partitioner::getPartitionFashion() const {
  return partition_function_->getPartitionFashion();
}
Attribute Partitioner::getPartitionKey() const { return *partition_key_; }
PartitionFunction* Partitioner::getPartitionFunction() const {
  return partition_function_;
}
ProjectionID Partitioner::getProejctionID() const { return projection_id_; }
bool Partitioner::allPartitionBound() const {
  for (unsigned i = 0; i < number_of_partitions_; i++) {
    if (!partition_info_list[i]->is_all_blocks_bound()) {
      return false;
    }
  }
  return true;
}

vector<PartitionID> Partitioner::getPartitionIDList() {
  vector<PartitionID> ret;
  ret.clear();
  for (vector<PartitionInfo*>::iterator iter = partition_info_list.begin();
       iter != partition_info_list.end(); iter++)
    ret.push_back((*iter)->partition_id_);
  return ret;
}
bool OneToOnePartitionInfo::is_colocated(const PartitionInfo& target) const {
  if (target.get_mode() == OneToMany) return false;
  if (binding_node_id_ == -1 ||
      ((OneToOnePartitionInfo*)&target)->binding_node_id_ == -1)
    return false;
  return binding_node_id_ ==
         ((OneToOnePartitionInfo*)&target)->binding_node_id_;
}
bool OneToManyPartitionInfo::is_colocated(const PartitionInfo& target) const {
  return false;
}

} /* namespace catalog */
} /* namespace claims */
