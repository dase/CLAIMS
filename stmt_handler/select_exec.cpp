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

#include "../logical_operator/logical_query_plan_root.h"
#include "../physical_operator/physical_operator_base.h"
#include "../stmt_handler/stmt_handler.h"
using claims::logical_operator::LogicalQueryPlanRoot;
using claims::physical_operator::PhysicalOperatorBase;
using std::endl;
using std::vector;
using std::string;
using std::cout;

namespace claims {
namespace stmt_handler {
SelectExec::SelectExec(AstNode* stmt) : StmtExec(stmt) {
  select_ast_ = static_cast<AstSelectStmt*>(stmt_);
}

SelectExec::~SelectExec() {
  //  if (NULL != select_ast_) {
  //    delete select_ast_;
  //    select_ast_ = NULL;
  //  }
}

RetCode SelectExec::Execute(executed_result* exec_result) {
#ifdef PRINTCONTEXT
  select_ast_->Print();
  cout << "--------------begin semantic analysis---------------" << endl;
#endif
  SemanticContext sem_cnxt;
  RetCode ret = select_ast_->SemanticAnalisys(&sem_cnxt);
  if (eOK != ret) {
    exec_result->error_info = "semantic analysis error";
    exec_result->status = false;
    LOG(ERROR) << "semantic analysis error result= : " << ret;
    cout << "semantic analysis error result= : " << ret << endl;
    return ret;
  }
#ifdef PRINTCONTEXT
  select_ast_->Print();
  cout << "--------------begin push down condition ------------" << endl;
#endif
  select_ast_->PushDownCondition(NULL);
#ifdef PRINTCONTEXT
  select_ast_->Print();
  cout << "--------------begin logical plan -------------------" << endl;
#endif

  LogicalOperator* logic_plan = NULL;
  select_ast_->GetLogicalPlan(logic_plan);

  logic_plan = new LogicalQueryPlanRoot(0, logic_plan,
                                        LogicalQueryPlanRoot::kResultCollector);
  logic_plan->GetPlanContext();
#ifdef PRINTCONTEXT
  logic_plan->Print();
  cout << "--------------begin physical plan -------------------" << endl;
#endif

  PhysicalOperatorBase* physical_plan = logic_plan->GetPhysicalPlan(64 * 1024);
#ifdef PRINTCONTEXT
  physical_plan->Print();
  cout << "--------------begin output result -------------------" << endl;
#endif

  physical_plan->Open();
  while (physical_plan->Next(NULL)) {
  }
  exec_result->result = physical_plan->GetResultSet();
  physical_plan->Close();

  delete logic_plan;
  delete physical_plan;
  return rSuccess;
}
}  // namespace stmt_handler
}  // namespace claims
