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
 * /CLAIMS/physical_operator/test/delete_filter_test.h
 *
 *  Created on: Nov 5, 2015
 *      Author: minqi
 *		   Email: mqzhou@sei.ecnu.edu.cn
 *
 * Description:
 *
 */

#ifndef PHYSICAL_OPERATOR_TEST_DELETE_FILTER_TEST_H_
#define PHYSICAL_OPERATOR_TEST_DELETE_FILTER_TEST_H_

namespace claims {
namespace physical_query_plan {

/*
 *
 */
class DeleteFilterTest : public testing::Test {
 public:
  DeleteFilterTest();
  virtual ~DeleteFilterTest();
};

} /* namespace physical_query_plan */
} /* namespace claims */

#endif  //  PHYSICAL_OPERATOR_TEST_DELETE_FILTER_TEST_H_
