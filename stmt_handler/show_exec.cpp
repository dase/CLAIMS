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
 * /CLAIMS/stmt_handler/show_exec.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#include <string>
#include <vector>
#include <iostream>
#include "../stmt_handler/show_exec.h"
#include "../catalog/catalog.h"
#include "../Environment.h"
#include "../catalog/stat/Analyzer.h"
#include "../catalog/projection_binding.h"
using std::endl;
using std::string;
using std::vector;
using claims::catalog::Catalog;
namespace claims {
namespace stmt_handler {

ShowExec::ShowExec(AstNode* stmt) : StmtExec(stmt) {
  assert(stmt_);
  show_stmt_ast_ = dynamic_cast<AstShowStmt*>(stmt_);
}

ShowExec::~ShowExec() {}

RetCode ShowExec::Execute(ExecutedResult* exec_result) {
  int ret = rSuccess;
  ostringstream ostr;
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  // int table_count = local_catalog->getTableCount();
  switch (show_stmt_ast_->show_type_) {
    case 1: {
      ostr << "TABLES: " << endl;  //   Num = " << table_count << endl;
      local_catalog->GetAllTables(ostr);
      // for (unsigned i = 0; i < table_count; ++i) {
      //   ostr << "    " << local_catalog->getTable(i)->getTableName() << endl;
      // }
      exec_result->info_ = ostr.str();
      exec_result->status_ = true;
      exec_result->result_ = NULL;
      break;
    }
    default: {
      exec_result->error_info_ = "Ooooops, not Supported now!";
      LOG(ERROR) << "Not Supported now.";
      exec_result->status_ = false;
      exec_result->result_ = NULL;
    }
  }
}
}  // namespace stmt_handler
}  // namespace claims
