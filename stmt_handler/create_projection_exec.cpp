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
 * /CLAIMS/stmt_handler/create_projection_exec.cpp
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
#include <assert.h>  // NOLINT
#include "../stmt_handler/create_projection_exec.h"
#include "../Environment.h"
using std::vector;

namespace claims {
namespace stmt_handler {

CreateProjectionExec::CreateProjectionExec(AstNode* stmt) : StmtExec(stmt) {
  assert(stmt_);

  result_flag_ = true;
  create_projection_ast_ = dynamic_cast<AstCreateProjection*>(stmt_);
}

CreateProjectionExec::~CreateProjectionExec() {}

RetCode CreateProjectionExec::Execute(executed_result* exec_result) {
  int ret = rSuccess;
  int partition_num = create_projection_ast_->partition_num_;
  string tablename = create_projection_ast_->table_name_;
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  TableDescriptor* table =
      local_catalog->getTable(create_projection_ast_->table_name_);

  if (NULL == table) {
    exec_result->error_info = "There is no table named " +
                              create_projection_ast_->table_name_ +
                              " during creating projection";
    exec_result->status = false;
    exec_result->result = NULL;
    LOG(ERROR) << "There is no table named "
               << create_projection_ast_->table_name_
               << " during creating projection";
    // TODO(yuyang): add error number
    return rParserError;
  }

  TableID table_id = table->get_table_id();
  string partition_attribute_name =
      create_projection_ast_->partition_attribute_name_;

  vector<ColumnOffset> index;
  index.push_back(0);
  AstColumn* col_list =
      dynamic_cast<AstColumn*>(create_projection_ast_->column_list_);
  string colname;
  while (col_list) {
    if ("NULL" != col_list->column_name_) {
      colname = col_list->column_name_;
    } else if ("NULL" != col_list->relation_name_) {
      colname = col_list->relation_name_;
    } else {
      exec_result->error_info = "No column name during creating projection.";
      exec_result->status = false;
      exec_result->result = NULL;
      // TODO(yuyang): add error number
      return rParserError;
      break;
    }
    cout << tablename + "." + colname << endl;
    if (table->isExist(tablename + "." + colname)) {
      index.push_back(table->getAttribute(colname).index);
    } else {
      exec_result->error_info =
          "The column " + colname + "is not existed during creating projection";
      exec_result->status = false;
      exec_result->result = NULL;
      // TODO(yuyang): add error number
      return rParserError;
      break;
    }
    col_list = dynamic_cast<AstColumn*>(col_list)->next_;
  }

  local_catalog->getTable(table_id)->createHashPartitionedProjection(
      index, partition_attribute_name, partition_num);
  int projection_index =
      local_catalog->getTable(table_id)->getNumberOfProjection() - 1;
  for (unsigned i = 0; i < local_catalog->getTable(table_id)
                               ->getProjectoin(projection_index)
                               ->getPartitioner()
                               ->getNumberOfPartitions();
       i++) {
    local_catalog->getTable(table_id)
        ->getProjectoin(projection_index)
        ->getPartitioner()
        ->RegisterPartition(i, 0);
  }

  // create projection start ( for table_del )
  table = local_catalog->getTable((tablename + "_DEL"));
  if (NULL != table) {
    table_id = local_catalog->getTable(tablename + "_DEL")->get_table_id();
    vector<ColumnOffset> indexDEL;
    indexDEL.push_back(0);

    if (table->isExist(tablename + "_DEL" + "row_id_DEL")) {
      indexDEL.push_back(table->getAttribute("row_id_DEL").index);
    }

    local_catalog->getTable(table_id)->createHashPartitionedProjection(
        indexDEL, "row_id_DEL", partition_num);

    projection_index =
        local_catalog->getTable(table_id)->getNumberOfProjection() - 1;

    for (unsigned i = 0; i < local_catalog->getTable(table_id)
                                 ->getProjectoin(projection_index)
                                 ->getPartitioner()
                                 ->getNumberOfPartitions();
         i++) {
      local_catalog->getTable(table_id)
          ->getProjectoin(projection_index)
          ->getPartitioner()
          ->RegisterPartition(i, 0);
    }
  }

  local_catalog->saveCatalog();

  exec_result->status = true;
  exec_result->result = NULL;
  exec_result->info = "create projection successfully";

  return rSuccess;
}

} /* namespace stmt_handler */
} /* namespace claims */
