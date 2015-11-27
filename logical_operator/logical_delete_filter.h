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
 * /CLAIMS/logical_operator/equal_join.h
 *
 *  Created on: Nov 7, 2015
 *      Author: minqi
 *       Email: mqzhou at sei dot ecnu dot edu dot cn
 *
 * Description:
 *   This file mainly describe the EqualJoin Operator.
 *
 */
#ifndef LOGICAL_OPERATOR_LOGICAL_DELETE_FILTER_H_
#define LOGICAL_OPERATOR_LOGICAL_DELETE_FILTER_H_
#include <vector>
#include "../catalog/attribute.h"
#include "../logical_operator/logical_operator.h"
#include "../physical_operator/physical_sort.h"

namespace claims {
namespace logical_operator {

/**
 * @brief Generate the delete filter plan, which filters the deleted tuple based
 * on the del table.
 * @details The delete filter is very similar to the EqualJoin operator fills
 * the join statement. It joins the base table and the deleted table together to
 * filter out the deleted tuples. The return of the delete filter is different
 * from that of the equal join， and its output schema is the base table, and
 * output tuples are all from the base table without tuples in the delete table.
 * Basically, the partition key in the delete table may be the same as those in
 * the base table. In such cases, all the delete filter will be executed on
 * local node. If not (in cases when users set the partition key of the base
 * table to the be the key not of the row_id), the delete filter have to add the
 * exchange operator as its child. The delete filter use the equal condition on
 * the row_id to filter out the deleted tuples. If exchange operator is added,
 * the delete table will be repartitioned.
 *
 */
class LogicalDeleteFilter : public LogicalOperator {
 public:
  /**
   * @brief The JoinPair contains two attributes to be joined.
   */
  struct FilterPair {
    FilterPair(const Attribute& left_filter_attr,
               const Attribute& right_filter_attr)
        : left_filter_attr_(left_filter_attr),
          right_filter_attr_(right_filter_attr) {}
    Attribute left_filter_attr_;
    Attribute right_filter_attr_;
  };

  /**
   * @brief the deleted table is always smaller than the base table, thus there
   * are only two filter policy
   * however, the system needs to deal with all kinds of partition scenarios,
   * thus we include other repartition policies
   * since the deleted table always partitioned on the basetable_row_id key,
   * that's to say
   * if the base table is partitioned on the row_id, then it will use the no
   * repartition policy.
   * if the base table is partitioned on the other keys, then the deleted table
   * will be broadcast to the all the node which has the partitions of the base
   * table.
   */
  enum FilterPolicy {
    kNull,                 //!< kNull
    kNoRepartition,        //!< kNoRepartition
    kLeftRepartition,      //!< kLeftRepartition
    kRightRepartition,     //!< krightRepartition
    kCompleteRepartition,  //!< kCompleteRepartition
    kLeftBroadcast         //!< kLeftBroadcast
  };

 public:
  /**
   * @brief  Method description:Create a EqualJoin implement.
   * @param  std::vector<JoinPair>
   * @param  LogicalOperator* left_input
   * @param  LogicalOperator* right_input
   */
  LogicalDeleteFilter(std::vector<FilterPair>, LogicalOperator* left_input,
                      LogicalOperator* right_input);
  virtual ~LogicalDeleteFilter();
  /**
   * @brief Method description: Get the child information.
   * @details The mostly important member is "property" and "attributes
   * list". When getting data from child, we make some choices according to
   * JoinPolice. JoinPolice is generated from
   * canLeverageHashPartition(), isEqualCondition(), hasSamePartition().
   * If all of them is true, it means partition_key in the join_list and two
   * partition_keys in the same join_pair.Besides, and they have partitioned to
   * the same machine.So we don't need repartition, and JoinPolice =
   * kNodeRepartition.
   * If both canLeverageHashPartition()s are true, but isEqualCondition() or
   * hasSamePartition() is false, the machine which has smaller data will send
   * its data to another according to decideLeftOrRightRepartition().
   * If canLeverageHashPartition(left) is true but canLeverageHashPartition
   * (right) is false, right nodes will be repartitioned.
   * join_police_ = kRightRepartition
   * If canLeverageHashPartition(left) is false but canLeverageHashPartition
   * (right) is true, left nodes will be repartitioned.
   * oin_police_ = kLeftRepartition
   * If both canLeverageHashPartition()s are false, left and right nodes will be
   * repartitioned.
   * join_police_ = kCompleteRepartition
   */
  PlanContext GetPlanContext();
  PhysicalOperatorBase* GetPhysicalPlan(const unsigned& blocksize);
  bool GetOptimalPhysicalPlan(Requirement requirement,
                              PhysicalPlanDescriptor& physical_plan_descriptor,
                              const unsigned& block_size = 4096 * 1024);

