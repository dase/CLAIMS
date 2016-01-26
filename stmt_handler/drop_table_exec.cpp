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

#include "../catalog/catalog.h"
#include "../common/file_handle/file_handle_imp_factory.h"
#include "../common/error_define.h"
#include "../Config.h"
#include "../Environment.h"
#include "../loader/data_injector.h"
#include "../loader/table_file_connector.h"
#include "../stmt_handler/drop_table_exec.h"
using claims::common::FilePlatform;
using claims::loader::TableFileConnector;
using claims::catalog::Catalog;
using std::vector;
using claims::common::rNULLDropTableName;
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
    LOG(WARNING) << "semantic analysis error result= : " << ret;
    cout << "semantic analysis error result= : " << ret << endl;
    return ret;
  }
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  AstDropTableList* table_list =
      dynamic_cast<AstDropTableList*>(drop_table_ast_->table_list_);
  while (NULL != table_list) {
    string table_name;
    if ("" != table_list->table_name_) {  // if the table_name is null, then
                                          // go to the next element of the
                                          // table_list_
      table_name = table_list->table_name_;
      if (CheckBaseTbl(table_name)) {
        DropTable(table_name);  // drop the base table
        cout << table_name + " is dropped from this database!" << endl;
        DropTable(table_name + "_DEL");  // drop the
        cout << table_name + "_DeL is dropped from this database!" << endl;
        exec_result->info_ = "drop table successfully!";

      } else {
        DropTable(table_name);
        cout << table_name + "is dropped from this database!" << endl;
        exec_result->info_ = "drop table successfully!";
      }
    } else {
      WLOG(rNULLDropTableName,
           "the table name in the drop table statement is NULL");
      exec_result->error_info_ = "drop table [" + table_name + "] failed.";
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

bool DropTableExec::CheckBaseTbl(const string& table_name) const {
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  string base_tbl_name = "";
  // if the length of the table name is less than 4, it should be the name of
  // the base table.
  if ("" != table_name && table_name.size() < 4) {
    return true;
  }
  if ("_DEL" == table_name.substr(table_name.size() - 4, 4)) {
    base_tbl_name = table_name.substr(0, table_name.size() - 4);
    if (local_catalog->getTable(base_tbl_name)) {
      return false;
    } else {
      return true;
    }
  } else {
    if (local_catalog->getTable(table_name + "_DEL")) {
      return true;
    } else {
      return false;
    }
  }
}

/**
 * drop the table based on the provided table name,
 * this operator will delete the table in the catalog as well as the table file
 *on the disk or hdfs
 *
 * @param table_name
 * @return
 */
RetCode DropTableExec::DropTable(const string& table_name) {
  RetCode ret = rSuccess;
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  if ("" == table_name) {
    ret = rNULLDropTableName;
    WLOG(ret, "the name of the table to be dropped is NULL");
    return ret;
  }
  TableDescriptor* table_desc = local_catalog->getTable(table_name);
  if (local_catalog->DropTable(table_name, table_desc->get_table_id())) {
    vector<vector<string>> write_path;
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
      write_path.push_back(prj_write_path);
      TableFileConnector* connector = new TableFileConnector(
          Config::local_disk_mode ? FilePlatform::kDisk : FilePlatform::kHdfs,
          write_path);
      connector->DeleteFiles();
    }
    delete table_desc;
    // if only the deleted table is dropped, for example when doing the table
    // compaction, or the delete table is delete by the system administrator
    if (!CheckBaseTbl(table_name)) {
      table_desc =
          local_catalog->getTable(table_name.substr(table_name.size() - 4, 4));
      if (NULL != table_desc) {
        table_desc->SetDeletedTuples(false);
      }
    }
    return ret;
  }
}
} /* namespace stmt_handler */
} /* namespace claims */
