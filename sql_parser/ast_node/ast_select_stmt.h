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
 * /sql_parser/ast_node/ast_select_stmt.h
 *  Created on: May 22, 2015 11:32:03 AM
 *  Modified on: Nov 16, 2015
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 */

#ifndef SQL_PARSER_AST_NODE_AST_SELECT_STMT_H_
#define SQL_PARSER_AST_NODE_AST_SELECT_STMT_H_
#include <map>
#include <set>
#include <string>
#include <vector>

#include "./ast_node.h"
#include "../../common/expression/expr_node.h"
#include "../../logical_operator/logical_operator.h"
using claims::logical_operator::LogicalOperator;
using std::string;
using std::vector;
// namespace claims {
// namespace sql_parser {
/**
 * @brief The AST of select list.
 * @details The member bool is_all_ stands for "select *" statement.
 */
class AstSelectList : public AstNode {
 public:
  AstSelectList(AstNodeType ast_node_type, bool is_all, AstNode* args,
                AstNode* next);
  ~AstSelectList();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  void RecoverExprName(string& name);
  void ReplaceAggregation(AstNode*& agg_column, set<AstNode*>& agg_node,
                          bool need_collect);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);

  bool is_all_;
  AstNode* args_;
  AstNode* next_;
};
/**
 * @brief The AST of select expression statement.
 */
class AstSelectExpr : public AstNode {
 public:
  AstSelectExpr(AstNodeType ast_node_type, string expr_alias, AstNode* expr);
  ~AstSelectExpr();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  void RecoverExprName(string& name);
  void ReplaceAggregation(AstNode*& agg_column, set<AstNode*>& agg_node,
                          bool need_collect);
  string expr_alias_;
  AstNode* expr_;
  bool have_agg_func_;
  bool have_alias_;
};
/**
 * @brief The AST of from list.
 * @details TODO(fzh):condition_ is not used in this version.
 */
class AstFromList : public AstNode {
 public:
  AstFromList(AstNodeType ast_node_type, AstNode* args, AstNode* next);
  ~AstFromList();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  RetCode PushDownCondition(PushDownConditionContext* pdccnxt);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);

  map<string, AstNode*> table_joined_root;
  AstNode* args_;
  AstNode* next_;
  AstNode* condition_;
  set<AstNode*> equal_join_condition_;
  set<AstNode*> normal_condition_;
};
/**
 * @brief The AST of table.
 * @details AstTable mainly includes database name, table name, table alias
 * and table id.
 */
class AstTable : public AstNode {
 public:
  AstTable(AstNodeType ast_node_type, string db_name, string table_name,
           string table_alias);
  ~AstTable();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  RetCode PushDownCondition(PushDownConditionContext* pdccnxt);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);

  set<AstNode*> equal_join_condition_;
  set<AstNode*> normal_condition_;
  string db_name_;
  string table_name_;
  string table_alias_;
  int table_id_;
  // AstNode* condition_; //
};
/**
 * @brief The AST of sub query statement.
 * @details AstSubquery mainly includes sub query alias and a pointer to
 * select statement.
 */
class AstSubquery : public AstNode {
 public:
  AstSubquery(AstNodeType ast_node_type, string subquery_alias,
              AstNode* subquery);
  ~AstSubquery();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  RetCode PushDownCondition(PushDownConditionContext* pdccnxt);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);

  string subquery_alias_;
  AstNode* subquery_;
  set<AstNode*> equal_join_condition_;
  set<AstNode*> normal_condition_;
};
/**
 * @brief The AST of join condition.
 */
class AstJoinCondition : public AstNode {
 public:
  AstJoinCondition(AstNodeType ast_node_type, string join_condition_type,
                   AstNode* condition);
  ~AstJoinCondition();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  string join_condition_type_;
  AstNode* condition_;
};
/**
 * @brief The AST of join statement.
 * @details AstJoin mainly includes join_type, pointer to left table, pointer to
 * right table and pointer to join condition.
 */
class AstJoin : public AstNode {
 public:
  AstJoin(AstNodeType ast_node_type, int join_type, AstNode* left_table,
          AstNode* right_table, AstNode* join_condition);
  ~AstJoin();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  RetCode PushDownCondition(PushDownConditionContext* pdccnxt);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);

  string join_type_;
  AstNode* left_table_;
  AstNode* right_table_;
  AstJoinCondition* join_condition_;
  set<AstNode*> equal_join_condition_;
  set<AstNode*> normal_condition_;
};
/**
 * @brief The AST of where clause.
 */
class AstWhereClause : public AstNode {
 public:
  AstWhereClause(AstNodeType ast_node_type, AstNode* expr);
  ~AstWhereClause();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  void RecoverExprName(string& name);
  AstNode* expr_;
};
/**
 * @brief The AST of group by list.
 */
class AstGroupByList : public AstNode {
 public:
  AstGroupByList(AstNodeType ast_node_type, AstNode* expr, AstNode* next);
  ~AstGroupByList();
  void Print(int level = 0) const;
  // TODO(FZH) need to support expression and be sure group attributes are
  // different from each other
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  void RecoverExprName(string& name);
  RetCode SolveSelectAlias(SelectAliasSolver* const select_alias_solver);

