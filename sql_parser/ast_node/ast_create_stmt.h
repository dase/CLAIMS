/*
 *  /sql_parser/src/astnode/ast_create_stmt.h
 *
 *  Created on: Jul 23, 2015
 *      Author: fish
 *       Email: youngfish93@hotmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSe
 *
 */

#ifndef SQL_PARSER_AST_NODE_AST_CREATE_STMT_H_
#define SQL_PARSER_AST_NODE_AST_CREATE_STMT_H_
#include <string>
#include <iostream>  //NOLINT

#include "../ast_node/ast_node.h"

using std::string;
// namespace claims {
// namespace sql_parser {
/**
 * @brief The AST of create database statement.
 * @details AstCreateDatabase mainly includes the database name and create type.
 * The member create_type_ stands for the create database(1) and create
 * scheme(2).
 * The member check_ stands for having EXIST statement.(1 means having EXIST
 * while 0 means not.)
 */
class AstCreateDatabase : public AstNode {
 public:
  AstCreateDatabase(AstNodeType ast_node_type, int create_type, int check,
                    string db_name);
  ~AstCreateDatabase();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string db_name_;
  int create_type_;
  int check_;
};

/**
 * @brief The AST of create table statement.
 * @details AstCreateTable mainly includes table name, database name and its
 * list.
 * The AstNode* list will point to the column list information.
 * The member check_ stands for having EXIST statement.(1 means having EXIST
 * while 0 means not.)
 * The member create_type stands for having temporary statement or not.
 */
class AstCreateTable : public AstNode {
 public:
  AstCreateTable(AstNodeType ast_node_type, int create_type, int check,
                 string table_name, string additional_name, AstNode* list,
                 AstNode* select_stmt);
  ~AstCreateTable();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string table_name_;
  string additional_name_;
  int check_;
  int create_type_;
  AstNode* col_list_;
  AstNode* select_stmt_;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
};
/**
 * @brief The list of columns from creating new table.
 * @details The member AstNode* data_ point to the data information of the
 * column.
 * AstNode* next_ point to next column.
 */
class AstCreateColList : public AstNode {
 public:
  AstCreateColList(AstNodeType ast_node_type, AstNode* data, AstNode* next);
  ~AstCreateColList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  AstNode* data_;
  AstNode* next_;
};
/**
 * @brief The definition of column from create table statement.
 * @details The int def_type_ means the type of definition.(1 means common
 * definition; 2 means PRIMARY KEY; 3 means KEY; 4 means INDEX; 5 means FULLTEXT
 * INDEX; 6 means FULLTEXT KEY )
 * The data_type includes the data_type structure of columns(e.x. int, string
 * etc).
 * The col_atts_ includes the attributions of columns.
 */
class AstCreateDef : public AstNode {
 public:
  AstCreateDef(AstNodeType ast_node_type, int def_type, string name,
               AstNode* data_type_, AstNode* col_atts, AstNode* col_list);
  ~AstCreateDef();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int def_type_;
  string col_name_;
  AstNode* data_type_;
  AstNode* col_atts_;
  AstNode* col_list_;
};
/**
 * @brief The AST of create select into statement.
 * @details AstNode* select_stmt points to a select statement.
 * The member ignore_replace_ stands for "IGNORE" and "REPLACE". (1 means
 * IGNORE; 2 means REPLACE; 0 means none of them.)
 * The member temporary_ stands for "TEMPORARY".
 */
class AstCreateSelect : public AstNode {
 public:
  AstCreateSelect(AstNodeType ast_node_type, int ignore_replace_,
                  int temporary_, AstNode* select_stmt_);
  ~AstCreateSelect();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int ignore_replace_;
  int temporary_;
  AstNode* select_stmt_;
};
/**
 * @brief The AST of create projection statement.
 * @details AstCreateProjection mainly includes table name, partition number,
 * partition name and column list.
 */
class AstCreateProjection : public AstNode {
 public:
  AstCreateProjection(AstNodeType ast_node_type, string table_name,
                      AstNode* column_list, int partition_num,
                      string partition_attribute_name);
  ~AstCreateProjection();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  AstNodeType ast_node_type_;
  string table_name_;
  int partition_num_;
  string partition_attribute_name_;
  AstNode* column_list_;
};

/***
 * @brief The column attributes of create table statement.
 * @details  The data type contains NOT NULLX, NULLX, DEFAULT STRING, DEFAULT
 * INTNUM, DEFAULT APPROXNUM, DEFAULT BOOL, AUTO_INCREMENT, UNIQUE KEY, PRIMARY
 * KEY, KEY, COMMENT STRING and UNIQUE.
 * And double_num_, int_num_ and default_stirng separately mean default values.
 */
