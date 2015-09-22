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
 * /Claims/LogicalQueryPlan/Limit.h
 *
 *  Created on: Sep 18, 2015
 *      Author: Han,wangli
 *		   Email: chxiaoyifeng1992@gmail.com
 *
 * Description:
 *
 *
 *
 */

#ifndef LOGICALQUERYPLAN_LIMIT_H_
#define LOGICALQUERYPLAN_LIMIT_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "LogicalOperator.h"
// namespace claims {
// namespace logical_query_plan {

/**
 * @brief Method description:limit operator is a simple operation.It get
 * @param start_position_:the fault value is 0,it means the position of
 * acquiring metedata.return_tuples_:the fault value is -1,it means which row is
 * needed.
 * @return values of start_positions and returned_tuples.
 */

class LimitConstraint {
 public:
  LimitConstraint(unsigned long return_tuples);
  LimitConstraint(unsigned long return_tuples, unsigned long position);
  // TODO(Han):The positions of parameters is reverse, we should make sure where
  // the function called.
  LimitConstraint();
  // virtual ~LimitConstraint();
  bool CanBeOmitted() const;

 public:
  unsigned long start_position_;
  unsigned long returned_tuples_;
};

//}   // namespace logical_query_plan
//}   // namespace claims

#endif  //  LOGICALQUERYPLAN_LIMIT_H_