  AstNode* expr_;
  AstNode* next_;
};
/**
 * @brief The AST of group by clause.
 */
class AstGroupByClause : public AstNode {
 public:
  AstGroupByClause(AstNodeType ast_node_type, AstNode* groupby_list,
                   bool with_roolup);
  ~AstGroupByClause();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  void RecoverExprName(string& name);
  RetCode SolveSelectAlias(SelectAliasSolver* const select_alias_solver);
  AstGroupByList* groupby_list_;
  bool with_roolup_;
};
/**
 * @brief The AST of order by list.
 */
class AstOrderByList : public AstNode {
 public:
  explicit AstOrderByList(AstNodeType ast_node_type, AstNode* expr,
                          int orderby_type, AstNode* next);
  ~AstOrderByList();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  void RecoverExprName(string& name);
  void ReplaceAggregation(AstNode*& agg_column, set<AstNode*>& agg_node,
                          bool need_collect);
  RetCode SolveSelectAlias(SelectAliasSolver* const select_alias_solver);
  AstNode* expr_;
  string orderby_direction_;
  AstNode* next_;
};
/**
 * @brief The AST of order by clause.
 */
class AstOrderByClause : public AstNode {
 public:
  AstOrderByClause(AstNodeType ast_node_type, AstNode* orderby_list);
  ~AstOrderByClause();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  void RecoverExprName(string& name);
  void ReplaceAggregation(AstNode*& agg_column, set<AstNode*>& agg_node,
                          bool need_collect);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);
  RetCode SolveSelectAlias(SelectAliasSolver* const select_alias_solver);
  AstOrderByList* orderby_list_;
};
/**
 * @brief The AST of having clause.
 */
class AstHavingClause : public AstNode {
 public:
  AstHavingClause(AstNodeType ast_node_type, AstNode* expr);
  ~AstHavingClause();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  void RecoverExprName(string& name);
  void ReplaceAggregation(AstNode*& agg_column, set<AstNode*>& agg_node,
                          bool need_collect);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);
  RetCode SolveSelectAlias(SelectAliasSolver* const select_alias_solver);
  AstNode* expr_;
};
/**
 * @brief The AST of limit clause.
 */
class AstLimitClause : public AstNode {
 public:
  AstLimitClause(AstNodeType ast_node_type, AstNode* offset,
                 AstNode* row_count);
  ~AstLimitClause();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);
  AstNode* offset_;
  AstNode* row_count_;
};
/**
 * TODO(fzh):not used in this version.
 */
class AstSelectIntoClause : public AstNode {
 public:
  // AstSelectIntoClause();
  // ~AstSelectIntoClause();
  // void Print(int level = 0) const;
};
/**
 * @brief The AST of AstColumn.
 */
class AstColumn : public AstNode {
 public:
  AstColumn(AstNodeType ast_node_type, string relation_name,
            string column_name);
  AstColumn(AstNodeType ast_node_type, string relation_name, string column_name,
            string expr_str);
  AstColumn(AstNodeType ast_node_type, string relation_name, string column_name,
            AstNode* next);
  explicit AstColumn(AstColumn* node);
  ~AstColumn();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  void RecoverExprName(string& name);
  void GetRefTable(set<string>& ref_table);
  RetCode GetLogicalPlan(ExprNode*& logic_expr,
                         LogicalOperator* child_logic_plan);
  RetCode SolveSelectAlias(SelectAliasSolver* const select_alias_solver);
  AstNode* AstNodeCopy();
  string relation_name_;
  string column_name_;
  AstNode* next_;
};
/**
 * @brief The AST of select statement.
 * @details AstSelectStmt is the beginning of a SQL AST. So it has pointers to
 * all other clauses.
 * NOTE: A select_expr can be given an alias using AS alias_name. The alias is
 * used as the expression's column name and can be used in GROUP BY, ORDER BY,
 * or HAVING clauses.
 */
class AstSelectStmt : public AstNode {
 public:
  enum SelectOpts {
    SELECT_ALL,
    SELECT_DISTINCT,
    SELECT_DISTINCTROW,
  };
  AstSelectStmt(AstNodeType ast_node_type, int select_opts,
                AstNode* select_list, AstNode* from_list, AstNode* where_clause,
                AstNode* groupby_clause, AstNode* having_clause,
                AstNode* orderby_clause, AstNode* limit_clause,
                AstNode* select_into_clause);
  ~AstSelectStmt();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  RetCode PushDownCondition(PushDownConditionContext* pdccnxt);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);
  RetCode GetLogicalPlanOfAggeration(LogicalOperator*& logic_plan);
  RetCode GetLogicalPlanOfProject(LogicalOperator*& logic_plan);

  string select_str_;
  SelectOpts select_opts_;
  AstNode* select_list_;
  AstNode* from_list_;
  AstNode* where_clause_;
  AstNode* groupby_clause_;
  AstNode* having_clause_;
  AstNode* orderby_clause_;
  AstNode* limit_clause_;
  AstNode* select_into_clause_;
  set<AstNode*> groupby_attrs_;
  set<AstNode*> agg_attrs_;

  bool have_aggeragion_;
};
//}  // namespace sql_parser
//}  // namespace claims

#endif  //  SQL_PARSER_AST_NODE_AST_SELECT_STMT_H_
