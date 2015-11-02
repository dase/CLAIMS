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
 * physical_operator/physical_operator_base.h
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#ifndef PHYSICAL_OPERATOR_PHYSICAL_OPERATOR_BASE_H_
#define PHYSICAL_OPERATOR_PHYSICAL_OPERATOR_BASE_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>
#include "../common/Block/ResultSet.h"
#include "../common/Block/BlockStream.h"
#include "../common/ids.h"
namespace claims {
namespace physical_operator {
/**
 * This is the base class for the block stream iterators.
 */

class PhysicalOperatorBase {
 public:
  PhysicalOperatorBase();
  virtual ~PhysicalOperatorBase();

  static PhysicalOperatorBase* createIterator(const string& IteratorName);

  virtual bool Open(const PartitionOffset& part_off = 0) = 0;
  virtual bool Next(BlockStreamBase*) = 0;
  virtual bool Close() = 0;
  virtual void Print() { printf("??\n"); };

  /*
   * Suggested by scdong, this method can be removed.
   */
  virtual ResultSet* GetResultSet();

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    /*This is the base class, nothing needs to be serialized.*/
  }
};
}  // namespace physical_operator
}  // namespace claims

#endif  //  PHYSICAL_OPERATOR_PHYSICAL_OPERATOR_BASE_H_
