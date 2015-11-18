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
 * /CLAIMS/physical_operator/test/fake_table_scan.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: minqi
 *		   Email: mqzhou@sei.ecnu.edu.cn
 *
 * Description:
 *
 */

#include "fake_table_scan.h"

namespace claims {
namespace physical_query_plan {
namespace test {

FakeTableScan::~FakeTableScan() {
  // TODO Auto-generated destructor stub
}

FakeTableScan::FakeTableScan() {
  // TODO Auto-generated constructor stub
}

bool FakeTableScan::Open(const PartitionOffset& part_off) {}

bool FakeTableScan::Next() {}

bool FakeTableScan::Close() {}

bool FakeTableScan::Print() {}

} /* namespace test */
} /* namespace physical_query_plan */
} /* namespace claims */
