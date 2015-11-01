/*
 * Requirement.h
 *
 *  Created on: Jan 14, 2014
 *      Author: wangli
 */

#ifndef LOGICAL_OPERATOR_REQUIREMENT_H_
#define LOGICAL_OPERATOR_REQUIREMENT_H_
#include <vector>
#include "../common/ids.h"
#include "../common/hash.h"
#include "../Catalog/Attribute.h"
#include "../logical_operator/plan_context.h"
/**
 * This class describes what properties are required to the child data flow.
 */
namespace claims {
namespace logical_operator {

enum NetworkTransfer { NONE, Shuffle, piped };

class Requirement {
 public:
  Requirement();

  void setRequiredPartitionkey(const Attribute partition_key);
  Attribute getPartitionKey() const;
  bool hasReuiredPartitionKey() const;

  void setRequiredPartitionFucntion(PartitionFunction* partition);
  PartitionFunction* getPartitionFunction() const;
  bool hasRequiredPartitionFunction() const;

  void setRequiredLocations(std::vector<NodeID> location_list);
  std::vector<NodeID> getRequiredLocations() const;
  bool hasRequiredLocations() const;

  void setRequiredCost(const unsigned long cost);
  NetworkTransfer requireNetworkTransfer(const PlanContext& plan_context) const;
  bool passLimits(const unsigned long cost) const;
  virtual ~Requirement();

  /** try to merge the req if *this and req are compatible.
   *	return true if successful
   */
  bool tryMerge(const Requirement req, Requirement& target) const;

 private:
  /**
   * describe the desirable location for each partition.
   * empty list if there is no such requirement.
   */
  std::vector<NodeID> location_list_;

  /**
   * describe what partition function are required, including partition numbers
   * and
   * partition fashion(e.g., hash, range, etc.)
   * null if there is no such requirement
   */
  PartitionFunction* partition_function_;

  /**
   * describe the desirable partition attribute.
   * TODO: a set of desirable partition attributes.
   */
  Attribute partition_key_;

  /**
   * specify the acceptable cost for the child sub-plan.
   * zero if no limit is specified.
   */
  unsigned long cost_limit_;
};
}  // namespace logical_operator
}  // namespace claims
#endif  // LOGICAL_OPERATOR_REQUIREMENT_H_
