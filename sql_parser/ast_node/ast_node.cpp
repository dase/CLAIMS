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
 * ./sql_parser/ast_node.cpp
 *  Created on: May 21, 2015 4:10:35 PM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *
 */

#include "../ast_node/ast_node.h"
#include <glog/logging.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <utility>

using std::cout;
using std::setw;
using std::endl;
using std::string;
using std::endl;

AstNode::AstNode(AstNodeType ast_node_type) : ast_node_type_(ast_node_type) {}

AstNode::~AstNode() {}
void AstNode::Print(int level) const {
  cout << setw(level * 8) << " "
       << "This is an AST_NODE!" << endl;
}
ErrorNo AstNode::SemanticAnalisys(SemanticContext* sem_cnxt) {
  LOG(WARNING) << "This is AstNode's semantic analysis" << endl;
  return eOK;
}

AstNodeType AstNode::ast_node_type() { return ast_node_type_; }

AstStmtList::AstStmtList(AstNodeType ast_node_type, AstNode* stmt,
                         AstNode* next)
    : AstNode(ast_node_type), stmt_(stmt), next_(next) {}

AstStmtList::~AstStmtList() {
  delete stmt_;
  delete next_;
}

void AstStmtList::Print(int level) const {
  cout << setw(level * 8) << " "
       << "|stmt list|" << endl;
  if (stmt_ != NULL) {
    stmt_->Print(level);
  }
  if (next_ != NULL) {
    next_->Print(level++);
  }
}
ErrorNo AstStmtList::SemanticAnalisys(SemanticContext* sem_cnxt) {
  ErrorNo ret = eOK;
  if (NULL != stmt_) {
    ret = stmt_->SemanticAnalisys(sem_cnxt);
    if (eOK != ret) {
      return ret;
    }
  }
  if (NULL != next_) {
    return next_->SemanticAnalisys(sem_cnxt);
  }
  return eOK;
}
SemanticContext::SemanticContext() {
  tables_.clear();
  column_to_table_.clear();
}

SemanticContext::~SemanticContext() {}

ErrorNo SemanticContext::IsTableExist(const string table) {
  if (1 != tables_.count(table)) return eTableNotExist;
  return eOK;
}
/**
 * "NULL".column could be ambiguous or not exist, otherwise replace "NULL" with
 * unique name
 */
ErrorNo SemanticContext::IsColumnExist(string& table, const string column) {
  int table_counts = column_to_table_.count(column);
  if (table_counts == 0) {
    return eColumnNotExist;
  } else if (table_counts > 1) {       // ambiguous
    if (table.compare("NULL") == 0) {  // NULL.col
      return eColumnIsAmbiguous;
    } else {  // tbl.col
      auto table_range = column_to_table_.equal_range(table);
      for (auto it = table_range.first; it != table_range.second; ++it) {
        if (it->second.compare(table) == 0) {
          return eOK;
        }
      }
      return eColumnIsAmbiguous;
    }
  } else {  // unique col
    auto it = column_to_table_.find(column);
    if (table.compare("NULL") == 0) {  // NULL.col
      table = it->second;
      return eOK;
    } else {  // tbl.col
      if (it->second.compare(table) == 0) {
        return eOK;
      } else {
        return eColumnNotExist;
      }
    }
  }
}

ErrorNo SemanticContext::AddTable(string table) {
  tables_.insert(table);
  return eOK;
}
/**
 * if <table,column> doesn't exist in column_to_table_, so add into it,
 * otherwise, ignore it.
 */
ErrorNo SemanticContext::AddTableColumn(const string& table,
                                        const string& column) {
  if (table.compare("NULL") == 0) {
    LOG(ERROR) << "table name couldn't be 'NULL'!";
    return eTableillegal;
  } else {  // guarantee the <table,column> is unique
    int counts = 0;
    auto table_range = column_to_table_.equal_range(table);
    for (auto it = table_range.first; it != table_range.second; ++it) {
      if (it->second.compare(table) == 0) {
        counts++;
      }
    }
    if (0 == counts) {
      column_to_table_.insert(make_pair(column, table));
    }
  }
  return eOK;
}

/**
 * check tb.col to add is ambiguous to existed tb.col
 */
ErrorNo SemanticContext::AddTableColumn(
    const multimap<string, string>& column_to_table) {
  for (auto it_column = column_to_table_.begin();
       it_column != column_to_table_.end(); ++it_column) {
    for (auto it = column_to_table.begin(); it != column_to_table.end(); ++it) {
      if (it_column->first.compare(it->first) == 0 &&
          it_column->second.compare(it->second) == 0) {
        return eColumnIsAmbiguousToExistedColumn;
      }
    }
  }
  column_to_table_.insert(column_to_table.begin(), column_to_table.end());
  return eOK;
}
/**
 * check <tb1.a,tb2.a> isn't ambiguous in subquery, but <alias.a,alias.a> is
 * ambiguous at upper level
 */
ErrorNo SemanticContext::GetAliasColumn(
    const string& alias, multimap<string, string>& column_to_table) {
  for (auto it = column_to_table_.begin(); it != column_to_table_.end(); ++it) {
    if (column_to_table_.count(it->first) > 1) {
      return eColumnIsAmbiguousAfterAlias;
    }
    column_to_table.insert(make_pair(alias, it->second));
  }
  return eOK;
}
