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
 * /CLAIMS/stmt_handler/stmt_handler.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#include <iostream>
#include <glog/logging.h>
#include "../stmt_handler/stmt_handler.h"

namespace claims {
namespace stmt_handler {

StmtHandler::StmtHandler(AstNode* stmt_ast) {
  // TODO Auto-generated constructor stub
  switch (stmt_ast->ast_node_type_) {
    case AST_SELECT_STMT: {
      stmt_executor_ = new SelectExec(stmt_ast);
      break;
    }
    case AST_INSERT_STMT: {
      stmt_executor_ = new InsertExec(stmt_ast);
      break;
    }
    default : {
      LOG(WARNING) << "can't find corresponding executor!" << std::endl;
      stmt_executor_ = new StmtExec(stmt_ast);
    }
  }
}

StmtHandler::~StmtHandler() {
  // TODO Auto-generated destructor stub

}

int StmtHandler::Execute() {
  return stmt_executor_->Execute();
}

}   // namespace stmt_handler
} // namespace claims
