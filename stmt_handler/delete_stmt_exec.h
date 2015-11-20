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
 * /CLAIMS/stmt_handler/delete_stmt_exec.h
 *
 *  Created on: Nov 19, 2015
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#ifndef STMT_HANDLER_DELETE_STMT_EXEC_H_
#define STMT_HANDLER_DELETE_STMT_EXEC_H_

#include <string>

#include "../common/Block/ResultSet.h"
#include "../stmt_handler/stmt_exec.h"
#include "../sql_parser/ast_node/ast_delete_stmt.h"
using std::string;

namespace claims {
namespace stmt_handler {

class DeleteStmtExec : public StmtExec {
 public:
  /**
   * @brief Method description: The executor about delete statement.
   * @param AstNode* stmt   point to AST
   */
  DeleteStmtExec(AstNode *stmt);  // NOLINT
  virtual ~DeleteStmtExec();

  /**
   * @brief the concrete operation of delete statement.
   */
  RetCode Execute(executed_result *exec_result);
  void InsertDeletedDataIntoTableDEL(string del_table_name,
                                     ResultSet *result_set);

 private:
  AstDeleteStmt *delete_stmt_ast_;
};

} /* namespace stmt_handler */
} /* namespace claims */

#endif  //  STMT_HANDLER_DELETE_STMT_EXEC_H_
