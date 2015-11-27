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
 * /Claims/loader/Validity.h
 *
 *  Created on: Nov 17, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef LOADER_VALIDITY_H_
#define LOADER_VALIDITY_H_
#include "../common/error_define.h"

namespace claims {
namespace loader {

struct Validity {
 public:
  Validity(int column_index, RetCode check_res)
      : column_index_(column_index), check_res_(check_res) {}
  virtual ~Validity();

 public:
  int column_index_;
  RetCode check_res_;
};

} /* namespace loader */
} /* namespace claims */

#endif  // LOADER_VALIDITY_H_
