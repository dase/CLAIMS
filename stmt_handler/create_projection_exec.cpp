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
#include "../catalog/table.h"
#include "../Environment.h"
#include "../../common/error_define.h"
using std::vector;
using claims::common::rStmtHandlerCreateProjectionWithEmptyColumn;
using claims::common::rSuccess;
using claims::common::rTableNotExisted;

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
  string table_name = create_projection_ast_->table_name_;
  string partition_attribute_name =
      create_projection_ast_->partition_attribute_name_;
  Catalog* local_catalog = Environment::getInstance()->getCatalog();
  ret = CreateTableProjection(table_name, local_catalog, sem_cnxt.index_,
                              partition_attribute_name, partition_num);
  if (common::rResourceIsLocked == ret) {
    exec_result->SetError(
        "Can't create new projection when loading/ inserting data");
  } else if (rSuccess == ret) {
    exec_result->status_ = true;
    exec_result->result_ = NULL;
    exec_result->info_ = "create projection successfully";
// if the projection on the del table failed, the projection creation on the
// based table failed too
#ifdef CREATE_DEL_TABLE
    /**
     * start to create the projection no the del table
     */
    string table_del_name = table_name + "_DEL";
    vector<ColumnOffset> columns;
    columns.push_back(0);  // the row_id of the  del table
    columns.push_back(1);  // the row_id_DEL of del table, which is
                           // corresponding to the row_id of the base table;
    if ("row_id" != partition_attribute_name) {
      AddPartitionAttributeToDel(table_name, partition_attribute_name,
                                 local_catalog);
      vector<ColumnOffset> columns;
      columns.push_back(0);  // the row_id of the  del table
      columns.push_back(1);  // the row_id_DEL of del table, which is
                             // corresponding to the row_id of the base table;
      columns.push_back(local_catalog->getTable(table_del_name)
                            ->getAttribute(partition_attribute_name)
                            .GetAttributeIndex());
      ret = CreateTableProjection(table_del_name, local_catalog, columns,
                                  partition_attribute_name, partition_num);

    } else {
      ret = CreateTableProjection(table_del_name, local_catalog, columns,
                                  partition_attribute_name + "_DEL",
                                  partition_num);
    }
    if (rStmtHandlerCreateProjectionWithEmptyColumn == ret) {
      WLOG(ret,
           "the column set is empty when create projection on the del table:" +
               table_del_name);
      return ret;
    } else if (rTableNotExisted == ret) {
      WLOG(ret,
           "the del table is not existed when creating projection on it: " +
               table_del_name);
      return ret;
    }

#endif
    local_catalog->saveCatalog();
    return ret;
  } else {
    return ret;
  }
  return ret;
}

RetCode CreateProjectionExec::CreateTableProjection(
    const string& table_name, Catalog*& catalog,
    const vector<ColumnOffset>& columns, const string& partition_attribute,
    const int& partition_num) {
  RetCode ret = rSuccess;
  TableDescriptor* table = catalog->getTable(table_name);
  TableID table_id;

  if (NULL != table) {
    table_id = table->get_table_id();
    //    vector<ColumnOffset> indexDEL;
    //    indexDEL.push_back(0);
    if (0 != columns.size()) {
      EXEC_AND_RETURN_ERROR(
          ret, catalog->getTable(table_id)->createHashPartitionedProjection(
                   columns, partition_attribute, partition_num),
          "failed to create projection");

      int projection_index =
          catalog->getTable(table_id)->getNumberOfProjection() - 1;
      Partitioner* partitioner = catalog->getTable(table_id)
                                     ->getProjectoin(projection_index)
                                     ->getPartitioner();
      for (unsigned i = 0; i < partitioner->getNumberOfPartitions(); i++) {
        catalog->getTable(table_id)
            ->getProjectoin(projection_index)
            ->getPartitioner()
            ->RegisterPartition(i, 0);
      }
    } else {
      ret = rStmtHandlerCreateProjectionWithEmptyColumn;
      WLOG(ret, "no columns are given when creating projection on table:" +
                    table_name);
      return ret;
    }
  } else {
    ret = rTableNotExisted;
    WLOG(ret,
         "the dedicated table is not existed when creating projection on it" +
             table_name);
    return ret;
  }
  return ret;
}

RetCode CreateProjectionExec::AddPartitionAttributeToDel(
    const string& table_name, const string& partition_attribute_name,
    Catalog*& catalog) {
  TableDescriptor* table_base = catalog->getTable(table_name);
  Attribute partition = table_base->getAttribute(partition_attribute_name);
  string table_del_name = table_name + "_DEL";
  TableDescriptor* table_del = catalog->getTable(table_del_name);
  partition.SetAttributeName(table_del_name + "." + partition_attribute_name);
  partition.SetAttributeIndex(table_del->getNumberOfAttribute());
  partition.SetTableID(table_del->get_table_id());
  table_del->addAttribute(partition);
}

} /* namespace stmt_handler */
} /* namespace claims */
