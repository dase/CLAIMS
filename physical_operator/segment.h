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
 * /Claims/stmt_handler/segment.h
 *
 *  Created on: Mar 13, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef PHYSICAL_OPERATOR_SEGMENT_H_
#define PHYSICAL_OPERATOR_SEGMENT_H_
#include <vector>
#include "../common/error_define.h"
#include "../common/ids.h"
using std::vector;
namespace claims {

namespace physical_operator {
class PhysicalOperatorBase;
class Segment {
 public:
  Segment();
  Segment(PhysicalOperatorBase* plan_segment_,
          vector<NodeID> lower_node_id_list, vector<NodeID> upper_node_id_list,
          u_int64_t exchange_id);
  virtual ~Segment();
  RetCode Send_Plan_Segment();
  PhysicalOperatorBase* get_plan_segment() { return plan_segment_; }

  vector<NodeID> lower_node_id_list_;
  vector<NodeID> upper_node_id_list_;
  u_int64_t exchange_id_ = 0;

 private:
  PhysicalOperatorBase* plan_segment_;
};
}
}  // namespace claims

#endif  //  PHYSICAL_OPERATOR_SEGMENT_H_
