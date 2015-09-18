/*
 * Filter.h
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#ifndef FILTER_H_
#define FILTER_H_
#include <vector>
#include <map>
#ifdef DMALLOC
#include "./dmalloc.h"
#endif
#include "LogicalOperator.h"
#include "../common/AttributeComparator.h"
#include "../common/ExpressionCalculator.h"
#include "../common/ExpressionItem.h"
#include "../common/Expression/qnode.h"
class Filter : public LogicalOperator {
 public:
  Filter(LogicalOperator* child, vector<QNode*> qual);
  Filter(std::vector<AttributeComparator> ComparatorList,
         LogicalOperator* child);
  virtual ~Filter();
  Dataflow getDataflow();
  BlockStreamIteratorBase* getIteratorTree(const unsigned& blocksize);
  bool GetOptimalPhysicalPlan(Requirement requirement,
                              PhysicalPlanDescriptor& physical_plan_descriptor,
                              const unsigned& block_size = 4096 * 1024);
  void print(int level = 0) const;

 private:
  bool couldHashPruned(unsigned partition_id,
                       const DataflowPartitioningDescriptor&) const;
  float predictSelectivity() const;
  bool getcolindex(Dataflow dataflow);  // get column index

 private:
  LogicalOperator*
      child_;  //<子逻辑算子，对于filter来说只有scan，需要从它那里获取Dataflow和Iterator
  // Tree
  vector<AttributeComparator> comparator_list_;  //<存放比较方法
  map<string, int>
      colindex_;  //<该filter会用到表的第几列及其属性名，放入Dataflow和Iterator
  // Tree中传给上层
  vector<QNode*>
      qual_;  //<qualification的简称，QNode是node的一个扩展，node是这个项目的一个基本类型，QNode可以负责为物理计划初始化表达式；在这里而言，也会放入Dataflow和Iterator
              // Tree中传给上层
};

#endif /* FILTER_H_ */
