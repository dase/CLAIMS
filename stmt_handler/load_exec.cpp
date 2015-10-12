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
#include "../stmt_handler/load_exec.h"

#include "../Environment.h"
#include "../Loader/Hdfsloader.h"
namespace claims {
namespace stmt_handler {

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
int LoadExec::Execute() {
  int ret = common::kStmtHandlerOk;

  if (!isTableExist()) {
    error_msg_ = "the table " + tablename_ + " does not exist during loading!";
    result_flag_ = false;
    result_set_ = NULL;
    ret = common::kStmtHandlerTableExistDuringCreate;
  } else {
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
    /*
    ASTParserLogging::log(
        "The separator are :%c,%c, The sample is %lf, mode is %d\n",
        column_separator[0], tuple_separator[0], load_ast_->sample_,
        load_ast_->mode_);
    */

    // string col_sep = column_separator[0];
    // string tup_sep = tuple_separator[0];
    char buf[100];
    sprintf(buf, "The separator are :%c,%c, The sample is %lf, mode is %d\n",
            column_separator[0], tuple_separator[0], load_ast_->sample_,
            load_ast_->mode_);
#if 0
    LOG(INFO) << "The separator are :" + col_sep + "," + tup_sep +
                     ", The sample is " + load_ast_->sample_ + "," +
                    "mode is " + load_ast_->mode_ << std::endl;
#endif

    LOG(INFO) << buf << std::endl;

    HdfsLoader *loader =
        new HdfsLoader(column_separator[0], tuple_separator[0], path_names,
                       table_desc_, (open_flag)load_ast_->mode_);
    loader->load(load_ast_->sample_);

    result_flag_ = true;
    result_set_ = NULL;
    info_ = "load data successfully";

    Environment::getInstance()->getCatalog()->saveCatalog();
    ret = common::kStmtHandlerCreateTableSuccess;
  }

  return ret;
}

}  // namespace stmt_handler
}  // namespace claims
