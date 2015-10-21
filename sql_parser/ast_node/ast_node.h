/*
 * ast_node.h
 *  Created on: May 21, 2015 4:10:35 PM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef AST_NODE_H_  // NOLINT
#define AST_NODE_H_
#include <string>
#include <map>
#include <set>
#include <vector>

using std::multimap;
using std::set;
using std::string;
typedef int ErrorNo;
enum AstNodeType {
  AST_NODE,
  AST_STMT_LIST,

  AST_SELECT_LIST,
  AST_SELECT_EXPR,
  AST_FROM_LIST,
  AST_TABLE,
  AST_SUBQUERY,
  AST_JOIN_CONDITION,
  AST_JOIN,
  AST_WHERE_CLAUSE,
  AST_GROUPBY_LIST,
  AST_GROUPBY_CLAUSE,
  AST_ORDERBY_LIST,
  AST_ORDERBY_CLAUSE,
  AST_HAVING_CLAUSE,
  AST_LIMIT_CLAUSE,
  AST_SELECT_INTO_CLAUSE,
  AST_COLUMN,
  AST_SELECT_STMT,
  AST_EXPR_LIST,
  AST_EXPR_CONST,
  AST_EXPR_UNARY,
  AST_EXPR_CAL_BINARY,
  AST_EXPR_CMP_BINARY,
  AST_EXPR_FUNC,

  AST_CREATE_DATABASE,
  AST_CREATE_SCHEMA,
  AST_CREATE_TABLE_LIST,
  AST_CREATE_TABLE_LIST_SEL,
  AST_CREATE_TABLE_SEL,
  AST_CREATE_COL_LIST,
  AST_CREATE_DEF_NAME,
  AST_CREATE_DEF_PR_KEY,
  AST_CREATE_DEF_KEY,
  AST_CREATE_DEF_INDEX,
  AST_CREATE_SEL,
  AST_CREATE_DEF_FTEXT_INDEX,
  AST_CREATE_DEF_FTEXT_KEY,
  AST_CREATE_INDEX,
  AST_LOAD_TABLE,
  AST_INDEX_COL,
  AST_INDEX_COL_LIST,
  AST_COLUMN_ATTS,
  AST_DATA_TYPE,
  AST_ENUM,
  AST_ENUM_LIST,
  AST_OPT_LENGTH,
  AST_OPT_CSC,
  AST_CREATE_PROJECTION,
  AST_CREATE_PROJECTION_NUM,
  AST_DROP_INDEX,
  AST_DROP_DB,
  AST_DROP_SCHEMA,
  AST_DROP_TABLE,
  AST_DROP_TABLE_LIST,
  AST_INSERT_STMT,
  AST_INSERT_VALUE_LIST,
  AST_INSERT_VALUE,
  AST_INSERT_ASSIGN_LIST,

  AST_INTNUM,
  AST_APPROXNUM,
  AST_STRINGVAL,
  AST_BOOL,
};
enum ErrorNoType {
  eOK = 10000,
  eTableNotExist,
  eTableillegal,
  eTableAliasEqualExistedTable,
  eTableAliasEqualLowerAlias,
  eColumnNotExist,
  eColumnIsAmbiguous,
  eColumnAliasIsAmbiguous,
  eColumnIsAmbiguousAfterAlias,
  eColumnIsAmbiguousToExistedColumn,
  eArgNotExist,
  eFromClauseIsNULL,
  eSelectClauseIsNULL,
};
const int TAB_SIZE = 4;
class SemanticContext {
 public:
  SemanticContext();
  ~SemanticContext();
  ErrorNo IsTableExist(const string table);
  ErrorNo IsColumnExist(string& table, const string column);
  ErrorNo AddTable(string table);
  ErrorNo AddTableColumn(const string& table, const string& column);
  ErrorNo AddTableColumn(const multimap<string, string>& column_to_table);
  ErrorNo GetAliasColumn(const string& alias,
                         multimap<string, string>& column_to_table);

 private:
  multimap<string, string> column_to_table_;
  set<string> tables_;
};
class AstNode {
 public:
  explicit AstNode(AstNodeType ast_node_type);
  virtual ~AstNode();
  virtual void Print(int level = 0) const;
  virtual ErrorNo SemanticAnalisys(SemanticContext* sem_cnxt);
  AstNodeType ast_node_type();
  AstNodeType ast_node_type_;
};
struct ParseResult {
  void* yyscan_info_;
  AstNode* ast;
  const char* sql_clause;
  int error_number;
};

/*
 * used to link every statement in one sql contains multiple statement
 * for example: select a from tb;select max(a) from tb2;
 */
class AstStmtList : public AstNode {
 public:
  AstStmtList(AstNodeType ast_node_type, AstNode* stmt, AstNode* next);
  ~AstStmtList();
  void Print(int level = 0) const;
  ErrorNo SemanticAnalisys(SemanticContext* sem_cnxt);
  AstNode* stmt_;
  AstNode* next_;
};

#endif  // AST_NODE_H__    //  NOLINT
