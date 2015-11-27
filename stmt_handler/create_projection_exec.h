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
 * /CLAIMS/stmt_handler/create_projection_exec.h
 *
 *  Created on: Nov 17, 2015
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#ifndef STMT_HANDLER_CREATE_PROJECTION_EXEC_H_
#define STMT_HANDLER_CREATE_PROJECTION_EXEC_H_

#include "../stmt_handler/stmt_exec.h"
#include "../sql_parser/ast_node/ast_create_stmt.h"
namespace claims {
namespace stmt_handler {
/**
 * @brief
 */
class CreateProjectionExec : public StmtExec {
 public:
  /**
   * @brief The executor about create projection statement.
   * @param AstNode* stmt
   */
  CreateProjectionExec(AstNode* stmt);  // NOLINT
  virtual ~CreateProjectionExec();

  /**
   * @brief the concrete operation of create projection statement.
   * @param ExecutedResult *exec_result
   */
  RetCode Execute(ExecutedResult* exec_result);

 private:
  AstCreateProjection* create_projection_ast_;
};

} /* namespace stmt_handler */
} /* namespace claims */

#endif  //  STMT_HANDLER_CREATE_PROJECTION_EXEC_H_
