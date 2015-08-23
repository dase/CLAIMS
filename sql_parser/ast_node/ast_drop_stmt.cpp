/*
 *  /sql_parser/src/astnode/ast_drop_stmt.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSe
 *
 */

#include "../ast_node/ast_drop_stmt.h"
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

AstDropIndex::AstDropIndex(AstNodeType ast_node_type, string index_name,
                           string table_name)
    : AstNode(ast_node_type),
      index_name_(index_name),
      table_name_(table_name) {
}

AstDropIndex::~AstDropIndex() {
}

void AstDropIndex::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Drop Index|" << endl;
}

AstDropDatabase::AstDropDatabase(AstNodeType ast_node_type, int drop_type,
                                 int check, string db_name)
    : AstNode(ast_node_type),
      db_name_(db_name),
      drop_type_(drop_type),
      check_(check) {
}

AstDropDatabase::~AstDropDatabase() {
}

void AstDropDatabase::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Drop Database|" << endl;
}

AstDropTable::AstDropTable(AstNodeType ast_node_type, int is_temp, int is_check,
                           int opt_rc, AstNode* table_list)
    : AstNode(ast_node_type),
      is_temp_(is_temp),
      is_check_(is_check),
      option_rc_(opt_rc),
      table_list_(table_list) {
}

AstDropTable::~AstDropTable() {
  delete table_list_;
}

void AstDropTable::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Drop Table|" << endl;
  if (table_list_ != NULL) {
    table_list_->Print(level);
  }
}

AstDropTableList::AstDropTableList(AstNodeType ast_node_type, string db_name,
                                   string table_name, AstNode* next)
    : AstNode(ast_node_type),
      db_name_(db_name),
      table_name_(table_name),
      next_(next) {
}

AstDropTableList::~AstDropTableList() {
  delete next_;
}
void AstDropTableList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Drop Table List|" << endl;
}