 private:
  std::vector<unsigned> GetLeftFilterKeyIds() const;
  std::vector<unsigned> GetRightFilterKeyIds() const;
  std::vector<unsigned> GetLeftPayloadIds() const;
  std::vector<unsigned> GetRightPayloadIds() const;
  int GetIdInLeftFilterKeys(const Attribute&) const;
  int GetIdInLeftFilterKeys(
      const Attribute&,
      const std::vector<Attribute> shadow_attribute_list) const;
  int GetIdInRightFilterKeys(const Attribute&) const;
  int GetIdInRightFilterKeys(
      const Attribute&,
      const std::vector<Attribute> shadow_attribute_list) const;
  int GetIdInAttributeList(const std::vector<Attribute>& attributes,
                           const Attribute&) const;
  bool IsHashOnLeftKey(const Partitioner& part, const Attribute& key) const;

  /**
   * @brief Method description:Check whether the partitioning is based on hash
   * and the hash key is a subset of the join keys such that hash join is
   * enabled.
   * @param const std::vector<Attribute>& partition_key_list
   * @param const DataflowPartitioningDescriptor& partitoiner
   * @return bool
   */
  bool CanOmitHashRepartition(const std::vector<Attribute>& join_key_list,
                              const PlanPartitioner& partitoiner) const;
  /**
   * @brief Method description:Check whether two partition_keys in the same
   * join_pair.
   * @param const Attribute& left
   * @param const Attribute& right
   * @return  bool
   */
  bool IsInOneFilterPair(const Attribute& left_partition_key,
                         const Attribute& right_partition_key) const;

  /**
   * @brief Method description:Check which has the smaller data.
   * @details Current version only consider the data size for simplicity.
   * @param const Dataflow& left_dataflow
   * @param const Dataflow& right_dataflow
   * @return JoinPolice
   * TODO(admin): Consider not only data size but also other factors, such as
   * parallelism, resource, etc.
   */
  FilterPolicy DecideLeftOrRightRepartition(
      const PlanContext& left_dataflow,
      const PlanContext& right_dataflow) const;

  PlanPartitioner DecideOutputDataflowProperty(
      const PlanContext& left_dataflow,
      const PlanContext& right_dataflow) const;
  void Print(int level = 0) const;

  /**
   * Assuming that R and S are the two join table, the selectivity is
   * the number of tuples generated by the join operator to the number of
   * |R|*|S|.
   */
  double PredictFilterSelectivity(const PlanContext& left_dataflow,
                                  const PlanContext& right_dataflow) const;

  /**
   * assuming that R ane S are the two join table, and the join condition is
   * R.x=S.x.
   * return |O|, where |O|=|R.x=x1|*|S.x=x1|+|R.x=x2|*|S.x=x2|+......
   */
  double PredictFilterSelectivityOnSingleJoinAttributePair(
      const Attribute& attr_left, const Attribute& attr_right) const;

 private:
  std::vector<FilterPair> filterkey_pair_list_;
  std::vector<Attribute> left_filter_key_list_;
  std::vector<Attribute> right_filter_key_list_;
  LogicalOperator* left_child_;
  LogicalOperator* right_child_;
  FilterPolicy filter_policy_;
  PlanContext* dataflow_;
};
}  // namespace logical_operator
}  // namespace claims
#endif  // LOGICAL_OPERATOR_LOGICAL_DELETE_FILTER_H_
