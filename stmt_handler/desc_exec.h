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
 * /CLAIMS/stmt_handler/desc_stmt_exec.h
 *
 *  Created on: Feb 26, 2016
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#ifndef STMT_HANDLER_DESC_EXEC_H_
#define STMT_HANDLER_DESC_EXEC_H_

#include "../stmt_handler/stmt_exec.h"
#include "../sql_parser/ast_node/ast_desc_stmt.h"

namespace claims {
namespace stmt_handler {

class DescExec : public StmtExec {
 public:
  /**
   * @brief Method description: The executor about show statement.
   * @param AstNode* stmt   point to AST
   */
  DescExec(AstNode *stmt);  // NOLINT
  ~DescExec();
  /**
   * @brief the concrete operation of show statement.
   */
  RetCode Execute(ExecutedResult *exec_result);

 private:
  AstDescStmt *desc_stmt_ast_;
};

} /* namespace stmt_handler */
} /* namespace claims */

#endif  //  STMT_HANDLER_DESC_EXEC_H_
