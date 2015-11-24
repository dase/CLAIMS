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
 * /CLAIMS/stmt_handler/drop_table_exec.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>  // NOLINT
#include "../stmt_handler/drop_table_exec.h"
#include "../Environment.h"
#include "../Loader/Hdfsloader.h"

using std::vector;
namespace claims {
namespace stmt_handler {

DropTableExec::DropTableExec(AstNode* stmt) : StmtExec(stmt) {
  assert(stmt_);
  drop_table_ast_ = dynamic_cast<AstDropTable*>(stmt_);
}

DropTableExec::~DropTableExec() {}

RetCode DropTableExec::Execute(executed_result* exec_result) {
  RetCode ret = rSuccess;

  SemanticContext sem_cnxt;
  ret = drop_table_ast_->SemanticAnalisys(&sem_cnxt);
  if (rSuccess != ret) {
    exec_result->error_info =
        "Semantic analysis error.\n" + sem_cnxt.error_msg_;
    exec_result->status = false;
    LOG(ERROR) << "semantic analysis error result= : " << ret;
    cout << "semantic analysis error result= : " << ret << endl;
    return ret;
  }

  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  AstDropTableList* table_list =
      dynamic_cast<AstDropTableList*>(drop_table_ast_->table_list_);
  while (NULL != table_list) {
    string tablename;
    if ("" != table_list->table_name_) {
      tablename = table_list->table_name_;
#ifdef sem_cnxt
    } else {
      exec_result->error_info =
          "No table name or invalid name during dropping table!";
      exec_result->status = false;
      exec_result->result = NULL;
      return rParserError;
      break;
    }
#endif
    TableDescriptor* table_desc = local_catalog->getTable(tablename);
#ifdef sem_cnxt
    if (NULL == table_desc) {
      exec_result->error_info = "table [" + tablename + "] is not exist!";
      exec_result->status = false;
      exec_result->result = NULL;
      return rParserError;
    } else {
#endif
      if (local_catalog->drop_table(tablename, table_desc->get_table_id())) {
        HdfsLoader* Hl = new HdfsLoader(table_desc, (open_flag)(DELETE_FILE));
        Hl->DeleteDataFilesForDropTable();

        delete table_desc;
        cout << tablename + " is dropped from this database!" << endl;
        exec_result->info = "drop table successfully!";

#if 1
        // drop table_DEL couple with table.
        {
          TableDescriptor* table_desc_DEL =
              Environment::getInstance()->getCatalog()->getTable(tablename +
                                                                 "_DEL");
          Environment::getInstance()->getCatalog()->drop_table(
              tablename + "_DEL", table_desc_DEL->get_table_id());
          HdfsLoader* Hl =
              new HdfsLoader(table_desc_DEL, (open_flag)(DELETE_FILE));
          Hl->DeleteDataFilesForDropTable();

          delete table_desc_DEL;
          cout << tablename + "_DEL" + " is dropped from this database!"
               << endl;
        }
#endif
      } else {
        cout << "drop table [" + tablename + "] failed" << endl;
        exec_result->error_info = "drop table [" + tablename + "] failed.";
        exec_result->status = false;
        exec_result->result = NULL;
      }
    }

    table_list = dynamic_cast<AstDropTableList*>(table_list->next_);
  }
  local_catalog->saveCatalog();
  exec_result->status = true;
  exec_result->result = NULL;

  return ret;
}
} /* namespace stmt_handler */
} /* namespace claims */
