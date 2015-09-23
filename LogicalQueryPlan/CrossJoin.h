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
 * /libeasy/include/afda.h
 *
 *  Created on: Jul 14, 2014
 *      Author: minqi, wangli1426, fzhedu
 *       Email: mqzhou@sei.ecnu.edu.cn
 *
 * Description: Generate the dataflow and the physical operator for the cross join logical plan
 *
 */

#ifndef LOGICALQUERYPLAN_CROSSJOIN_H_
#define LOGICALQUERYPLAN_CROSSJOIN_H_

#include "LogicalOperator.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"


namespace claims {
namespace logical_query_plan {
/***
 * @brief
 */

class CrossJoin : public LogicalOperator {
 public:
  CrossJoin();
  CrossJoin(LogicalOperator* left_input, LogicalOperator* right_input);
  virtual ~CrossJoin();
  Dataflow getDataflow();
  BlockStreamIteratorBase* getIteratorTree(const unsigned& blocksize);
  int GetJoinPolice();

  void print(int level = 0) const;

 protected:
  enum JoinPloice {
    kLeftBroadcast = 1,
    kRightBroadcast,
    kLocalJoin
  };

 private:
  int DecideJoinPolice(const Dataflow &left, const Dataflow &right);
  /** return true if the nest-loop-join can be conducted locally**/
  bool CanLocalJoin(const Dataflow &left, const Dataflow &right) const;
  int GenerateChildPhysicalQueryPlan(
      BlockStreamIteratorBase* & left_child_iterator_tree,
      BlockStreamIteratorBase* & right_child_iterator_tree,
      const unsigned & blocksize);

  LogicalOperator* left_child_;
  LogicalOperator* right_child_;
  Dataflow* dataflow_;
  JoinPloice join_police_;
};
}   // namespace logical_query_plan
}   // namespace claims
#endif // LOGICALQUERYPLAN_CROSSJOIN_H_
