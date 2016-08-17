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
 * /CLAIMS/sql_parser/ast_node/ast_update_stmt.h
 *
 *  Created on: Aug 16, 2016
 *      Author: cswang
 *		   Email: cs_wang@infosys.com
 *
 * Description:
 *
 */

#ifndef SQL_PARSER_AST_NODE_AST_UPDATE_STMT_H_
#define SQL_PARSER_AST_NODE_AST_UPDATE_STMT_H_

#include "../ast_node/ast_node.h"

// namespace claims {
// namespace ast_node {

class AstUpdateStmt : public AstNode {
 public:
  AstUpdateStmt(AstNodeType ast_node_type, 
  	AstNode* update_set_list, AstNode* update_table_list, 
  	AstNode* where_list);
  virtual AstUpdateStmt();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);

  AstNodeType ast_node_type_;
  AstNode* update_set_list_; 
  AstNode* update_table_list_;
  AstNode* where_list_;
};

class AstUpdateSetList : public AstNode {
 public:
  AstUpdateSetList(AstNodeType ast_node_type, AstNode* args0, AstNode* args1, AstNode* next);
  ~AstUpdateSetList();
  void Print(int level = 0) const;
  RetCode SemanticAnalisys(SemanticContext* sem_cnxt);
  RetCode PushDownCondition(PushDownConditionContext& pdccnxt);
  RetCode GetLogicalPlan(LogicalOperator*& logic_plan);
  
  AstNodeType ast_node_type_;
  AstNode* args0_;
  AstNode* args1_;
  AstNode* next_;
};


// } /* namespace ast_node */
// } /* namespace claims */

#endif  //  SQL_PARSER_AST_NODE_AST_DELETE_STMT_H_
