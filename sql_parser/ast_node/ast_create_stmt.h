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
  AstNode* list_;
  AstNode* select_stmt_;
};

class AstCreateColList : public AstNode {
 public:
  AstCreateColList(AstNodeType ast_node_type, AstNode* data, AstNode* next);
  ~AstCreateColList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  AstNode* data_;
  AstNode* next_;
};

class AstCreateDef : public AstNode {
 public:
  AstCreateDef(AstNodeType ast_node_type, int def_type, string name,
               AstNode* data_type_, AstNode* col_atts, AstNode* col_list);
  ~AstCreateDef();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int def_type_;
  string name_;
  AstNode* data_type_;
  AstNode* col_atts_;
  AstNode* col_list_;
};

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

class AstCreateProjection : public AstNode {
 public:
  AstCreateProjection(AstNodeType ast_node_type, string table_name,
                      AstNode* column_list, int partition_num,
                      string partition_attribute_name);
  ~AstCreateProjection();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string table_name_;
  int partition_num_;
  string partition_attribute_name_;
  AstNode* column_list_;
};

/***
 * @brief Column Atts
 */
class AstColumnAtts : public AstNode {
 public:
  /**
   * @brief AstColumnAtts Constructor
   * @param
   * @details  
   */
  AstColumnAtts(AstNodeType ast_node_type, int datatype, int int_num,
                double double_num, string default_string, AstNode* col_list);
  /**
   * @brief AstColumnAtts Destructor.
   * @param
   * @details  here is an example (additional)
   */
  ~AstColumnAtts();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int datatype_;
  int int_num_;
  double double_num_;
  string default_string_;
  AstNode * col_list_;

 private:
};

/***
 * @brief An Ast of "CREATE INDEX" Statement
 */
class AstCreateIndex : public AstNode {
 public:
  /**
   * @brief AstCreateIndex Constructor.
   * @param
   * @details  here is an example (additional)
   */
  AstCreateIndex(AstNodeType ast_node_type, int index_att, string index_name,
                 int index_type, string table_name, AstNode* index_col_name);
  /**
   * @brief AstCreateIndex Destructor.
   * @param
   * @details  here is an example (additional)
   */
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
 * @brief
 */
class AstIndexColList : public AstNode {
 public:
  /**
   * @brief AstIndexColList Constructor.
   * @param
   * @details  here is an example (additional)
   */
  AstIndexColList(AstNodeType ast_node_type, string name, AstNode* length,
                  int asc_desc, AstNode* next);
  /**
   * @brief AstIndexColList Destructor.
   * @param
   * @details  here is an example (additional)
   */
  ~AstIndexColList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string name_;
  AstNode* length_;
  int asc_desc_;
  AstNode * next_;

 private:
};

/***
 * @brief the Data Type of "CREATE stmt"
 */
class AstDataType : public AstNode {
 public:
  /**
   * @brief AstDataType Constructor.
   * @param
   * @details  here is an example (additional)
   */
  AstDataType(AstNodeType ast_node_type, int data_type, AstNode* length,
              int opt_uz, AstNode* opt_csc, int isbinary, AstNode* enum_list);
  /**
   * @brief AstDataType Destructor.
   * @param
   * @details  here is an example (additional)
   */
  ~AstDataType();
  void Print(int level = 0) const;

  AstNodeType ast_node_type_;
  int data_type_;
  AstNode * length_;
  int opt_uz_;
  AstNode * opt_csc_;
  int isbinary_;
  AstNode * enum_list_;

 private:
};

/***
 * @brief
 */
class AstEnumList : public AstNode {
 public:
  /**
   * @brief AstEnumList Constructor.
   * @param
   * @details  here is an example (additional)
   */
  AstEnumList(AstNodeType ast_node_type, string enum_name_, AstNode* next_);
  /**
   * @brief AstEnumList Destructor.
   * @param
   * @details  here is an example (additional)
   */
  ~AstEnumList();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  string enum_name_;
  AstNode* next_;

 private:
};

/***
 * @brief
 */
class AstOptLength : public AstNode {
 public:
  /**
   * @brief AstOptLength Constructor.
   * @param
   * @details  here is an example (additional)
   */
  AstOptLength(AstNodeType ast_node_type, int data1, int data2);
  /**
   * @brief AstOptLength Destructor.
   * @param
   * @details  here is an example (additional)
   */
  ~AstOptLength();
  void Print(int level = 0) const;

  AstNodeType ast_node_type_;
  int data1_;
  int data2_;

 private:
};

/***
 * @brief
 */
class AstOptCsc : public AstNode {
 public:
  /**
   * @brief AstOptCsc Constructor.
   * @param
   * @details  here is an example (additional)
   */
  AstOptCsc(AstNodeType ast_node_type, int data_type, string str1, string str2);
  /**
   * @brief AstOptCsc Destructor.
   * @param
   * @details  here is an example (additional)
   */
  ~AstOptCsc();
  void Print(int level = 0) const;
  AstNodeType ast_node_type_;
  int data_type_;
  string str1_;
  string str2_;

 private:
};

#endif  // SQL_PARSER_AST_NODE_AST_CREATE_STMT_H_
