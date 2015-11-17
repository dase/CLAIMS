/*
 * Requirement.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: wangli
 */
#include "../logical_operator/Requirement.h"

#include <assert.h>

#include "../logical_operator/plan_context.h"

namespace claims {
namespace logical_operator {
Requirement::Requirement() : partition_function_(0), cost_limit_(0) {
  // TODO Auto-generated constructor stub
}

Requirement::~Requirement() {
  // TODO Auto-generated destructor stub
}

void Requirement::setRequiredPartitionkey(const Attribute partition_key) {
  partition_key_ = partition_key;
}
Attribute Requirement::getPartitionKey() const { return partition_key_; }
bool Requirement::hasReuiredPartitionKey() const {
  return !partition_key_.isNULL();
}

void Requirement::setRequiredPartitionFucntion(PartitionFunction* partition) {
  partition_function_ = partition;
}
PartitionFunction* Requirement::getPartitionFunction() const {
  return partition_function_;
}
bool Requirement::hasRequiredPartitionFunction() const {
  return partition_function_ != 0;
}

void Requirement::setRequiredLocations(std::vector<NodeID> location_list) {
  location_list_ = location_list;
}
std::vector<NodeID> Requirement::getRequiredLocations() const {
  return location_list_;
}
void Requirement::setRequiredCost(const unsigned long cost) {
  cost_limit_ = cost;
}
NetworkTransfer Requirement::requireNetworkTransfer(
    const PlanContext& plan_context) const {
  NetworkTransfer ret;
  /* whether the plan_context is partitioned on desirable key*/
  bool right_partition_key = true;
  bool right_partition_function = true;
  if (!partition_key_.isNULL()) {
    /**there is partition key requirement*/
    if (partition_key_ != plan_context.plan_partitioner_.get_partition_key()) {
      /* the partition key is not match*/
      right_partition_key = false;
    } else {
      /* partition key matches and now test the partition numbers*/
      if (partition_function_ != 0) {
        if (!partition_function_->equal(
                plan_context.plan_partitioner_.get_partition_func())) {
          right_partition_function = false;
        }
      } else {
      }
    }
  }

  if (right_partition_key && right_partition_function) {
    if (!location_list_.empty()) {
      /* there is requirememt for locations*/
      bool same_locations = true;
      for (unsigned i = 0; i < location_list_.size(); i++) {
        if (location_list_[i] !=
            plan_context.plan_partitioner_.GetPartition(i)->get_location()) {
          same_locations = false;
          break;
        }
      }
      if (same_locations)
        ret = NONE;
      else
        ret = piped;

    } else {
      ret = NONE;
    }
  } else {
    ret = Shuffle;
  }

  return ret;
}

bool Requirement::passLimits(const unsigned long cost) const {
  if (cost_limit_ == 0)
    return true;
  else
    return cost_limit_ >= cost;
}
bool Requirement::hasRequiredLocations() const {
  return !location_list_.empty();
}

bool isLocationsCompatible(const std::vector<NodeID> l,
                           const std::vector<NodeID> r) {
  if (l.size() != r.size()) return false;
  for (unsigned i = 0; i < l.size(); i++) {
    if (l[i] != r[i]) return false;
  }
  return true;
}

bool Requirement::tryMerge(const Requirement req, Requirement& target) const {
  if (!this->getPartitionKey().isNULL() && !req.getPartitionKey().isNULL()) {
    if (this->getPartitionKey() != req.getPartitionKey())
      return false;
    else
      target.setRequiredPartitionkey(req.getPartitionKey());
  } else if (!this->getPartitionKey().isNULL()) {
    target.setRequiredPartitionkey(this->getPartitionKey());
  } else if (!req.getPartitionKey().isNULL()) {
    target.setRequiredPartitionkey(req.getPartitionKey());
  }

  /* if the code arrives here, the partition key is compatible.*
   * Then we need to check the number of partitions and locations
   */

  /*test the number of partitions*/
  if (this->hasRequiredPartitionFunction() &&
      req.hasRequiredPartitionFunction()) {
    if (!(this->getPartitionFunction()->equal(req.getPartitionFunction())))
      return false;
    else
      target.setRequiredPartitionFucntion(req.getPartitionFunction());
  } else if (this->hasRequiredPartitionFunction()) {
    target.setRequiredPartitionFucntion(this->getPartitionFunction());
  } else if (req.hasRequiredPartitionFunction()) {
    target.setRequiredPartitionFucntion(req.getPartitionFunction());
  }

  /* test the locations*/
  if (this->hasRequiredLocations() && req.hasRequiredLocations()) {
    if (!isLocationsCompatible(this->getRequiredLocations(),
                               req.getRequiredLocations()))
      return false;
    else
      target.setRequiredLocations(this->getRequiredLocations());
  } else {
    target.setRequiredLocations(this->hasRequiredLocations()
                                    ? this->getRequiredLocations()
                                    : req.getRequiredLocations());
  }

  return true;
}
}  // namespace logical_operator
}  // namespace claims
