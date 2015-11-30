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
 * /CLAIMS/stmt_handler/load_exec.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: cswang
 *       Email: cs_wang@infosys.com
 *
 * Description:
 *    this file is the function body of class LoadExec.
 */

#include <assert.h>
#include <vector>
#include <string>
#include "../stmt_handler/load_exec.h"
#include "../Environment.h"
#include "../loader/data_injector.h"
using std::vector;
using claims::loader::DataInjector;
namespace claims {
namespace stmt_handler {
#define NEWRESULT
#define NEW_LOADER
LoadExec::LoadExec(AstNode *stmt) : StmtExec(stmt) {
  // TODO Auto-generated constructor stub
  assert(stmt_);
  load_ast_ = dynamic_cast<AstLoadTable *>(stmt_);
  tablename_ = load_ast_->table_name_;
  table_desc_ = Environment::getInstance()->getCatalog()->getTable(tablename_);
}

LoadExec::~LoadExec() {
  // TODO Auto-generated destructor stub
}
/**
 * @brief load data from file system.
 * @details check whether the table we have created or not. forbid to load data
 * into an nonexistent table.
 *  then add path names from path node to a vector path_names,
 *  create new HdfsLoader, load data.
 */
RetCode LoadExec::Execute(ExecutedResult *exec_result) {
  RetCode ret = rSuccess;

  SemanticContext sem_cnxt;
  ret = load_ast_->SemanticAnalisys(&sem_cnxt);
  if (rSuccess != ret) {
    exec_result->SetError("Semantic analysis error.\n" + sem_cnxt.error_msg_);
    //    exec_result->error_info_ =
    //        "Semantic analysis error.\n" + sem_cnxt.error_msg_;
    //    exec_result->status_ = false;
    LOG(ERROR) << "semantic analysis error result= : " << ret;
    cout << "semantic analysis error result= : " << ret << endl;
    return ret;
  }

  TableDescriptor *table = Environment::getInstance()->getCatalog()->getTable(
      load_ast_->table_name_);

  //  if (!isTableExist()) {
  //    exec_result->SetError("the table " + load_ast_->table_name_ +
  //                          " does not exist during loading!");
  //    ret = common::rStmtHandlerTableExistDuringCreate;
  //    return ret;
  //  } else if (table->getNumberOfProjection() == 0) {
  //    exec_result->SetError("the table has not been created a projection!");
  //    ret = common::rNoProjection;
  //    return ret;
  //  } else {
  string column_separator = load_ast_->column_separator_;
  string tuple_separator = load_ast_->tuple_separator_;
  AstExprList *path_node = dynamic_cast<AstExprList *>(load_ast_->path_);

  // ASTParserLogging::log("load file\'s name:");
  LOG(INFO) << "load file's name:" << std::endl;
  std::vector<string>
      path_names;  // save the name of files which should be loaded
  // for test: the path name is:   /home/imdb/data/tpc-h/part.tbl
  while (path_node) {
    AstExprConst *data = dynamic_cast<AstExprConst *>(path_node->expr_);
    // ASTParserLogging::log("%s", data->data_.c_str());
    LOG(INFO) << data->data_ << std::endl;
    path_names.push_back(data->data_);
    path_node = dynamic_cast<AstExprList *>(path_node->next_);
  }

// split sign should be considered carefully, in case of it may be "||" or
// "###"

// string col_sep = column_separator[0];
// string tup_sep = tuple_separator[0];
// char buf[100];

/*    sprintf(buf, "The separator are :%c,%c, The sample is %lf, mode is %d\n",
            column_separator[0], tuple_separator[0], load_ast_->sample_,
            load_ast_->mode_);*/
#if 1
  LOG(INFO) << "The separator are :" + column_separator + "," +
                   tuple_separator + ", The sample is " << load_ast_->sample_
            << ", mode is " << load_ast_->mode_ << std::endl;
#endif
  GETCURRENTTIME(start_time);
// LOG(INFO) << buf << std::endl;
#ifdef NEW_LOADER
  DataInjector *injector =
      new DataInjector(table, column_separator, tuple_separator);
  static char *load_output_info[] = {"Loading", "Loading.", "Loading..",
                                     "Loading..."};
  ret = injector->LoadFromFile(path_names,
                               static_cast<FileOpenFlag>(load_ast_->mode_),
                               exec_result, load_ast_->sample_);
  double load_time_ms = GetElapsedTime(start_time);
  LOG(INFO) << " load time: " << load_time_ms / 1000.0 << " sec" << endl;
  if (ret != rSuccess) {
    LOG(ERROR) << "failed to load files: ";
    for (auto it : path_names) {
      LOG(ERROR) << it << " ";
    }
    LOG(ERROR) << " into table " << table->getTableName() << endl;

    if (exec_result->error_info_ == "")
      exec_result->SetError("failed to load data");
  } else {
    ostringstream oss;
    oss << "load data successfully (" << load_time_ms / 1000.0 << " sec) ";
    exec_result->SetResult(oss.str(), NULL);
  }
  DELETE_PTR(injector);
#else
  Hdfsloader *loader =
      new Hdfsloader(column_separator[0], tuple_separator[0], path_names, table,
                     (FileOpenFlag)new_node->mode);
  loader->load(new_node->sample);

  result->SetResult("load data successfully", NULL);
#endif
  Environment::getInstance()->getCatalog()->saveCatalog();
  //  }

  return ret;
}

}  // namespace stmt_handler
}  // namespace claims
