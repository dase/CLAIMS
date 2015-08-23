/*
 *  /sql_parser/src/astnode/ast_create_stmt.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSe
 *
 */

#include "../ast_node/ast_create_stmt.h"
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

AstCreateDatabase::AstCreateDatabase(AstNodeType ast_node_type, int create_type,
                                     int check, string db_name)
    : AstNode(ast_node_type),
      create_type_(create_type),
      check_(check),
      db_name_(db_name) {
}

AstCreateDatabase::~AstCreateDatabase() {
}

void AstCreateDatabase::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Create Database|" << endl;
}

AstCreateTable::AstCreateTable(AstNodeType ast_node_type, int create_type,
                               int check, string table_name,
                               string additional_name, AstNode * list,
                               AstNode * select_stmt)
    : AstNode(ast_node_type),
      create_type_(create_type),
      check_(check),
      table_name_(table_name),
      additional_name_(additional_name),
      list_(list),
      select_stmt_(select_stmt) {
}

AstCreateTable::~AstCreateTable() {
  delete list_;
  delete select_stmt_;
}

void AstCreateTable::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Create Table|" << "  "
      << table_name_ << endl;

  if (list_ != NULL) {
    list_->Print(level + 1);
  }
  if (select_stmt_ != NULL) {
    select_stmt_->Print(level + 1);
  }
}

AstCreateColList::AstCreateColList(AstNodeType ast_node_type, AstNode* data,
                                   AstNode* next)
    : AstNode(ast_node_type),
      data_(data),
      next_(next) {
}

AstCreateColList::~AstCreateColList() {
  delete data_;
  delete next_;
}

void AstCreateColList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Create Column List|" << endl;
  if (data_ != NULL) {
    data_->Print(level + 1);
  }
  if (next_ != NULL) {
    next_->Print(level);
  }
}

AstCreateDef::AstCreateDef(AstNodeType ast_node_type, int def_type, string name,
                           AstNode* data_type, AstNode* col_atts,
                           AstNode* col_list)
    : AstNode(ast_node_type),
      def_type_(def_type),
      name_(name),
      data_type_(data_type),
      col_atts_(col_atts),
      col_list_(col_list) {
}

AstCreateDef::~AstCreateDef() {
  delete data_type_;
  delete col_atts_;
  delete col_list_;
}

void AstCreateDef::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Create Definition|" << endl;
  cout << setw(level * TAB_SIZE) << " " << "Col Name: " << name_ << endl;
  cout << setw(level * TAB_SIZE) << " " << "    Type: ";

  if (data_type_ != NULL) {
    data_type_->Print(level);
  }
  if (col_atts_ != NULL) {
    col_atts_->Print(level);
  }
  if (col_list_ != NULL) {
    col_list_->Print(level);
  }
}

AstCreateSelect::AstCreateSelect(AstNodeType ast_node_type, int ignore_replace,
                                 int temporary, AstNode* select_stmt)
    : AstNode(ast_node_type),
      ignore_replace_(ignore_replace),
      temporary_(temporary),
      select_stmt_(select_stmt) {
}

AstCreateSelect::~AstCreateSelect() {
  delete select_stmt_;
}

void AstCreateSelect::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Create Selection|" << endl;
  if (select_stmt_ != NULL) {
    select_stmt_->Print(level + 1);
  }
}

AstCreateProjection::AstCreateProjection(AstNodeType ast_node_type,
                                         string table_name,
                                         AstNode* column_list,
                                         int partition_num,
                                         string partition_attribute_name)
    : AstNode(ast_node_type),
      table_name_(table_name),
      column_list_(column_list),
      partition_num_(partition_num),
      partition_attribute_name_(partition_attribute_name) {
}

AstCreateProjection::~AstCreateProjection() {
  delete column_list_;
}

void AstCreateProjection::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Create Projection|" << "  "
      << table_name_ << endl;
  cout << setw(++level * TAB_SIZE) << " " << "Prtition Number: "
      << partition_num_ << " ON " << partition_attribute_name_ << endl;

  if (column_list_ != NULL) {
    column_list_->Print(level);
  }
}

AstColumnAtts::AstColumnAtts(AstNodeType ast_node_type, int datatype,
                             int int_num, double double_num,
                             string default_string, AstNode* col_list)
    : AstNode(ast_node_type),
      datatype_(datatype),
      int_num_(int_num),
      double_num_(double_num),
      default_string_(default_string),
      col_list_(col_list) {
}

AstColumnAtts::~AstColumnAtts() {
  delete col_list_;
}

void AstColumnAtts::Print(int level) const {
  bitset < 12 > type;
  type = datatype_;
  if (type != 0) {
    cout << setw(level * TAB_SIZE) << " " << "Col Atts: ";

    if (type[0] == 1) {
      cout << " <NOT NULL> ";
    }
    if (type[1] == 1) {
      cout << " <NULL> ";
    }
    if (type[2] == 1) {
      cout << " <DEFAULT," << default_string_ << "> ";
    }
    if (type[3] == 1) {
      cout << " <DEFAULT " << int_num_ << "> ";
    }
    if (type[4] == 1) {
      cout << " <DEFAULT " << double_num_ << "> ";
    }
    if (type[5] == 1) {
      cout << " <DEFAULT " << int_num_ << "> ";
    }
    if (type[6] == 1) {
      cout << " <AUTO INCREMENT> ";
    }
    if (type[7] == 1) {
      cout << " <UNIQUE KEY> ";
    }
    if (type[8] == 1) {
      cout << " <PRIMARY KEY> ";
    }
    if (type[9] == 1) {
      cout << " <KEY> ";
    }
    if (type[10] == 1) {
      cout << " <COMMENT STRING> ";
    }
    if (type[11] == 1) {
      cout << " <UNIQUE> ";
    }
    cout << endl;
    if (col_list_ != NULL) {
      cout << ", ";
      col_list_->Print(level + 1);
    }
  }
}

