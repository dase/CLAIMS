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
 * /CLAIMS/stmt_handler/insert_exec.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 * Description:
 *      this file is the function body of class InsertExec.
 *
 */

#include <assert.h>
#include "../stmt_handler/insert_exec.h"

#include "../catalog/table.h"
#include "../catalog/catalog.h"
#include "../Environment.h"
#include "../loader/data_injector.h"
#include "../common/error_define.h"

using claims::catalog::Catalog;
using claims::common::rSuccess;
using claims::common::FileOpenFlag;
using claims::loader::DataInjector;
using claims::common::rNotSupport;
using claims::catalog::TableDescriptor;
using claims::common::rTableNotExist;
#define NEW_LOADER
namespace claims {
namespace stmt_handler {

const int InsertExec::INT_LENGTH = 10;
const int InsertExec::FLOAT_LENGTH = 10;
const int InsertExec::SMALLINT_LENGTH = 4;

#define NEWRESULT
/**
 * @brief Constructor
 * @detail convert the base class member stmt_ to insert_ast_ and get the table
 * name,
 *  firstly get the descriptor from catalog by table name.
 *  initialize members has_warning_, is_correct_, is_all_col_ to default values.
 */
InsertExec::InsertExec(AstNode *stmt)
    : StmtExec(stmt),
      has_warning_(false),
      is_correct_(true),
      is_all_col_(false) {
  assert(stmt_);
  insert_ast_ = static_cast<AstInsertStmt *>(stmt_);
  tablename_ = insert_ast_->table_name_;
  table_desc_ = Environment::getInstance()->getCatalog()->getTable(tablename_);
}

InsertExec::~InsertExec() {}

RetCode InsertExec::InsertValueToStream(AstInsertVals *insert_value,
                                        TableDescriptor *table,
                                        unsigned position,
                                        std::ostringstream &ostr) {
  RetCode ret = rSuccess;
  bool has_warning = true;
  if (insert_value->value_type_ == 0) {
    // check whether the column type match value type

    switch (insert_value->expr_->ast_node_type()) {
      // 2014-4-17---only these type are supported now---by Yu

      //      case t_stringval:
      //      case t_intnum:
      //      case t_approxnum:
      //      case t_bool: {
      case AST_EXPR_CONST: {
        AstExprConst *expr = dynamic_cast<AstExprConst *>(insert_value->expr_);
        DLOG(INFO) << (expr->data_ == "" ? "NULL" : expr->data_) << endl;
        ostr << expr->data_;
        break;
      }
      // case t_expr_cal: {
      case AST_EXPR_CAL_BINARY: {
        // TODO(ANYONE): the value to insert may be a expr like
        // "-234+56*82/2 > 23", which should be supported
        ret = rNotSupport;
        ELOG(ret, "Insert a expr as value is not supported, including '-1' ");
        break;
      }
      case t_name: {
        AstColumn *col = dynamic_cast<AstColumn *>(insert_value->expr_);
        DLOG(INFO) << (col->column_name_ == "" ? "NULL" : col->column_name_)
                   << endl;
        ostr << col->column_name_;
        break;
      }
      default: {
        ret = rNotSupport;
        ELOG(ret, "type: " << insert_value->expr_->ast_node_type());
      }
    }
  } else if (insert_value->ast_node_type() == 1) {
    string data;
    table->getAttribute(position).attrType->operate->SetDefault(data);
    ostr << data;
  }  // 设置为default, 暂不支持
  return ret;
}

/**
 *  check whether the string is digit, can use strtol()
 */
bool InsertExec::CheckType(const column_type *col_type, AstNode *expr) {
  AstNodeType insert_value_type = expr->ast_node_type();
  AstExprConst *pexpr = dynamic_cast<AstExprConst *>(expr);
  switch (col_type->type) {
    case t_int:
      return (insert_value_type != AST_INTNUM) ||
             (pexpr->data_.length()) > INT_LENGTH;
    case t_float:
      return !(insert_value_type == AST_APPROXNUM) ||
             (pexpr->data_.length()) > FLOAT_LENGTH;
    case t_double:
      return (insert_value_type != AST_APPROXNUM);
    case t_string:
      return (insert_value_type != AST_STRINGVAL) ||
             (pexpr->data_.length()) > col_type->get_length();
    // case t_u_long: return (insert_value_type != t_intnum) ||
    // strlen(expr->data) > INT_LENGTH || (expr->data[0] == '-'); // =='-'
    // 实际不可行，‘-’不会被识别进expr中
    case t_date:
      return false;
    case t_time:
      return false;
    case t_datetime:
      return false;
    case t_smallInt:
      return (insert_value_type != AST_INTNUM) ||
             (pexpr->data_.length()) > SMALLINT_LENGTH;
    case t_decimal:
      return (insert_value_type != AST_INTNUM);
    // case t_u_smallInt: return (insert_value_type != t_intnum) ||
    // (expr->data[0] == '-') || length(expr->data) > INT_LENGTH;
    //  strtol();
    default:
      return true;
  }
}

/**
 * @brief insert data
 * @detail check whether the table we have created or not. forbid to insert data
 * into an nonexistent table.
 *  There are two different cases when we insert data:
 *    1. insert all columns
 *    2. insert part of all columns
 *  To case 1, we just insert all rows values and make sure the value count and
 * type match each column.
 *  To case 2, make sure each value correspond to its column and its type.
 *  If no exceptions and errors, data will be stored into tables and hdfs proper
 * time.
 * @return a result code cooperate with the client.
 */
RetCode InsertExec::Execute(ExecutedResult *exec_result) {
  int ret = rSuccess;
  string table_name(insert_ast_->table_name_);
  TableDescriptor *table =
      Environment::getInstance()->getCatalog()->getTable(table_name);
  if (table == NULL) {
    ret = rTableNotExist;
    exec_result->SetError("The table " + table_name + " does not exist!");
    ELOG(ret, "table name: " << table_name);
    return ret;
  }

  unsigned col_count = 0;
  AstColumn *col = dynamic_cast<AstColumn *>(insert_ast_->col_list_);
  if (NULL == col) {
    is_all_col_ = true;  // insert all columns
  } else {               // get insert column count
    ++col_count;
    while ((col = dynamic_cast<AstColumn *>(col->next_))) ++col_count;
  }

  AstInsertValList *insert_value_list =
      dynamic_cast<AstInsertValList *>(insert_ast_->insert_val_list_);
  if (NULL == insert_value_list) {
#ifdef NEWRESULT
    LOG(ERROR) << "No value!" << endl;
    exec_result->SetError("No value!");
#else
    error_msg_ = "No value!";
    result_flag_ = false;
    result_set_ = NULL;
#endif
    ret = common::rStmtHandlerInsertNoValue;
  } else {
    std::ostringstream ostr;
    int changed_row_num = 0;

    // get data in one row like (...), (...), (...) by while loop.
    while (insert_value_list) {
      // make sure: the insert column count = insert value count = used column
      // count = used value count
      AstInsertVals *insert_value =
          dynamic_cast<AstInsertVals *>(insert_value_list->insert_vals_);
      col = dynamic_cast<AstColumn *>(insert_ast_->col_list_);

      if (is_all_col_) {
        // by scdong: Claims adds a default row_id attribute for all tables
        // which is attribute(0),
        // when inserting tuples we should begin to construct the string_tuple
        // from the second attribute.
        for (unsigned int position = 1;
             position < table_desc_->getNumberOfAttribute(); position++) {
          // check value count
          if (insert_value == NULL) {
// ASTParserLogging::elog("Value count is too few");
#ifdef NEWRESULT
            LOG(ERROR) << "Value count is too few" << endl;
            exec_result->SetError("Value count is too few");
            is_correct_ = false;
#else
            is_correct_ = false;
            error_msg_ = "Value count is too few";
            result_flag_ = false;
            result_set_ = NULL;
#endif
            break;
          }

          // insert value to ostringstream and if has warning return 1;   look
          // out   the order!
          ret = InsertValueToStream(insert_value, table_desc_, position, ostr);
          // move to next
          insert_value = dynamic_cast<AstInsertVals *>(insert_value->next_);
          ostr << "|";
        }

        if (rSuccess != ret) break;
        // check insert value count
        if (NULL == insert_value) {
#ifdef NEWRESULT
          LOG(ERROR) << "Value count is too many";
          exec_result->SetError("Value count is too many");
          is_correct_ = false;
#else
          error_msg_ = "Value count is too many";
          result_flag_ = false;
          result_set_ = NULL;
          is_correct_ = false;
#endif
          break;
        }

      } else {  // insert part of columns
        // get insert value count and check whether it match column count
        unsigned insert_value_count = 0;
        while (insert_value) {
          ++insert_value_count;
          insert_value = dynamic_cast<AstInsertVals *>(insert_value->next_);
        }
        if (insert_value_count != col_count) {
// ASTParserLogging::elog("Column count doesn't match value count");
#ifdef NEWRESULT
          LOG(ERROR) << "Column count doesn't match value count";
          exec_result->SetError("Column count doesn't match value count");
          is_correct_ = false;
#else
          error_msg_ = "Column count doesn't match value count";
          LOG(ERROR) << "Column count doesn't match value count" << std::endl;
          result_flag_ = false;
          result_set_ = NULL;
          is_correct_ = false;
#endif
          break;
        }
        unsigned int used_col_count = 0;
        // by scdong: Claims adds a default row_id attribute for all tables
        // which
        // is attribute(0), when inserting tuples we should begin to construct
        // the
        // string_tuple from the second attribute.
        for (unsigned int position = 1;
             position < table_desc_->getNumberOfAttribute(); position++) {
          // find the matched column and value by name
          col = dynamic_cast<AstColumn *>(insert_ast_->col_list_);
          AstInsertVals *insert_value =
              dynamic_cast<AstInsertVals *>(insert_value_list->insert_vals_);

          // take attention that attrName is tablename.colname
          // here is temporary code and need to change later
          while (col &&
                 (table_desc_->getAttribute(position).attrName).compare(
                     table_desc_->getTableName() + "." + col->relation_name_)) {
            col = dynamic_cast<AstColumn *>(col->next_);
            insert_value = dynamic_cast<AstInsertVals *>(insert_value->next_);
          }

          // if find the column count is proved to match the insert value
          // count, so column exist, then insert_value exist
          if (col && insert_value) {
            ++used_col_count;
            // insert value to ostringstream and if has warning return 1; look
            // out the order!
            if (rSuccess != (ret = InsertValueToStream(insert_value, table,
                                                       position, ostr))) {
              ELOG(ret, "failed to insert value to stream");
              is_correct_ = false;
              exec_result->SetError("Not supported type to insert");
              break;
            }
          }
          ostr << "|";
        }

        // check if every insert column is existed
        if (used_col_count != col_count) {
// ASTParserLogging::elog("Some columns don't exist");
#ifdef NEWRESULT
          exec_result->SetError("Some columns don't exist");
          LOG(ERROR) << "Some columns don't exist" << std::endl;
#else
          error_msg_ = "Some columns don't exist";
          LOG(ERROR) << "Some columns don't exist" << std::endl;
          result_flag_ = false;
          result_set_ = NULL;
          is_correct_ = false;
#endif
          break;
        }
      }

      if (!is_correct_) break;

      insert_value_list =
          dynamic_cast<AstInsertValList *>(insert_value_list->next_);
      if (insert_value_list != NULL) ostr << "\n";

      ++changed_row_num;

    }  // while

//    if (is_correct_) {
//      if (has_warning_) {
//// ASTParserLogging::log("[WARNING]: The type is not matched!\n");
//        exec_result->info_ = "The type is not matched";
//        LOG(WARNING) << "The type is not matched!" << std::endl;
//      }
//      // ASTParserLogging::log("the insert content is \n%s\n",
//      // ostr.str().c_str());
//      LOG(INFO) << "the insert content is " << std::endl << ostr.str()
//                << std::endl;
//
//      HdfsLoader *Hl = new HdfsLoader(table_desc_);
//      string tmp = ostr.str();
//      Hl->append(ostr.str());
//
//      Environment::getInstance()->getCatalog()->saveCatalog();
//      result_flag_ = true;
//      ostr.clear();
//      ostr.str("");
//      ostr << "insert data successfully. " << changed_row_num
//           << " rows changed.";
//      exec_result->info_ = ostr.str();
//      exec_result->result_ = NULL;
//
//      ret = common::kStmtHandlerInsertDataSuccess;
//    }
#ifdef NEW_LOADER
    DataInjector *injector = new DataInjector(table);

    // str() will copy string buffer without the last '\n'
    ret = injector->InsertFromString(ostr.str() + "\n", exec_result);
    if (rSuccess == ret) {
      ostr.clear();
      ostr.str("");
      ostr << "insert data successfully. " << changed_row_num
           << " rows changed.";
      exec_result->SetResult(ostr.str(), NULL);
    } else {
      LOG(ERROR) << "failed to insert tuples into table:"
                 << table->getTableName() << endl;

      exec_result->SetError("failed to insert tuples into table ");
    }
    DELETE_PTR(injector);
#else
    Hdfsloader *Hl = new Hdfsloader(table);
    Hl->append(ostr.str());
    delete Hl;
#endif

    Environment::getInstance()->getCatalog()->saveCatalog();
  }

  return ret;
}

}  // namespace stmt_handler
}  // namespace claims