class AstColumnAtts : public AstNode {
 public:
  AstColumnAtts(AstNodeType ast_node_type, int datatype, int int_num,
                double double_num, string default_string, AstNode* col_list);
  ~AstColumnAtts();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int datatype_;
  int int_num_;
  double double_num_;
  string default_string_;
  AstNode* col_list_;

 private:
};

/***
 * @brief The AST of create index statement.
 * @details AstCreateIndex mainly includes index attributes, index type, index
 * name and index column names.
 * The member index_att_ stands for the attributes of create index.(1 means
 * UNNIQUE; 2 means FULLTEXT; 3 means SPATIAL; 0 means none of them.)
 * The member index_type_ stands for the USING options.(1 means BTREE and 2
 * means
 * HASH)
 */
class AstCreateIndex : public AstNode {
 public:
  AstCreateIndex(AstNodeType ast_node_type, int index_att, string index_name,
                 int index_type, string table_name, AstNode* index_col_name);
  ~AstCreateIndex();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int index_att_;
  string index_name_;
  int index_type_;
  string table_name_;
  AstNode* index_col_name_;

 private:
};

/***
 * @brief The information of index column list from create index statement.
 * @details AstIndexColList mainly includes the index length, direction and
 * name.
 * The asc_desc_ stands for direction of index.(0 means ASC and 1 means DESC.)
 * We use ASC as default.
 */
class AstIndexColList : public AstNode {
 public:
  AstIndexColList(AstNodeType ast_node_type, string name, AstNode* length,
                  int asc_desc, AstNode* next);
  ~AstIndexColList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string name_;
  AstNode* length_;
  int asc_desc_;
  AstNode* next_;

 private:
};

/***
 * @brief The data type in create statement.
 * @details AstDataType mainly includes data type name, length and other
 * attributes.
 * The following is an overview of the numeric column types:
 * int data_type:
   | BIT        1
   | TINYINT    2
   | SMALLINT   3
   | MEDIUMINT  4
   | INT        5
   | INTEGER    6
   | BIGINT     7
   | REAL       8
   | DOUBLE     9
   | FLOAT      10
   | DECIMAL    11
   | DATE       12
   | TIME       13
   | TIMESTAMP  14
   | DATETIME   15
   | YEAR       16
   | CHAR       17
   | VARCHAR    18
   | BINARY     19
   | VARBINARY  20
   | TINYBLOB   21
   | BLOB       22
   | MEDIUMBLOB 23
   | LONGBLOB   24
   | TINYTEXT   25
   | TEXT       26
   | MEDIUMTEXT 27
   | LONGTEXT   28
   | ENUM       29
   | SET        30
 * The member int opt_uz has UNSIGNED and ZEROFILL two values.
 */
class AstDataType : public AstNode {
 public:
  AstDataType(AstNodeType ast_node_type, int data_type, AstNode* length,
              int opt_uz, AstNode* opt_csc, int isbinary, AstNode* enum_list);
  ~AstDataType();
  void Print(int level = 0) const;

  AstNodeType ast_node_type_;
  int data_type_;
  AstNode* length_;
  int opt_uz_;
  AstNode* opt_csc_;
  int isbinary_;
  AstNode* enum_list_;

 private:
};
/**
 * @brief The data structure of enum in columns data types.
 * @details AstEnumList mainly includes enum name and next pointer.
 */
class AstEnumList : public AstNode {
 public:
  AstEnumList(AstNodeType ast_node_type, string enum_name_, AstNode* next_);
  ~AstEnumList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string enum_name_;
  AstNode* next_;

 private:
};

/***
 * @brief The data length in columns data types.
 */
class AstOptLength : public AstNode {
 public:
  AstOptLength(AstNodeType ast_node_type, int data1, int data2);
  ~AstOptLength();
  void Print(int level = 0) const;

  AstNodeType ast_node_type_;
  int data1_;
  int data2_;

 private:
};

class AstOptCsc : public AstNode {
 public:
  AstOptCsc(AstNodeType ast_node_type, int data_type, string str1, string str2);
  ~AstOptCsc();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int data_type_;
  string str1_;
  string str2_;

 private:
};
//}  // namespace sql_parser
//}  // namespace claims

#endif  // SQL_PARSER_AST_NODE_AST_CREATE_STMT_H_