AstCreateIndex::AstCreateIndex(AstNodeType ast_node_type, int index_att,
                               string index_name, int index_type,
                               string table_name, AstNode* index_col_name)
    : AstNode(ast_node_type),
      index_att_(index_att),
      index_name_(index_name),
      index_type_(index_type),
      table_name_(table_name),
      index_col_name_(index_col_name) {
}

AstCreateIndex::~AstCreateIndex() {
  delete index_col_name_;
}

void AstCreateIndex::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Create Index|" << endl;
  if (index_col_name_ != NULL) {
    index_col_name_->Print(level + 1);
  }
}

AstDataType::AstDataType(AstNodeType ast_node_type, int data_type,
                         AstNode* length, int opt_uz, AstNode* opt_csc,
                         int isbinary, AstNode* enum_list)
    : AstNode(ast_node_type),
      data_type_(data_type),
      length_(length),
      opt_uz_(opt_uz),
      opt_csc_(opt_csc),
      isbinary_(isbinary),
      enum_list_(enum_list) {
}

AstDataType::~AstDataType() {
  delete length_;
  delete opt_csc_;
  delete enum_list_;
}

void AstDataType::Print(int level) const {
  if (data_type_ == 1) {
    cout << "BIT" << " " << endl;
  } else if (data_type_ == 2) {
    cout << "TINYINT" << endl;
  } else if (data_type_ == 3) {
    cout << "SMALLINT" << endl;
  } else if (data_type_ == 4) {
    cout << "MEDIUMINT" << endl;
  } else if (data_type_ == 5) {
    cout << "INT" << endl;
  } else if (data_type_ == 6) {
    cout << "INTEGER" << endl;
  } else if (data_type_ == 7) {
    cout << "BIGINT" << endl;
  } else if (data_type_ == 8) {
    cout << "REAL" << endl;
  } else if (data_type_ == 9) {
    cout << "DOUBLE" << endl;
  } else if (data_type_ == 10) {
    cout << "FLOAT" << endl;
  } else if (data_type_ == 11) {
    cout << "DECIMAL" << endl;
  } else if (data_type_ == 12) {
    cout << "DATE" << endl;
  } else if (data_type_ == 13) {
    cout << "TIME" << endl;
  } else if (data_type_ == 14) {
    cout << "TIMESTAMP" << endl;
  } else if (data_type_ == 15) {
    cout << "DATETIME" << endl;
  } else if (data_type_ == 16) {
    cout << "YEAR" << endl;
  } else if (data_type_ == 17) {
    cout << "CHAR" << endl;
  } else if (data_type_ == 18) {
    cout << "VARCHAR" << endl;
  } else if (data_type_ == 19) {
    cout << "BINARY" << endl;
  } else if (data_type_ == 20) {
    cout << "VARBINARY" << endl;
  } else if (data_type_ == 21) {
    cout << "TINYBLOB" << endl;
  } else if (data_type_ == 22) {
    cout << "BLOB" << endl;
  } else if (data_type_ == 23) {
    cout << "MEDIUMBLOB" << endl;
  } else if (data_type_ == 24) {
    cout << "LONGBLOB" << endl;
  } else if (data_type_ == 25) {
    cout << "TINYTEXT" << endl;
  } else if (data_type_ == 26) {
    cout << "TEXT" << endl;
  } else if (data_type_ == 27) {
    cout << "MEDIUMTEXT" << endl;
  } else if (data_type_ == 28) {
    cout << "LONGTEXT" << endl;
  } else if (data_type_ == 29) {
    cout << "ENUM" << endl;
  } else if (data_type_ == 30) {
    cout << "SET" << endl;
  }

  if (length_ != NULL) {
    length_->Print(level);
  }
}

AstEnumList::AstEnumList(AstNodeType ast_node_type, string enum_name,
                         AstNode* next)
    : AstNode(ast_node_type),
      enum_name_(enum_name),
      next_(next) {
}

AstEnumList::~AstEnumList() {
  delete next_;
}

void AstEnumList::Print(int level) const {
}

AstOptLength::AstOptLength(AstNodeType ast_node_type, int data1, int data2)
    : AstNode(ast_node_type),
      data1_(data1),
      data2_(data2) {
}

AstOptLength::~AstOptLength() {
}

void AstOptLength::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "  Length: " << data1_ << " "
      << data2_ << endl;
}

AstOptCsc::AstOptCsc(AstNodeType ast_node_type, int data_type, string str1,
                     string str2)
    : AstNode(ast_node_type),
      data_type_(data_type),
      str1_(str1),
      str2_(str2) {
}

AstOptCsc::~AstOptCsc() {
}

void AstOptCsc::Print(int level) const {
}

AstIndexColList::AstIndexColList(AstNodeType ast_node_type, string name,
                                 AstNode* length, int asc_desc, AstNode* next)
    : AstNode(ast_node_type),
      name_(name),
      length_(length),
      asc_desc_(asc_desc),
      next_(next) {
}

AstIndexColList::~AstIndexColList() {
  delete length_;
  delete next_;
}

void AstIndexColList::Print(int level) const {
  cout << setw(level * TAB_SIZE) << " " << "|Index Col List|" << endl;
  if (next_ != NULL) {
    next_->Print(level);
  }
}
