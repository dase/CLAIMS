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
#include "../Environment.h"

#include "../Catalog/stat/Analyzer.h"

#include "../Catalog/ProjectionBinding.h"
using std::endl;
using std::string;
using std::vector;

namespace claims {
namespace stmt_handler {

ShowExec::ShowExec(AstNode* stmt) : StmtExec(stmt) {
  assert(stmt_);
  show_stmt_ast_ = dynamic_cast<AstShowStmt*>(stmt_);
}

ShowExec::~ShowExec() {}

RetCode ShowExec::Execute(executed_result* exec_result) {
  int ret = rSuccess;
  ostringstream ostr;
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  int table_count = local_catalog->getTableCount();
  switch (show_stmt_ast_->show_type_) {
    case 1: {
      ostr << "TABLES:   Num = " << table_count << endl;
      for (unsigned i = 0; i < table_count; ++i) {
        ostr << "    " << local_catalog->getTable(i)->getTableName() << endl;
      }
      exec_result->info = ostr.str();
      exec_result->status = true;
      exec_result->result = NULL;
      break;
    }
    default: {
      exec_result->error_info = "Ooooops, not Supported now!";
      LOG(ERROR) << "Not Supported now.";
      exec_result->status = false;
      exec_result->result = NULL;
    }
  }
}
}  // namespace stmt_handler
}  // namespace claims
