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
 * /CLAIMS/stmt_handler/select_exec.h
 *
 *  Created on: Sep 23, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef STMT_HANDLER_SELECT_EXEC_H_
#define STMT_HANDLER_SELECT_EXEC_H_
#include <stack>
#include <string>
#include <vector>

#include "../physical_operator/physical_operator_base.h"
#include "../physical_operator/segment.h"
#include "../stmt_handler/delete_stmt_exec.h"
#include "../stmt_handler/stmt_exec.h"
#include "../stmt_handler/stmt_handler.h"
using std::stack;
using claims::physical_operator::PhysicalOperatorBase;
using claims::physical_operator::Segment;
using std::string;
namespace claims {
namespace stmt_handler {
class SelectExec : public StmtExec {
  friend class DeleteStmtExec;

 public:
  SelectExec(AstNode* stmt, string raw_sql);  // NOLINT
  SelectExec(AstNode* stmt);
  virtual ~SelectExec();
  RetCode Execute(ExecutedResult* exec_result);

  RetCode Execute();

 private:
  static void* SendAllSegments(void* arg);
  RetCode IsUpperExchangeRegistered(vector<NodeID>& upper_node_id_list,
                                    const u_int64_t exchange_id);

 private:
  AstSelectStmt* select_ast_;
  string raw_sql_;
  stack<Segment*> all_segments_;
};
}  // namespace stmt_handler
}  // namespace claims
#endif  //  STMT_HANDLER_SELECT_EXEC_H_
