/*
 * LogicalQueryPlanRoot.h
 *
 *  Created on: Nov 13, 2013
 *      Author: wangli
 */

#ifndef LOGICALQUERYPLANROOT_H_
#define LOGICALQUERYPLANROOT_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "./LogicalOperator.h"
#include "../common/ids.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
struct LimitConstraint {
  /* the upper bound on the number of tuples returned*/
  unsigned long returned_tuples_;
  /* the start position of the first returned tuples.
   * Note: the index starts from 0;
   */
  unsigned long start_position_;
  LimitConstraint(unsigned long return_tuples)
      : returned_tuples_(return_tuples),
        start_position_(0) {
  };
  LimitConstraint(unsigned long return_tuples, unsigned long position)
      : returned_tuples_(return_tuples),
        start_position_(position) {
  };
  LimitConstraint() : returned_tuples_(-1), start_position_(0) { }
  /* return if the limit constraint can be omitted.*/
  bool canBeOmitted() const {
    return returned_tuples_ == -1 & start_position_ == 0;
  }
};

/***
 * @brief: top logical operation in logical query plan,
 * to generate a few top physical operation.
 */
class LogicalQueryPlanRoot : public LogicalOperator {
 public:
  /* three style decide which one the top physical operation is:
   * BlockStreamPrint/BlockStreamPerformanceMonitorTop/BlockStreamResultCollector
   */
  enum outputFashion {
    PRINT,
    PERFORMANCE,
    RESULTCOLLECTOR
  };
  LogicalQueryPlanRoot(NodeID collecter, LogicalOperator* child,
                       const outputFashion& fashion = PERFORMANCE,
                       LimitConstraint limit_constraint = LimitConstraint());
  virtual ~LogicalQueryPlanRoot();
  Dataflow getDataflow();
  BlockStreamIteratorBase* getIteratorTree(const unsigned&);
  bool GetOptimalPhysicalPlan(Requirement requirement,
                              PhysicalPlanDescriptor& physical_plan_descriptor,
                              const unsigned & block_size = 4096 * 1024);
  void print(int level = 0) const;

 private:
  std::vector<std::string> getAttributeName(const Dataflow& dataflow) const;

 private:
  NodeID collecter_;
  LogicalOperator* child_;
  outputFashion fashion_;
  LimitConstraint limit_constraint_;
};

#endif /* LOGICALQUERYPLANROOT_H_ */
