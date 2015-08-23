/*
 *  /sql_parser/src/astnode/ast_load_stmt.cpp
 *
 *  Created on: Aug 4, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSe
 *
 */

#include "../ast_node/ast_load_stmt.h"
#include <iostream>   //  NOLINT
#include <iomanip>
#include <string>
#include <bitset>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::setw;
using std::bitset;

AstLoadTable::AstLoadTable(AstNodeType ast_node_type, string table_name,
                           AstNode* path, string column_separator,
                           string tuple_separator, double sample, int mode)
    : AstNode(ast_node_type),
      table_name_(table_name),
      path_(path),
      column_separator_(column_separator),
      tuple_separator_(tuple_separator),
      sample_(sample),
      mode_(mode) {
}

AstLoadTable::~AstLoadTable() {
  delete path_;
}

void AstLoadTable::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Load Table|" << "    ";
  cout << " Table name: " << table_name_ << endl;
  cout << setw(level * TAB_SIZE) << " " << " Path: " << endl;
  if (path_ != NULL) {
    path_->Print(++level);
  }
}
