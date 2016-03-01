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
      // to make consistency, drop the del table first, and then the base table
      table_name = table_list->table_name_;
      if (CheckBaseTbl(table_name)) {  // drop the base table
        ret = DropTable(table_name + "_DEL");
        if (ret == rSuccess) {
          cout << table_name + "_DEL is dropped from this database!" << endl;
          ret = DropTable(table_name);
          cout << table_name + "is dropped from this database!" << endl;
        } else {
          DropTable(table_name + "_DEL");
          DropTable(table_name);
        }
      } else {  // drop the del table only, but the information in the catalog
                // does not need to be removed
        ret = DeleteTableFiles(table_name);
        // todo (miqni 2016.1.28) to delete the del table from memory
        cout << table_name + "_DeL is dropped from this database!" << endl;
      }

      if (ret == rSuccess) {
        exec_result->info_ = "drop table successfully!";
      } else {
        exec_result->error_info_ = "drop table [" + table_name + "] failed.";
        exec_result->status_ = false;
        exec_result->result_ = NULL;
        return ret;
      }
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
    if (NULL != local_catalog->getTable(base_tbl_name)) {
      return false;
    } else {
      return true;
    }
  } else {
    if (NULL != local_catalog->getTable(table_name + "_DEL")) {
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
  // start to drop table
  // you need to delete the file first, and then drop the informantion in the
  // catalog
  ret = DeleteTableFiles(table_name);
  if (rSuccess != ret) {
    ELOG(ret, "failed to delete the files when dropping table" + table_name);
    return ret;
  } else {
    ret = DropTableFromCatalog(table_name);
    if (ret != rSuccess) {
      ELOG(ret,
           "failed to drop the table from the catalog, while its files have "
           "been deleted, when dropping table" +
               table_name);
      return ret;
    }
  }
}

/**
 * drop the table information from the catalog as well as to delete the object
 * of the TableDescroptor of the table_anme
 * @param table_name
 * @return
 */
RetCode DropTableExec::DropTableFromCatalog(const string& table_name) {
  RetCode ret = rSuccess;
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  TableDescriptor* table_desc = local_catalog->getTable(table_name);
  local_catalog->DropTable(table_name, table_desc->get_table_id());
  delete table_desc;
}

/**
 * delete the file associated with the Table table_name only
 * @param table_name
 * @return
 */
RetCode DropTableExec::DeleteTableFiles(const string& table_name) {
  RetCode ret = rSuccess;
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  TableDescriptor* table_desc = local_catalog->getTable(table_name);
  // start to delete the files
  vector<vector<string>> write_path;
  for (int i = 0; i < table_desc->getNumberOfProjection(); i++) {
    vector<string> prj_write_path;
    prj_write_path.clear();
    for (int j = 0; j < table_desc->getProjectoin(i)
                            ->getPartitioner()
                            ->getNumberOfPartitions();
         ++j) {
      string path = PartitionID(table_desc->getProjectoin(i)->getProjectionID(),
                                j).getPathAndName();
      prj_write_path.push_back(path);
    }
    write_path.push_back(prj_write_path);
    // unbound the file in memory
    if (table_desc->getProjectoin(i)->getPartitioner()->allPartitionBound()) {
      Catalog::getInstance()->getBindingModele()->UnbindingEntireProjection(
          table_desc->getProjectoin(i)->getPartitioner());
    }
  }
  TableFileConnector* connector = new TableFileConnector(
      Config::local_disk_mode ? FilePlatform::kDisk : FilePlatform::kHdfs,
      write_path);
  ret = connector->DeleteAllTableFiles();
  if (ret != rSuccess) {
    ELOG(ret,
         "failed to delete the projections, when delete the file on table" +
             table_name);
    return ret;
  }
  return ret;
}
} /* namespace stmt_handler */
} /* namespace claims */
