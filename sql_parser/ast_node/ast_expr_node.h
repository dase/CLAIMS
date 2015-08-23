/*
 * ast_expr_node.h
 *  Created on: May 22, 2015 11:36:17 AM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */
/*
 * those classes in this file are used to describe the calculation tree of expression,
 * just store some main information, so don't match the class used to describe the
 * calculation tree of execution completely.
 */
#ifndef AST_EXPR_NODE_H_    //NOLINT
#define AST_EXPR_NODE_H_
#include <string>

#include "../ast_node/ast_node.h"
using std::string;

class AstExprConst : public AstNode {
 public:
  AstExprConst(AstNodeType ast_node_type, string expr_type, string data);
  ~AstExprConst();
  void Print(int level = 0) const;
  string expr_type_;
  string data_;
};

class AstExprUnary : public AstNode {
 public:
  AstExprUnary(AstNodeType ast_node_type, string expr_type, AstNode* arg0);
  ~AstExprUnary();
  void Print(int level = 0) const;
  AstNode* arg0_;
  string expr_type_;
  string expr_str_;
};
/*
 *
 */
class AstExprFunc : public AstNode {
 public:
  AstExprFunc(AstNodeType ast_node_type, string expr_type, AstNode* arg0,
              AstNode* arg1, AstNode* arg2);
  ~AstExprFunc();
  void Print(int level = 0) const;
  AstNode* arg0_;
  AstNode* arg1_;
  AstNode* arg2_;
  string expr_type_;
  string expr_str_;
};

class AstExprCalBinary : public AstNode {
 public:
  AstExprCalBinary(AstNodeType ast_node_type, string expr_type, AstNode* arg0,
                   AstNode* arg1);
  ~AstExprCalBinary();
  void Print(int level = 0) const;
  AstNode* arg0_;
  AstNode* arg1_;
  string expr_type_;
  string expr_str_;
};

class AstExprCmpBinary : public AstNode {
 public:
  AstExprCmpBinary(AstNodeType ast_node_type, string expr_type, AstNode* arg0,
                   AstNode* arg1);
  AstExprCmpBinary(AstNodeType ast_node_type, string cmp_para, int cmp_type,
                   AstNode* arg0, AstNode* arg1);

  ~AstExprCmpBinary();
  void Print(int level = 0) const;
  AstNode* arg0_;
  AstNode* arg1_;
  string expr_type_;
  string expr_str_;
  string cmp_para_;  //  "ALL","ANY","SOME","NULL","SUBQUERY"
};
class AstExprList : public AstNode {
 public:
  AstExprList(AstNodeType ast_node_type, AstNode* expr, AstNode* next);
  ~AstExprList();
  void Print(int level = 0) const;
  AstNode* expr_;
  AstNode* next_;
};

#endif /* AST_EXPR_NODE_H_ */    //NOLINT
