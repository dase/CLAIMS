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
 * physical_operator/physical_operator_base.cpp
 *  Created on: Aug 26, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */
#include "../physical_operator/physical_operator_base.h"

#include <assert.h>
#include <iostream>
namespace claims {
namespace physical_operator {
PhysicalOperatorBase::PhysicalOperatorBase() {
  // TODO Auto-generated constructor stub
}

PhysicalOperatorBase::~PhysicalOperatorBase() {
  // TODO Auto-generated destructor stub
}

PhysicalOperatorBase* PhysicalOperatorBase::createIterator(
    const string& IteratorName) {
  //	if(IteratorName=="scan"){
  //		cout<<"scan iterator"<<endl;
  //		return new ExpandableBlockStreamSingleColumnScan();
  //	}
}

ResultSet* PhysicalOperatorBase::GetResultSet() {
  printf("You cannot get ResultSet from Root operator!\n");
  assert(false);
  return 0;
}
}  // namespace physical_operator
}  // namespace claims
