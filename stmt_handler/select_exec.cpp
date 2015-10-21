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
 * /CLAIMS/stmt_handler/select_exec.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#include "../stmt_handler/select_exec.h"
#include <glog/logging.h>
#include <iostream>
#include <vector>
#include <string>

using std::endl;
using std::vector;
using std::string;
using std::cout;

namespace claims {
namespace stmt_handler {
SelectExec::SelectExec(AstNode* stmt) : StmtExec(stmt) {
  // TODO Auto-generated constructor stub
  select_ast_ = static_cast<AstSelectStmt*>(stmt_);
}

SelectExec::~SelectExec() {
  // TODO Auto-generated destructor stub
}

int SelectExec::Execute(AstNode* ast_root) {}
}  // namespace stmt_handler
}  // namespace claims
