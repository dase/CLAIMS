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
#include "../../common/error_define.h"
using std::vector;

namespace claims {
namespace stmt_handler {

CreateProjectionExec::CreateProjectionExec(AstNode* stmt) : StmtExec(stmt) {
  assert(stmt_);

  result_flag_ = true;
  create_projection_ast_ = dynamic_cast<AstCreateProjection*>(stmt_);
}

CreateProjectionExec::~CreateProjectionExec() {}

RetCode CreateProjectionExec::Execute(ExecutedResult* exec_result) {
  RetCode ret = rSuccess;

  SemanticContext sem_cnxt;
  sem_cnxt.index_.push_back(0);
  ret = create_projection_ast_->SemanticAnalisys(&sem_cnxt);
  if (rSuccess != ret) {
    exec_result->error_info_ =
        "Semantic analysis error.\n" + sem_cnxt.error_msg_;
    exec_result->status_ = false;
    LOG(ERROR) << "semantic analysis error result= : " << ret;
    cout << "semantic analysis error result= : " << ret << endl;
    return ret;
  }

  /**
   * Start Create projection for table.
   */
  int partition_num = create_projection_ast_->partition_num_;
  string tablename = create_projection_ast_->table_name_;
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  TableDescriptor* table =
      local_catalog->getTable(create_projection_ast_->table_name_);

  TableID table_id = table->get_table_id();
  string partition_attribute_name =
      create_projection_ast_->partition_attribute_name_;

  local_catalog->getTable(table_id)->createHashPartitionedProjection(
      sem_cnxt.index_, partition_attribute_name, partition_num);
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
  /**
   *  Start create projection  for table_del.
   */
  table = local_catalog->getTable((tablename + "_DEL"));
  if (NULL != table) {
    table_id = local_catalog->getTable(tablename + "_DEL")->get_table_id();
    vector<ColumnOffset> indexDEL;
    indexDEL.push_back(0);

    if (table->isExist(tablename + "_DEL.row_id_DEL")) {
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

  exec_result->status_ = true;
  exec_result->result_ = NULL;
  exec_result->info_ = "create projection successfully";

  return ret;
}

} /* namespace stmt_handler */
} /* namespace claims */
