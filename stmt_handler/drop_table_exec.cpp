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

#include "../stmt_handler/drop_table_exec.h"
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>  // NOLINT

#include "../catalog/catalog.h"
#include "../common/file_handle/file_handle_imp_factory.h"
#include "../Config.h"
#include "../Environment.h"
#include "../loader/data_injector.h"
#include "../loader/table_file_connector.h"

using claims::common::FilePlatform;
using claims::loader::TableFileConnector;
using claims::catalog::Catalog;
using std::vector;
namespace claims {
namespace stmt_handler {

DropTableExec::DropTableExec(AstNode* stmt) : StmtExec(stmt) {
  assert(stmt_);
  drop_table_ast_ = dynamic_cast<AstDropTable*>(stmt_);
}

DropTableExec::~DropTableExec() {}

RetCode DropTableExec::Execute(ExecutedResult* exec_result) {
  RetCode ret = rSuccess;

  SemanticContext sem_cnxt;
  ret = drop_table_ast_->SemanticAnalisys(&sem_cnxt);
  if (rSuccess != ret) {
    exec_result->error_info_ =
        "Semantic analysis error.\n" + sem_cnxt.error_msg_;
    exec_result->status_ = false;
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
    }
    TableDescriptor* table_desc = local_catalog->getTable(tablename);
    if (local_catalog->DropTable(tablename, table_desc->get_table_id())) {
      vector<vector<string>> write_path_;
      for (int i = 0; i < table_desc->getNumberOfProjection(); i++) {
        vector<string> prj_write_path;
        prj_write_path.clear();
        for (int j = 0; j < table_desc->getProjectoin(i)
                                ->getPartitioner()
                                ->getNumberOfPartitions();
             ++j) {
          string path =
              PartitionID(table_desc->getProjectoin(i)->getProjectionID(), j)
                  .getPathAndName();
          prj_write_path.push_back(path);
        }
        write_path_.push_back(prj_write_path);

        TableFileConnector* connector = new TableFileConnector(
            Config::local_disk_mode ? FilePlatform::kDisk : FilePlatform::kHdfs,
            write_path_);

        connector->DeleteFiles();
      }

      delete table_desc;
      cout << tablename + " is dropped from this database!" << endl;
      exec_result->info_ = "drop table successfully!";

      // drop table_DEL couple with table.
      TableDescriptor* table_desc_DEL =
          Environment::getInstance()->getCatalog()->getTable(tablename +
                                                             "_DEL");
      Environment::getInstance()->getCatalog()->DropTable(
          tablename + "_DEL", table_desc_DEL->get_table_id());

      //          HdfsLoader* Hl =
      //              new HdfsLoader(table_desc_DEL,
      //              (open_flag)(DELETE_FILE));
      //          Hl->DeleteDataFilesForDropTable();

      delete table_desc_DEL;
      cout << tablename + "_DEL" + " is dropped from this database!" << endl;

    } else {
      cout << "drop table [" + tablename + "] failed" << endl;
      exec_result->error_info_ = "drop table [" + tablename + "] failed.";
      exec_result->status_ = false;
      exec_result->result_ = NULL;
    }
    table_list = dynamic_cast<AstDropTableList*>(table_list->next_);
  }
  local_catalog->saveCatalog();
  exec_result->status_ = true;
  exec_result->result_ = NULL;

  return ret;
}
} /* namespace stmt_handler */
} /* namespace claims */
