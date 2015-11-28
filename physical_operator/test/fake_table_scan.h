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
 * /CLAIMS/physical_operator/test/fake_table_scan.h
 *
 *  Created on: Nov 17, 2015
 *      Author: minqi
 *		   Email: mqzhou@sei.ecnu.edu.cn
 *
 * Description:
 *
 */

#ifndef PHYSICAL_OPERATOR_TEST_FAKE_TABLE_SCAN_H_
#define PHYSICAL_OPERATOR_TEST_FAKE_TABLE_SCAN_H_
#include "../../Catalog/Partitioner.h"
#include "../../common/ids.h"
#include "../../common/Schema/Schema.h"

namespace claims {
namespace physical_query_plan {
namespace test {

/*
 *
 */
class FakeTableScan : public physical_operator::PhysicalOperator {
 public:
  virtual ~FakeTableScan();
  FakeTableScan();

  class State {
    friend class FakeTableScan;

   public:
    State(ProjectionID id, Schema* schema, unsigned block_size);
    State() {}

   public:
    ProjectionID id_;
    Schema* Schema_;
    unsigned block_size_;
  };

  bool Open(const PartitionOffset& part_off);
  bool Next();
  bool Close();
  bool Print();

 public:
  State state_;

  static unsigned number_of_columns_ = 16;
};

} /* namespace test */
} /* namespace physical_query_plan */
} /* namespace claims */

#endif  //  PHYSICAL_OPERATOR_TEST_FAKE_TABLE_SCAN_H_
