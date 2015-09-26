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
 * /CLAIMS/stmt_handler/stmt_handler.h
 *
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 * 
 * Description:
 *
 */

#ifndef STMT_HANDLER_STMT_HANDLER_H_
#define STMT_HANDLER_STMT_HANDLER_H_

#include "../stmt_handler/stmt_exec.h"
#include "../stmt_handler/select_exec.h"
#include "../stmt_handler/load_exec.h"
#include "../stmt_handler/insert_exec.h"
#include "../stmt_handler/create_exec.h"

namespace claims {
namespace stmt_handler {

class StmtHandler {
 public:
  StmtHandler(AstNode* stmt_ast);
  virtual ~StmtHandler();
  int Execute();
 private:
  StmtExec* stmt_executor_;
};

}   // namespace stmt_handler
} // namespace claims
#endif //  STMT_HANDLER_STMT_HANDLER_H_ 
