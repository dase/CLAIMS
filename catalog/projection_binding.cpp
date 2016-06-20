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
 * /Claims/catalog/projection_binding.cpp
 *
 *
 *  Created on: Nov 2, 2013
 *      Author: wangli
 *  Renamed on: Oct 26, 2015
 *      Author: yukai
 *       Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "../catalog/projection_binding.h"

#include <vector>
#include "../Environment.h"
#include "../utility/maths.h"

namespace claims {
namespace catalog {

ProjectionBinding::ProjectionBinding() {
  // TODO Auto-generated constructor stub
}

ProjectionBinding::~ProjectionBinding() {
  // TODO Auto-generated destructor stub
}

bool ProjectionBinding::BindingEntireProjection(
    Partitioner* part, const StorageLevel& desriable_storage_level) {
  if (part->get_binding_mode_() == OneToOne) {
    std::vector<std::pair<unsigned, NodeID> > partition_id_to_nodeid_list;
    ResourceManagerMaster* rmm =
        Environment::getInstance()->getResourceManagerMaster();
    std::vector<NodeID> node_id_list = rmm->getSlaveIDList();
    unsigned allocate_cur = 0;
    //    allocate_cur = GetRandomInt(node_id_list.size());
    for (unsigned i = 0; i < part->getNumberOfPartitions(); i++) {
      NodeID target = node_id_list[allocate_cur];

      /*
       * check whether target node has enough resource,
       * if not, target point to next one node,
       * till some one is suited, return true
       * or all is failed and return false
       */
      bool check_passed = false;
      unsigned buget = part->getPartitionDataSize(i);
      //      printf("_____BUDGET:%d\n",buget);
      unsigned failures = 0;
      while (!check_passed) {
        //        if(target==0){
        //          allocate_cur=(allocate_cur+1)%node_id_list.size();
        //          target=node_id_list[allocate_cur];
        //          continue;
        //        }
        switch (desriable_storage_level) {
          case MEMORY: {
            if (rmm->ApplyMemoryBuget(target, buget) == true) {
              check_passed = true;
              failures = 0;
            }
            break;
          }
          case DISK: {
            if (rmm->ApplyDiskBuget(target, buget) == true) {
              check_passed = true;
              failures = 0;
            }
            break;
          }
          case HDFS: {
            check_passed = true;
            failures = 0;
            break;
          }
          default: { break; }
        }

        if (!check_passed) {
          allocate_cur = (allocate_cur + 1) % node_id_list.size();
          target = node_id_list[allocate_cur];
          failures++;
          if (failures >= node_id_list.size()) {
            /* none of the available Slave could meet the budget;
             * TODO: 1.Roll back the allocated budget for other partitions.
             *     2.The partitioner may need to consider OneToMany mode so
             *      that a single partition can be bound to multiple nodes.
             * */
            printf("binding fails! not enough resource!\n");
            return false;
          }
        }
      }

      /* store the binding information in the list*/
      partition_id_to_nodeid_list.push_back(
          std::pair<unsigned, NodeID>(i, target));

      allocate_cur = (allocate_cur + 1) % node_id_list.size();
      /*bind*/
      //      part->bindPartitionToNode(i,node_id_list[allocate_cur]);
      //
      //
      //      BlockManagerMaster::getInstance()->SendBindingMessage(partition_id,number_of_chunks,MEMORY,target);
    }
    /* conduct the binding according to the bingding information list*/
    for (unsigned i = 0; i < partition_id_to_nodeid_list.size(); i++) {
      const unsigned partition_off = partition_id_to_nodeid_list[i].first;
      const NodeID node_id = partition_id_to_nodeid_list[i].second;
      /* update the information in Catalog*/
      part->bindPartitionToNode(partition_off, node_id);

      /* notify the StorageManger of the target node*/
      PartitionID partition_id(part->getProejctionID(), partition_off);
      const unsigned number_of_chunks = part->getPartitionChunks(partition_off);
      BlockManagerMaster::getInstance()->SendBindingMessage(
          partition_id, number_of_chunks, desriable_storage_level, node_id);
    }
    return true;
  }

  /**
   * TODO: OneToMany mode where one partition is allowed to be bound to multiple
   * nodes.
   */
  return false;
}

bool ProjectionBinding::UnbindingEntireProjection(Partitioner* part) {
  if (part->get_binding_mode_() == OneToOne) {
    ResourceManagerMaster* rmm =
        Environment::getInstance()->getResourceManagerMaster();
    for (int i = 0; i < part->getNumberOfPartitions(); i++) {
      unsigned budget = part->getPartitionDataSize(i);
      switch (DESIRIABLE_STORAGE_LEVEL) {
        case MEMORY: {
          rmm->ReturnMemoryBuget(part->getPartitionLocation(i), budget);
          break;
        }
        case DISK: {
          rmm->ReturnDiskBuget(part->getPartitionLocation(i), budget);
          break;
        }
        default:
          break;
      }

      PartitionID partition_id(part->getProejctionID(), i);
      NodeID node_id = part->getPartitionLocation(i);
      BlockManagerMaster::getInstance()->SendUnbindingMessage(partition_id,
                                                              node_id);
      part->unbindPartitionToNode(i);
    }
    return true;
  }
  /**
   * TODO: OneToMany mode where one partition is allowed to be bound to multiple
   * nodes.
   */
  return false;
}

} /* namespace catalog */
} /* namespace claims */
