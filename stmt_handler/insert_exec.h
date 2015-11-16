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
 * /CLAIMS/stmt_handler/insert_exec.h
 *
 *  Created on: Sep 23, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 * Description:
 *    this file contains one class about the data definition language of
 *"insert",
 *    when we insert data to tables, the code in this file will take effect.
 */

#ifndef STMT_HANDLER_INSERT_EXEC_H_
#define STMT_HANDLER_INSERT_EXEC_H_

#include "../stmt_handler/stmt_exec.h"

namespace claims {
namespace stmt_handler {

/**
 * @brief insert data to the exist tables.
 * @details
 */
class InsertExec : public StmtExec {
 public:
  /**
   * @brief Method description: the executor about insert data to tables.
   * @param AstNode *stmt point to abstract syntax tree.
   */
  InsertExec(AstNode *stmt);
  virtual ~InsertExec();
  /**
   * @brief the concrete operation of insert data to tables.
   */
  RetCode Execute(executed_result *exec_result);

 private:
  /**
   * @brief insert value to stream
   */
  bool InsertValueToStream(AstInsertVals *insert_value, TableDescriptor *table,
                           unsigned position, std::ostringstream &ostr);
  /**
   * @brief check value type
   */
  bool CheckType(const column_type *col_type, AstNode *expr);

 private:
  /**
   * this pointer describes the abstract syntax tree about insert data to
   * tables.
   * It is converted from the member stmt_ of base class when we construct a new
   * object.
   */
  AstInsertStmt *insert_ast_;

  // warning flag during insert data into tables, it will be true will warning
  // occurs during insert data.
  bool has_warning_;
  // mark any errors occurred during insert values.
  bool is_correct_;
  // flag to insert all columns or part of columns, all columns will be set as
  // true else false.
  bool is_all_col_;
  // constant value for function CheckType()
  static const int INT_LENGTH;
  static const int FLOAT_LENGTH;
  static const int SMALLINT_LENGTH;
};
}  // namespace stmt_handler
}  // namespace claims

#endif  //  STMT_HANDLER_INSERT_EXEC_H_
