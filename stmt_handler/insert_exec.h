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
 * /Claims/stmt_handle/insert_exec.h
 *
 *  Created on: Sep 23, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef STMT_HANDLE_INSERT_EXEC_H_
#define STMT_HANDLE_INSERT_EXEC_H_

#include "stmt_exec.h"

namespace claims {
namespace stmt_handle {
class InsertExec : public StmtExec {
 public:
  InsertExec(AstNode* stmt);
  virtual ~InsertExec();
  int Execute();
 private:
  AstInsertStmt* insert_ast_;
};
}   // namespace stmt_handle
} // namespace claims

#endif  //  STMT_HANDLE_INSERT_EXEC_H_
