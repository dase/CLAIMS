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
 * /Claims/stmt_handler/segment.cpp
 *
 *  Created on: Mar 13, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#include "../physical_operator/segment.h"

#include <sys/types.h>
#include <vector>
#include "../common/ids.h"

namespace claims {
namespace physical_operator {

Segment::Segment() {
  // TODO Auto-generated constructor stub
}

Segment::~Segment() {
  // TODO Auto-generated destructor stub
}
Segment::Segment(PhysicalOperatorBase* plan_segment,
                 vector<NodeID> lower_node_id_list,
                 vector<NodeID> upper_node_id_list, u_int64_t exchange_id)
    : plan_segment_(plan_segment),
      lower_node_id_list_(lower_node_id_list),
      upper_node_id_list_(upper_node_id_list),
      exchange_id_(exchange_id) {}
RetCode Segment::Send_Plan_Segment() { return 0; }
}
}  // namespace claims
