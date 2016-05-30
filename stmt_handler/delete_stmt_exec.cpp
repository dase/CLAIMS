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
 * /CLAIMS/stmt_handler/delete_stmt_exec.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: yuyang
 *		   Email: youngfish93@hotmail.com
 *
 * Description:
 *
 */

#include <string>
#include <vector>
#include <iostream>
#include "../Environment.h"
#include "../stmt_handler/delete_stmt_exec.h"

#include "../common/Block/BlockStream.h"
#include "../common/Block/ResultSet.h"
#include "../loader/data_injector.h"
#include "../catalog/table.h"
#include "../catalog/projection.h"
#include "../Daemon/Daemon.h"
#include "../sql_parser/ast_node/ast_select_stmt.h"
#include "../stmt_handler/select_exec.h"
#include "../common/error_define.h"
using claims::loader::DataInjector;
using std::endl;
using std::string;
using std::vector;
using std::cout;
using claims::catalog::TableDescriptor;
using claims::common::rSuccess;
using claims::common::rNoProjection;
using claims::common::rCreateProjectionOnDelTableFailed;
namespace claims {
namespace stmt_handler {

DeleteStmtExec::DeleteStmtExec(AstNode* stmt) : StmtExec(stmt) {
  assert(stmt_);
  delete_stmt_ast_ = dynamic_cast<AstDeleteStmt*>(stmt_);
}

DeleteStmtExec::~DeleteStmtExec() {}

RetCode DeleteStmtExec::Execute(ExecutedResult* exec_result) {
  RetCode ret = rSuccess;
  string table_base_name =
      dynamic_cast<AstTable*>(
          dynamic_cast<AstFromList*>(delete_stmt_ast_->from_list_)->args_)
          ->table_name_;
  TableDescriptor* new_table =
      Environment::getInstance()->getCatalog()->getTable(table_base_name);

  SemanticContext sem_cnxt;
  ret = delete_stmt_ast_->SemanticAnalisys(&sem_cnxt);
  if (rSuccess != ret) {
    exec_result->SetError("Semantic analysis error.\n" + sem_cnxt.error_msg_);
    LOG(ERROR) << "semantic analysis error result= : " << ret;
    cout << "semantic analysis error result= : " << ret << endl;
    return ret;
  }

  /**
   * step1 : create new sql query including row_id, for example:
   * DELETE FROM tbA WHERE colA = 10;
   * =>
   * SELECT row_id FROM tbA WHERE colA = 10;
   */
  AstNode* appended_query_sel_stmt;
  ret = GenerateSelectStmt(table_base_name, appended_query_sel_stmt);
  if (rSuccess == ret) {
    appended_query_sel_stmt->Print();
    // ExecutedResult* appended_result = new ExecutedResult();
    SelectExec* appended_query_exec = new SelectExec(appended_query_sel_stmt);
    ret = appended_query_exec->Execute(exec_result);
    if (ret != rSuccess) {
      WLOG(ret, "failed to find the delete tuples from the table ");
      return ret;
    }
    ostringstream ostr;
    ostr << exec_result->result_->getNumberOftuples() << " tuples deleted.";
    exec_result->info_ = ostr.str();
    // set the flag weather it contains the deleted tuples or not in the
    // base table
    TableDescriptor* table =
        Environment::getInstance()->getCatalog()->getTable(table_base_name);
    table->SetDeletedTuples(true);

    /**
     * step2 : Insert delete data into _DEL table.
     */
    string table_del_name = table_base_name + "_DEL";
    InsertDeletedDataIntoTableDEL(table_del_name, exec_result);

    // release the release of appended_query_sel_stmt and appended_query_exec
    //  if (NULL != appended_query_sel_stmt) {
    //    delete appended_query_sel_stmt;
    //  }
    //  if (NULL != appended_query_exec) {
    //    delete appended_query_exec;
    //  }
    delete exec_result->result_;
    exec_result->result_ = NULL;
    return ret;
  } else if (rCreateProjectionOnDelTableFailed == ret) {
    WLOG(ret,
         "no projections has been created on the del table when delete tuples "
         "from the base table");
    return ret;
  }
}

RetCode DeleteStmtExec::GenerateSelectStmt(const string table_name,
                                           AstNode*& appended_query_sel_stmt) {
  RetCode ret = rSuccess;
  vector<string> column_names;
  string column_name;
  ret = GenerateSelectedColumns(table_name, column_names);
  if (rCreateProjectionOnDelTableFailed == ret) {
    WLOG(ret,
         "create projection on del table failed, since no projection has been "
         "created on the base table");
    return ret;
  }
  //  // add the the row_id column
  //  AstNode* appended_query_col =
  //      new AstColumn(AST_COLUMN, table_name, "row_id", NULL);
  //  AstNode* appended_query_sel_expr =
  //      new AstSelectExpr(AST_SELECT_EXPR, "", appended_query_col);
  //  AstNode* appended_query_sel_list_last =
  //      new AstSelectList(AST_SELECT_LIST, false, appended_query_sel_expr,
  //      NULL);
  AstNode* appended_query_sel_list = NULL;
  AstNode* appended_query_sel_list_last = NULL;
  vector<string>::reverse_iterator r_iter = column_names.rbegin();
  // -1 means the column row_id of the base table do not need to be selected
  for (; r_iter != column_names.rend() - 1; r_iter++) {
    if ("row_id_DEL" != *r_iter) {
      AstNode* appended_query_col =
          new AstColumn(AST_COLUMN, table_name, *r_iter, NULL);
      AstNode* appended_query_sel_expr =
          new AstSelectExpr(AST_SELECT_EXPR, "", appended_query_col);
      appended_query_sel_list =
          new AstSelectList(AST_SELECT_LIST, false, appended_query_sel_expr,
                            appended_query_sel_list_last);
      appended_query_sel_list_last = appended_query_sel_list;
    } else {
      column_name = *r_iter;
      column_name = column_name.substr(0, column_name.size() - 4);
      AstNode* appended_query_col =
          new AstColumn(AST_COLUMN, table_name, column_name, NULL);
      AstNode* appended_query_sel_expr =
          new AstSelectExpr(AST_SELECT_EXPR, "", appended_query_col);
      appended_query_sel_list =
          new AstSelectList(AST_SELECT_LIST, false, appended_query_sel_expr,
                            appended_query_sel_list_last);
      appended_query_sel_list_last = appended_query_sel_list;
    }
  }
  appended_query_sel_stmt = new AstSelectStmt(
      AST_SELECT_STMT, 0, appended_query_sel_list, delete_stmt_ast_->from_list_,
      delete_stmt_ast_->where_list_, NULL, NULL, NULL, NULL, NULL);
  return ret;
}

RetCode DeleteStmtExec::GenerateSelectedColumns(const string table_name,
                                                vector<string>& column_names) {
  RetCode ret = rSuccess;
  TableDescriptor* table_del =
      Environment::getInstance()->getCatalog()->getTable(table_name + "_DEL");
  string column_name;
  vector<Attribute> columns = table_del->getAttributes();
  if (0 == columns.size()) {
    ret = rNoProjection;
    WLOG(ret, "no projection is created on table:" + table_name +
                  "when creating projection on the del table");
    return ret;
  } else {
    for (auto& attribute : columns) {
      column_name = attribute.getName();
      auto pos = column_name.find(".");
      column_name = column_name.substr(pos + 1, column_name.size() - pos);
      column_names.push_back(column_name);
    }
    return ret;
  }

  //  string column_name;
  //  const vector<ProjectionDescriptor*>* projection_list =
  //      table_base->GetProjectionList();
  //  if (0 == (*projection_list).size()) {
  //    ret = rNoProjection;
  //    WLOG(ret, "no projection is created on table:" + table_name +
  //                  "when creating projection on the del table");
  //    return ret;
  //  } else {
  //    for (int i = 0; i < projection_list->size(); i++) {
  //      column_name =
  //          (*projection_list)[i]->getPartitioner()->getPartitionKey().getName();
  //      auto pos = column_name.find(".");
  //      column_name = column_name.substr(pos + 1, column_name.size() - pos);
  //      partition_attributes.push_back(column_name);
  //    }
  //    return ret;
  //  }
}

void DeleteStmtExec::InsertDeletedDataIntoTableDEL(
    string table_del_name, ExecutedResult* exec_result) {
  DynamicBlockBuffer::Iterator it = exec_result->result_->createIterator();
  BlockStreamBase* block = NULL;
  BlockStreamBase::BlockStreamTraverseIterator* tuple_it = NULL;

  cout << "insert del table name : " << table_del_name.c_str();
  TableDescriptor* table_del =
      Environment::getInstance()->getCatalog()->getTable(table_del_name);
  if (NULL == table_del) {
    LOG(ERROR) << "The table DEL " + table_del_name +
                      " is not existed during delete data." << std::endl;
    return;
  }
  /**
   *    Prepare the data which will be insert into TABLE_DEL.
   */
  ostringstream ostr;
  while (block = it.nextBlock()) {
    tuple_it = block->createIterator();
    void* tuple;
    while (tuple = tuple_it->nextTuple()) {
      for (unsigned i = 0; i < exec_result->result_->column_header_list_.size();
           i++) {
        // check whether the row has been deleted or not.
        if (true) {
          ostr << exec_result->result_->schema_->getcolumn(i)
                      .operate->toString(exec_result->result_->schema_
                                             ->getColumnAddess(i, tuple))
                      .c_str();
          ostr << "|";
        } else {
          continue;
        }
      }
      ostr << "\n";
    }
    delete tuple_it;
  }

  DataInjector* injector = new DataInjector(table_del);
  injector->InsertFromString(ostr.str(), exec_result);
  //  HdfsLoader* Hl = new HdfsLoader(tabledel);
  //  string tmp = ostr.str();
  //  Hl->append(ostr.str());
  //  cout << tmp << endl;
  Environment::getInstance()->getCatalog()->saveCatalog();
}

} /* namespace stmt_handler */
} /* namespace claims */
