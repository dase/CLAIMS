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
 * /CLAIMS/logical_operator/logical_limit.h
 *
 *  Created on: Sep 18, 2015
 *      Author: wangli，HanZhang
 *       Email: wangli1426@gmail.com
 *
 * Description: Implementation of Limit operator in logical layer
 */

#ifndef LOGICAL_OPERATOR_LOGICAL_LIMIT_H_
#define LOGICAL_OPERATOR_LOGICAL_LIMIT_H_

#include "../logical_operator/logical_operator.h"
namespace claims {
namespace logical_operator {

/**
 * @brief Method description:limit operator is a simple operator.
 * start_position_:the fault value is 0,it means the position where we should
 * acquire data.
 * return_tuples_:the fault value is -1, it means that all rows are needed.
 */

class LimitConstraint {
 public:
  LimitConstraint(unsigned long return_tuples);
  LimitConstraint(unsigned long return_tuples, unsigned long position);
  // TODO(HanZhang):The positions of parameters is reverse, we should make
  // sure where the function called.
  LimitConstraint();
  // virtual ~LimitConstraint();
  bool CanBeOmitted() const;

 public:
  unsigned long start_position_;
  unsigned long returned_tuples_;
};

}  // namespace logical_operator
}  // namespace claims

#endif  //  LOGICAL_OPERATOR_LOGICAL_LIMIT_H_
