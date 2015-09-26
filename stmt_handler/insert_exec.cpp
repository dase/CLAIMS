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

namespace claims {
namespace stmt_handler {

const int InsertExec::INT_LENGTH = 10;
const int InsertExec::FLOAT_LENGTH = 10;
const int InsertExec::SMALLINT_LENGTH = 4;

/**
 * @brief Constructor
 * @detail convert the base class member stmt_ to insert_ast_ and get the table name,
 *  firstly get the descriptor from catalog by table name.
 *  initialize members has_warning_, is_correct_, is_all_col_ to default values.
 */
InsertExec::InsertExec(AstNode* stmt)
    : StmtExec(stmt),
      has_warning_(false),
      is_correct_(true),
      is_all_col_(false) {
  // TODO Auto-generated constructor stub
  assert(stmt_);
  insert_ast_ = static_cast<AstInsertStmt*>(stmt_);
  tablename_ = insert_ast_->table_name_;
  table_desc_ = Environment::getInstance()->getCatalog()->getTable(tablename_);
}

InsertExec::~InsertExec() {
  // TODO Auto-generated destructor stub

}

bool InsertExec::InsertValueToStream(AstInsertVals *insert_value,
                                     TableDescriptor *table, unsigned position,
                                     std::ostringstream &ostr) {
  bool has_warning = true;
  if (insert_value->value_type_ == 0) {
    // check whether the column type match value type
    has_warning = CheckType(table->getAttribute(position).attrType,
                            insert_value->expr_);
    // 2014-4-17---only these type are supported now---by Yu
    switch (insert_value->expr_->ast_node_type())
    {
      case AST_STRINGVAL:
      case AST_INTNUM:
      case AST_APPROXNUM:
      case AST_BOOL: {
        AstExprConst *expr = dynamic_cast<AstExprConst*>(insert_value->expr_);
        ostr << expr->data_;
      }
      break;
      default: {

      }
    }
  }
  else if (insert_value->ast_node_type() == 1) {

  }
  return has_warning;
}

/**
 *  check whether the string is digit, can use strtol()
 */
bool InsertExec::CheckType(const column_type *col_type, AstNode *expr) {
  AstNodeType insert_value_type = expr->ast_node_type();
  AstExprConst *pexpr = dynamic_cast<AstExprConst*>(expr);
  switch (col_type->type) {
    case t_int:
      return (insert_value_type != AST_INTNUM)
          || (pexpr->data_.length()) > INT_LENGTH;
    case t_float:
      return !(insert_value_type == AST_APPROXNUM)
          || (pexpr->data_.length()) > FLOAT_LENGTH;
    case t_double:
      return (insert_value_type != AST_APPROXNUM);
    case t_string:
      return (insert_value_type != AST_STRINGVAL)
          || (pexpr->data_.length()) > col_type->get_length();
    case t_time:
      return false;
    case t_datetime:
      return false;
    case t_smallInt:
      return (insert_value_type != AST_INTNUM)
          || (pexpr->data_.length()) > SMALLINT_LENGTH;
    case t_decimal:
      return (insert_value_type != AST_INTNUM);
    default:
      return true;
  }

}

/**
 * @brief insert data
 * @detail check whether the table we have created or not. forbid to insert data into an nonexistent table.
 *  There are two different cases when we insert data:
 *    1. insert all columns
 *    2. insert part of all columns
 *  To case 1, we just insert all rows values and make sure the value count and type match each column.
 *  To case 2, make sure each value correspond to its column and its type.
 *  If no exceptions and errors, data will be stored into tables and hdfs proper time.
 * @return a result code cooperate with the client.
 */
int InsertExec::Execute() {

  int ret = common::kStmtHandlerOk;

  if (isTableExist()) {
    unsigned col_count = 0;
    AstColumn* col = dynamic_cast<AstColumn*>(insert_ast_->col_list_);
    if (NULL == col) {
      is_all_col_ = true; // insert all columns
    }
    else { // get insert column count
      ++col_count;
      while ((col = dynamic_cast<AstColumn*>(col->next_)))
        ++col_count;
    }

    AstInsertValList* insert_value_list =
        dynamic_cast<AstInsertValList*>(insert_ast_->insert_val_list_);
    if (NULL == insert_value_list) {
      error_msg_ = "No value!";
      result_flag_ = false;
      result_set_ = NULL;

      ret = common::kStmtHandlerInsertNoValue;
    }
    else {
      std::ostringstream ostr;
      int changed_row_num = 0;

      // get data in one row like (...), (...), (...) by while loop.
      while (insert_value_list) {

        AstInsertVals *insert_value =
            dynamic_cast<AstInsertVals *>(insert_value_list->insert_vals_);
        col = dynamic_cast<AstColumn*>(insert_ast_->col_list_);

        if (is_all_col_) {
          // by scdong: Claims adds a default row_id attribute for all tables which is attribute(0),
          // when inserting tuples we should begin to construct the string_tuple from the second attribute.
          for (unsigned int position = 1;
              position < table_desc_->getNumberOfAttribute(); position++) {
            // check value count
            if (insert_value == NULL) {
              // ASTParserLogging::elog("Value count is too few");
              is_correct_ = false;
              error_msg_ = "Value count is too few";
              result_flag_ = false;
              result_set_ = NULL;
              break;
            }

            // insert value to ostringstream and if has warning return 1;   look out   the order!
            has_warning_ = InsertValueToStream(insert_value, table_desc_,
                                               position, ostr) || has_warning_;
            // move to next
            insert_value = dynamic_cast<AstInsertVals *>(insert_value->next_);
            ostr << "|";
          }

          if (!is_correct_) break;
          // check insert value count
          if (insert_value) {
            error_msg_ = "Value count is too many";
            result_flag_ = false;
            result_set_ = NULL;
            is_correct_ = false;
            break;
          }

        }
        else {
          // get insert value count and check whether it match column count
          unsigned insert_value_count = 0;
          while (insert_value) {
            ++insert_value_count;
            insert_value = dynamic_cast<AstInsertVals *>(insert_value->next_);
          }
          if (insert_value_count != col_count) {
            //ASTParserLogging::elog("Column count doesn't match value count");
            error_msg_ = "Column count doesn't match value count";
            LOG(ERROR) << "Column count doesn't match value count" << std::endl;
            result_flag_ = false;
            result_set_ = NULL;
            is_correct_ = false;
            break;
          }
          unsigned int used_col_count = 0;
          // by scdong: Claims adds a default row_id attribute for all tables which
          // is attribute(0), when inserting tuples we should begin to construct the
          // string_tuple from the second attribute.
          for (unsigned int position = 1;
              position < table_desc_->getNumberOfAttribute(); position++) {
            // find the matched column and value by name
            col = dynamic_cast<AstColumn*>(insert_ast_->col_list_);
            AstInsertVals *insert_value =
                dynamic_cast<AstInsertVals *>(insert_value_list->insert_vals_);

            // take attention that attrName is tablename.colname
            // here is temporary code and need to change later
            while (col
                && (table_desc_->getAttribute(position).attrName).compare(
                    table_desc_->getTableName() + "." + col->relation_name_)) {
              col = dynamic_cast<AstColumn*>(col->next_);
              insert_value = dynamic_cast<AstInsertVals*>(insert_value->next_);
            }

            // if find the column count is proved to match the insert value count, so column
            // exist, then insert_value exist
            if (col && insert_value) {
              ++used_col_count;
              // insert value to ostringstream and if has warning return 1; look out
              // the order!
              has_warning_ = InsertValueToStream(insert_value, table_desc_,
                                                 position, ostr)
                  || has_warning_;
            }
            ostr << "|";
          }

          // check if every insert column is existed
          if (used_col_count != col_count) {
            // ASTParserLogging::elog("Some columns don't exist");
            error_msg_ = "Some columns don't exist";
            LOG(ERROR) << "Some columns don't exist" << std::endl;
            result_flag_ = false;
            result_set_ = NULL;
            is_correct_ = false;
            break;
          }

        }

        if (!is_correct_) break;

        insert_value_list = dynamic_cast<AstInsertValList*>(insert_value_list
            ->next_);
        if (insert_value_list != NULL) ostr << "\n";

        ++changed_row_num;

      }  // while

      if (is_correct_) {
        if (has_warning_) {
          //ASTParserLogging::log("[WARNING]: The type is not matched!\n");
          LOG(WARNING) <<  "The type is not matched!" << std::endl;
        }
        //ASTParserLogging::log("the insert content is \n%s\n", ostr.str().c_str());
        LOG(INFO) << "the insert content is " << std::endl << ostr.str() << std::endl;

        HdfsLoader* Hl = new HdfsLoader(table_desc_);
        string tmp = ostr.str();
        Hl->append(ostr.str());

        Environment::getInstance()->getCatalog()->saveCatalog();
        result_flag_ = true;
        ostr.clear();
        ostr.str("");
        ostr << "insert data successfully. " << changed_row_num
             << " rows changed.";
        info_ = ostr.str();
        result_set_ = NULL;

        ret = common::kStmtHandlerInsertDataSuccess;
      }
    }
  }
  else {
    error_msg_ = "The table " + tablename_ + "does not exist!";
    LOG(ERROR)<< "The table " + tablename_ + "does not exist!" << std::endl;
    result_flag_ = false;
    result_set_ = NULL;
    ret = common::kStmtHandlerTableNotExistDuringInsert;
  }

  return ret;
}

}  // namespace stmt_handler
}  // namespace claims
