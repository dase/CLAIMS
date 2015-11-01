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
 * /CLAIMS/physical_operator/physical_limit.h
 *
 *  Created on: Sep 18, 2015
 *      Author: wangli,Hanzhang
 *		   Email:wangli1426@gmail.com
 *
 * Description: Implementation of Limit operator in physical layer.
 *
 */

#ifndef PHYSICAL_OPERATOR_PHYSICAL_LIMIT_H_
#define PHYSICAL_OPERATOR_PHYSICAL_LIMIT_H_

#include "../physical_operator/physical_operator_base.h"

namespace claims {
namespace physical_operator {
/**
 * @brief Method description: Implementation of limit physical operator. This
 * operator is a traditional model of iterator. Execute inline function to judge
 * which position is starting point and whether tuples is acquired.
 */
class PhysicalLimit : public PhysicalOperatorBase {
 public:
  struct State {
    friend class PhysicalLimit;

   public:
    State(Schema* schema, PhysicalOperatorBase* child,
          unsigned long limit_tuples, unsigned block_size,
          unsigned long start_position = 0);
    State();

   private:
    Schema* schema_;
    PhysicalOperatorBase* child_;
    unsigned long limit_tuples_;
    unsigned block_size_;
    unsigned long start_position_;

   private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& schema_& child_& limit_tuples_& block_size_& start_position_;
    }
  };
  PhysicalLimit();
  PhysicalLimit(State state);
  virtual ~PhysicalLimit();

  /**
   * @brief Method description: Initialize the position of current tuple and
   * target tuple
   */
  bool Open(const PartitionOffset& kPartitionOffset);

  /**
   * @brief Method description:find limit_tuple tuples from start_position and
   * return them
   * @return : given tuples.
   */
  bool Next(BlockStreamBase* block);

  /**
   * @brief Method description: revoke resource
   */
  bool Close();
  void Print();

 private:
  /**
   * @brief Method description: this function judges whether tuples we need are
   * acquired.
   */
  inline bool LimitExhausted() const {
    return received_tuples_ >= state_.limit_tuples_;
  }
  /**
   * @brief Method description:Finding start_postition_ of limit is the aim of
   * this function.
   * @return start position.
   */

  inline bool ShouldSkip() const { return tuple_cur_ < state_.start_position_; }

 private:
  State state_;
  unsigned received_tuples_;
  BlockStreamBase* block_for_asking_;

  /* the index of current tuple*/
  unsigned long tuple_cur_;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperatorBase>(*this) & state_;
  }
};
}  // namespace physical_operator
}  // namespace claims

#endif  //  PHYSICAL_OPERATOR_PHYSICAL_LIMIT_H_
