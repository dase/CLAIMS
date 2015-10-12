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
 * /CLAIMS/stmt_handler/stmt_exec.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 * Description:
 *
 */

#include "../stmt_handler/stmt_exec.h"

#include <iostream>
namespace claims {
namespace stmt_handler {
StmtExec::StmtExec(AstNode* stmt) : stmt_(stmt) {
  // TODO Auto-generated constructor stub
  tablename_ = "";
  table_desc_ = NULL;
  result_set_ = NULL;
  result_flag_ = true;
  error_msg_ = "";
  info_ = "";
}

StmtExec::~StmtExec() {
  // TODO Auto-generated destructor stub
  if (result_set_) {
    delete result_set_;
    result_set_ = NULL;
  }
}

int StmtExec::Execute() {
  std::cout << "this is stmt_exec!" << std::endl;
  return 0;
}

bool StmtExec::isTableExist() {
  bool isExist = false;
  if (table_desc_ != NULL) {
    isExist = true;
  }
  return isExist;
}

const string& StmtExec::getErrorMsg() const { return error_msg_; }

const string& StmtExec::getInfo() const { return info_; }

bool StmtExec::isResultFlag() const { return result_flag_; }

const ResultSet* StmtExec::getResultSet() const { return result_set_; }

}  // namespace stmt_handler
}  // namespace claims
