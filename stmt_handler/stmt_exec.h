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
 * /CLAIMS/stmt_handler/stmt_exec.h
 *
 *  Created on: Sep 23, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#ifndef STMT_HANDLER_STMT_EXEC_H_
#define STMT_HANDLER_STMT_EXEC_H_

#include <glog/logging.h>
#include "../common/Block/ResultSet.h"
#include "../Catalog/table.h"
#include "../common/data_type.h"
#include "../common/error_define.h"

#include "../sql_parser/ast_node/ast_node.h"
#include "../sql_parser/ast_node/ast_insert_stmt.h"
#include "../sql_parser/ast_node/ast_select_stmt.h"
#include "../sql_parser/ast_node/ast_load_stmt.h"
#include "../sql_parser/ast_node/ast_expr_node.h"
#include "../sql_parser/ast_node/ast_drop_stmt.h"
#include "../sql_parser/ast_node/ast_create_stmt.h"

namespace claims {
namespace stmt_handler {
/**
 * @brief
 * @details
 */
class StmtExec {
 public:
  StmtExec(AstNode* stmt);
  virtual ~StmtExec();
  virtual int Execute();

 public:
  const ResultSet* getResultSet() const;
  bool isResultFlag() const;
  const string& getErrorMsg() const;
  const string& getInfo() const;

 protected:
  bool isTableExist();

 protected:
  AstNode* stmt_;
  string tablename_;
  TableDescriptor* table_desc_;

  ResultSet* result_set_;
  bool result_flag_;
  string error_msg_;
  string info_;
};

}  // namespace stmt_handler
}  // namespace claims
#endif  //  STMT_HANDLER_STMT_EXEC_H_
